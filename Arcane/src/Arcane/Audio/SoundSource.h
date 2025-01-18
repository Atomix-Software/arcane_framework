#pragma once

#include "Arcane/Core/Base.h"

#include <glm/glm.hpp>
#include <AL/al.h>

namespace Arcane
{

	class ARC_API SoundSource
	{
	public:
		SoundSource();
		~SoundSource();

		void Play(const ALuint buffer);
		void Stop();

		void Pause();
		void Resume();
		bool IsPlaying();

		void SetPosition(const float& x, const float& y, const float& z);
		void SetSound(const ALuint& buffer);
		void SetLooping(const bool& loop);

	private:
		ALuint m_SourceId;
		ALuint m_BufferId;

	};
}