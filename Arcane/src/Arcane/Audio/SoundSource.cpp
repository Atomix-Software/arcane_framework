#include <arcpch.h>

#include "Arcane/Audio/SoundSource.h"

namespace Arcane
{
	SoundSource::SoundSource()
	{
		alGenSources(1, &m_SourceId);
		alSourcef(m_SourceId, AL_PITCH, 1.0f);
		alSourcef(m_SourceId, AL_GAIN, 1.0f);
		alSource3f(m_SourceId, AL_POSITION, 0, 0, 0);
		alSource3f(m_SourceId, AL_VELOCITY, 0, 0, 0);
		alSourcei(m_SourceId, AL_LOOPING, false);
		alSourcei(m_SourceId, AL_BUFFER, (ALint) m_BufferId);
	}

	SoundSource::~SoundSource()
	{
		alDeleteSources(1, &m_SourceId);
	}

	void SoundSource::Play(const ALuint buffer)
	{
		if (buffer != m_BufferId)
		{
			m_BufferId = buffer;
			alSourcei(m_SourceId, AL_BUFFER, (ALint) m_BufferId);
		}

		alSourcePlay(m_SourceId);
	}

	void SoundSource::Stop()
	{
		alSourceStop(m_SourceId);
	}

	void SoundSource::Pause()
	{
		alSourcePause(m_SourceId);
	}

	void SoundSource::Resume()
	{
		alSourcePlay(m_SourceId);
	}

	bool SoundSource::IsPlaying()
	{
		ALint playState;
		alGetSourcei(m_SourceId, AL_SOURCE_STATE, &playState);
		return (playState == AL_PLAYING);
	}

	void SoundSource::SetPosition(const float& x, const float& y, const float& z)
	{
		alSource3f(m_SourceId, AL_POSITION, x, y, z);
	}

	void SoundSource::SetSound(const ALuint& buffer)
	{
		if (buffer == m_BufferId) return;

		alSourcei(m_SourceId, AL_BUFFER, (ALint) m_BufferId);
	}

	void SoundSource::SetLooping(const bool& loop)
	{
		alSourcei(m_SourceId, AL_LOOPING, (ALint)loop);
	}
}
