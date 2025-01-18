#pragma once

#include "Arcane/Core/Base.h"

#include <AL/alc.h>

namespace Arcane
{
	class ARC_API SoundDevice
	{

	public:
		SoundDevice();
		~SoundDevice();

		void GetLocation(float& x, float& y, float& z);
		void GetOrientation(float& orientation);
		float GetGain();

		void SetAttunation(int attunation);
		void SetLocation(const float& x, const float& y, const float& z);
		void SetOrientation(
			const float& atX, const float& atY, const float& atZ,
			const float& upX, const float& upY, const float& upZ);
		void SetGain(const float& val);

	public:
		static Unique<SoundDevice> Create();

	private:
		ALCdevice* m_ALCdevice;
		ALCcontext* m_ALCcontext;

	};
}