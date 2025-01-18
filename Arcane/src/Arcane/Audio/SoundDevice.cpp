#include <arcpch.h>

#include "Arcane/Audio/SoundDevice.h"

#include <AL/al.h>

namespace Arcane
{
	SoundDevice* SoundDevice::Get()
	{
		static SoundDevice* snd_device = new SoundDevice();
		return snd_device;
	}

	SoundDevice::SoundDevice()
	{
		m_ALCdevice = alcOpenDevice(nullptr);
		ARC_CORE_ASSERT(m_ALCdevice, "Failed to get Sound Device!");

		m_ALCcontext = alcCreateContext(m_ALCdevice, nullptr);
		ARC_CORE_ASSERT(m_ALCcontext, "Failed to set Sound Context!");
		ARC_CORE_ASSERT(alcMakeContextCurrent(m_ALCcontext), "Failed to make Context Current!");

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(m_ALCdevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(m_ALCdevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(m_ALCdevice) != AL_NO_ERROR)
			name = alcGetString(m_ALCdevice, ALC_DEVICE_SPECIFIER);

		ARC_CORE_TRACE("Opened {0}", name);
	}

	SoundDevice::~SoundDevice()
	{
		ARC_CORE_ASSERT(alcMakeContextCurrent(nullptr), "Failed to set Context to nullptr");
		alcDestroyContext(m_ALCcontext);
		ARC_CORE_ASSERT(!m_ALCcontext, "Failed to unset Context during close!");
		ARC_CORE_ASSERT(alcCloseDevice(m_ALCdevice), "Failed to close Sound Device!");
	}
}