#pragma once

#include "Arcane/Core/Base.h"
#include <chrono>

namespace Arcane
{
	class ARC_API Timestep
	{
	public:
		Timestep(float time) :
			m_Time(time) {}

		float GetSeconds() const { return m_Time; }
		float GetMillis() const { return m_Time * 1000.0f; }

		operator float() const { return m_Time; }
		Timestep& operator++(int other) { m_Time += other; return *this; }
		Timestep& operator--(int other) { m_Time -= other; return *this; }

		Timestep& operator+=(float other) { m_Time += other; return *this; }
		Timestep& operator+=(Timestep& other) { m_Time += other.m_Time; return *this; }

		Timestep& operator-=(float other) { m_Time -= other; return *this; }
		Timestep& operator-=(Timestep& other) { m_Time -= other.m_Time; return *this; }

		Timestep& operator*=(float other) { m_Time *= other; return *this; }
		Timestep& operator*=(Timestep& other) { m_Time *= other.m_Time; return *this; }

	private:
		float m_Time;

	};
}