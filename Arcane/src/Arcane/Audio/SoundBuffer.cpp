#include <arcpch.h>

#include "SoundBuffer.h"

#include <sndfile.h>
#include <inttypes.h>

#include <AL/alext.h>

namespace Arcane
{
	SoundBuffer* Arcane::SoundBuffer::Get()
	{
		static SoundBuffer* snd_buffer = new SoundBuffer();
		return snd_buffer;
	}

	ALuint SoundBuffer::AddSoundEffect(const char* filename)
	{
		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		sndfile = sf_open(filename, SFM_READ, &sfinfo);
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

		m_SoundBuffers.push_back(buffer);
		return buffer;
	}

	bool SoundBuffer::RemoveSoundEffect(const ALuint& buffer)
	{
		auto it = m_SoundBuffers.begin();
		while (it != m_SoundBuffers.end())
		{
			if (*it == buffer)
			{
				alDeleteBuffers(1, &*it);
				it = m_SoundBuffers.erase(it);

				return true;
			}
			else
			{
				++it;
			}
		}

		return false;
	}

	SoundBuffer::SoundBuffer()
	{
		m_SoundBuffers.clear();
	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers((ALsizei) m_SoundBuffers.size(), m_SoundBuffers.data());
		m_SoundBuffers.clear();
	}
}