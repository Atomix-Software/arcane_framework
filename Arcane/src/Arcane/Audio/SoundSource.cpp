#include <arcpch.h>

#include "Arcane/Audio/SoundSource.h"

namespace Arcane
{
	SoundSource::SoundSource(const glm::vec3& position, const glm::vec3& velocity, float pitch, float gain, bool loop) :
		m_Position(position), m_Velocity(velocity), m_Pitch(pitch), m_Gain(gain), m_Loop(loop)
	{
		alGenSources(1, &m_SourceId);
		alSourcef(m_SourceId, AL_PITCH, m_Pitch);
		alSourcef(m_SourceId, AL_GAIN, m_Gain);
		alSource3f(m_SourceId, AL_POSITION, m_Position.x, m_Position.y, m_Position.z);
		alSource3f(m_SourceId, AL_VELOCITY, m_Velocity.x, m_Velocity.y, m_Velocity.z);
		alSourcei(m_SourceId, AL_LOOPING, m_Loop);
		alSourcei(m_SourceId, AL_BUFFER, m_BufferId);
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

		ALint state = AL_PLAYING;
		while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{
			alGetSourcei(m_SourceId, AL_SOURCE_STATE, &state);
		}
	}
}
