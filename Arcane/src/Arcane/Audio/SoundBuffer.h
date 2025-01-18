#pragma once

#include "Arcane/Core/Base.h"

#include <AL/al.h>

namespace Arcane
{
	class ARC_API SoundBuffer
	{
	public:
		static SoundBuffer* Get();

		ALuint AddSoundEffect(const char* filename);
		bool RemoveSoundEffect(const ALuint& buffer);

	private:
		SoundBuffer();
		~SoundBuffer();

	private:
		std::vector<ALuint> m_SoundBuffers;

	};
}