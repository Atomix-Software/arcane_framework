#include <arcpch.h>

#include "Arcane/Audio/SoundBuffer.h"

#include <AL/al.h>
#include <inttypes.h>
#include <AL/alext.h>

namespace Arcane
{
	Shared<SoundLibrary> Arcane::SoundLibrary::Create()
	{
		return CreateShared<SoundLibrary>();
	}

	ALuint SoundLibrary::Load(const std::string& filename)
	{
		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		sndfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
		ARC_CORE_ASSERT(sndfile != NULL, "Could not open Audio File:" + std::string(filename) + ": " + sf_strerror(sndfile));

		if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t) (INT_MAX / sizeof(short)) / sfinfo.channels)
		{
			ARC_CORE_WARN("Bad sample count in {0} ({1})", filename, sfinfo.frames);
			sf_close(sndfile);
			return 0;
		}

		format = AL_NONE;
		switch (sfinfo.channels)
		{
			case 1: format = AL_FORMAT_MONO16; break;
			case 2: format = AL_FORMAT_STEREO16; break;
			case 3:
			{
				if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
					format = AL_FORMAT_BFORMAT2D_16;
				break;
			}
			case 4:
			{
				if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
					format = AL_FORMAT_BFORMAT3D_16;
				break;
			}
			default:
			{
				ARC_CORE_ERROR("Unsupported channel count: {0}", sfinfo.channels);
				sf_close(sndfile);
				return 0;
			}
		}

		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
		
		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1)
		{
			free(membuf);
			sf_close(sndfile);
			ARC_CORE_ERROR("Failed to read samples in {0} ({1}{2})", filename, PRId64, num_frames);
			return 0;
		}

		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

		buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

		free(membuf);
		sf_close(sndfile);

		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			ARC_CORE_ERROR("OpenAL Error: {0}", alGetString(err));
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);

			return 0;
		}

		m_SoundBuffers[filename] = buffer;
		return buffer;
	}

	bool SoundLibrary::Unload(const std::string& filename)
	{
		for (auto it = m_SoundBuffers.begin(); it != m_SoundBuffers.end(); ++it)
		{
			if (strcmp(it->first.c_str(), filename.c_str()) == 0)
			{
				alDeleteBuffers(1, &it->second);
				it = m_SoundBuffers.erase(it);

				return true;
			}
		}

		return false;
	}

	bool SoundLibrary::Unload(const ALuint& buffer)
	{
		for (auto it = m_SoundBuffers.begin(); it != m_SoundBuffers.end(); ++it)
		{
			if (it->second == buffer)
			{
				alDeleteBuffers(1, &it->second);
				it = m_SoundBuffers.erase(it);

				return true;
			}
		}

		return false;
	}

	SoundLibrary::SoundLibrary()
	{
		m_SoundBuffers.clear();
	}

	SoundLibrary::~SoundLibrary()
	{
		for (auto it = m_SoundBuffers.begin(); it != m_SoundBuffers.end(); ++it)
			alDeleteBuffers(1, &it->second);

		m_SoundBuffers.clear();
	}

	MusicBuffer::MusicBuffer(const std::string& filename) :
		m_Format(0)
	{
		alGenSources(1, &m_SourceId);
		alGenBuffers(NUM_BUFFERS, m_SoundBuffers);
		SetLooping(true);

		std::size_t frame_size;

		m_SoundFile = sf_open(filename.c_str(), SFM_READ, &m_SFinfo);
		ARC_CORE_ASSERT(m_SoundFile, "Could not open Music File: " + filename);

		if (m_SFinfo.channels == 1)
			m_Format = AL_FORMAT_MONO16;
		else if (m_SFinfo.channels == 2)
			m_Format = AL_FORMAT_STEREO16;
		else if (m_SFinfo.channels == 3)
		{
			if (sf_command(m_SoundFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				m_Format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (m_SFinfo.channels == 4)
		{
			if (sf_command(m_SoundFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				m_Format = AL_FORMAT_BFORMAT3D_16;
		}

		if (!m_Format)
		{
			sf_close(m_SoundFile);
			m_SoundFile = NULL;
			ARC_CORE_ASSERT(true, "Unsupported channel count from file!");
		}

		frame_size = ((size_t)BUFFER_SAMPLES * (size_t)m_SFinfo.channels) * sizeof(short);
		m_MemBuffer = static_cast<short*>(malloc(frame_size));
	}

	MusicBuffer::~MusicBuffer()
	{
		alDeleteSources(1, &m_SourceId);

		if (m_SoundFile)
			sf_close(m_SoundFile);

		m_SoundFile = nullptr;
		free(m_MemBuffer);

		alDeleteBuffers(NUM_BUFFERS, m_SoundBuffers);
	}

	void MusicBuffer::Play()
	{
		ALsizei i;
		alGetError();

		alSourceRewind(m_SourceId);
		alSourcei(m_SourceId, AL_BUFFER, 0);

		for (i = 0; i < NUM_BUFFERS; i++)
		{
			sf_count_t slen = sf_readf_short(m_SoundFile, m_MemBuffer, BUFFER_SAMPLES);
			if (slen < 1) break;

			slen *= m_SFinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(m_SoundBuffers[i], m_Format, m_MemBuffer, (ALsizei)slen, m_SFinfo.samplerate);
		}

		ARC_CORE_ASSERT(alGetError() == AL_NO_ERROR, "Error buffering for playback!");

		alSourceQueueBuffers(m_SourceId, i, m_SoundBuffers);
		alSourcePlay(m_SourceId);

		ARC_CORE_ASSERT(alGetError() == AL_NO_ERROR, "Error starting playback!");
	}

	void MusicBuffer::Stop()
	{

	}

	void MusicBuffer::Pause()
	{

	}

	void MusicBuffer::Resume()
	{

	}

	void MusicBuffer::UpdateBufferStream()
	{
		ALint processed, state;

		alGetError();

		alGetSourcei(m_SourceId, AL_SOURCE_STATE, &state);
		alGetSourcei(m_SourceId, AL_BUFFERS_PROCESSED, &processed);

		ARC_CORE_ASSERT(alGetError() == AL_NO_ERROR, "Error checking Music Source state!");

		while (processed > 0)
		{
			ALuint bufferId;
			sf_count_t slen;

			alSourceUnqueueBuffers(m_SourceId, 1, &bufferId);
			processed--;

			slen = sf_readf_short(m_SoundFile, m_MemBuffer, BUFFER_SAMPLES);
			alBufferData(bufferId, m_Format, m_MemBuffer, (ALsizei)slen, m_SFinfo.samplerate);
			alSourceQueueBuffers(m_SourceId, 1, &bufferId);
		}

		ARC_CORE_ASSERT(alGetError() == AL_NO_ERROR, "Error buffering Music data!");

		if (state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;

			alGetSourcei(m_SourceId, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0) return;

			alSourcePlay(m_SourceId);

			ARC_CORE_ASSERT(alGetError() == AL_NO_ERROR, "Error restarting music playback!");
		}
	}

	void MusicBuffer::SetLooping(const bool& loop)
	{
		alSourcei(m_SourceId, AL_LOOPING, (ALint)loop);
	}

	ALint MusicBuffer::GetSourceId()
	{
		return m_SourceId;
	}

	bool MusicBuffer::IsPlaying()
	{
		ALint playState;
		alGetSourcei(m_SourceId, AL_SOURCE_STATE, &playState);
		return (playState == AL_PLAYING);
	}
}