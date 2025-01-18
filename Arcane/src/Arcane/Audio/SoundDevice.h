#pragma once

#include "Arcane/Core/Base.h"

#include <AL/alc.h>

namespace Arcane
{
	class ARC_API SoundDevice
	{
	public:
		static SoundDevice* Get();

	private:
		SoundDevice();
		~SoundDevice();

	private:
		ALCdevice* m_ALCdevice;
		ALCcontext* m_ALCcontext;
	};
}