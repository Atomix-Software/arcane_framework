#include <arcpch.h>

#include "Arcane/Audio/SoundDevice.h"

#include <AL/al.h>

namespace Arcane
{
	Unique<SoundDevice> SoundDevice::Create()
	{
		return CreateUnique<SoundDevice>();
	}

	SoundDevice::SoundDevice()
	{
		m_ALCdevice = alcOpenDevice(nullptr);
		ARC_CORE_ASSERT(m_ALCdevice, "Failed to get Sound Device!");

		m_ALCcontext = alcCreateContext(m_ALCdevice, nullptr);
		ARC_CORE_ASSERT(m_ALCcontext, "Failed to set Sound Context!");
		alcMakeContextCurrent(m_ALCcontext);

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(m_ALCdevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(m_ALCdevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(m_ALCdevice) != AL_NO_ERROR)
			name = alcGetString(m_ALCdevice, ALC_DEVICE_SPECIFIER);
	}

	SoundDevice::~SoundDevice()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_ALCcontext); 
		alcCloseDevice(m_ALCdevice);
	}

	void SoundDevice::GetLocation(float& x, float& y, float& z)
	{
		alGetListener3f(AL_POSITION, &x, &y, &z);
	}

	void SoundDevice::GetOrientation(float& orientation)
	{
		alGetListenerfv(AL_ORIENTATION, &orientation);
	}

	float SoundDevice::GetGain()
	{
		float curr_gain;
		alGetListenerf(AL_GAIN, &curr_gain);
		return curr_gain;
	}

	void SoundDevice::SetAttunation(int attunation)
	{
		ARC_CORE_ASSERT(!(attunation < 0xD001 || attunation > 0xD006), "bad attunation key");
		alListeneri(AL_DISTANCE_MODEL, attunation);
	}

	void SoundDevice::SetLocation(const float& x, const float& y, const float& z)
	{
		alListener3f(AL_POSITION, x, y, z);
	}

	void SoundDevice::SetOrientation(const float& atX, const float& atY, const float& atZ, const float& upX, const float& upY, const float& upZ)
	{
		std::vector<float> ori;
		ori.push_back(atX);
		ori.push_back(atY);
		ori.push_back(atZ);
		ori.push_back(upX);
		ori.push_back(upY);
		ori.push_back(upZ);
		alListenerfv(AL_ORIENTATION, ori.data());
	}

	void SoundDevice::SetGain(const float& val)
	{
		float newVol = val;

		if (newVol < 0.f) newVol = 0.f;
		else if (newVol > 5.f) newVol = 5.f;

		alListenerf(AL_GAIN, newVol);
	}
}