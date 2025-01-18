#pragma once

#include "Arcane/Core/Base.h"

#include <glm/glm.hpp>
#include <AL/al.h>

namespace Arcane
{

	class ARC_API SoundSource
	{
	public:
		SoundSource(const glm::vec3& position = glm::vec3(0), const glm::vec3& velocity = glm::vec3(0), float pitch = 1.0f, float gain = 1.0f, bool loop = false);
		~SoundSource();

		void Play(const ALuint buffer);

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Velocity;
		float m_Pitch, m_Gain;
		bool m_Loop;

		ALuint m_SourceId;
		ALuint m_BufferId;
	};
}