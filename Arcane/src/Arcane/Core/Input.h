#pragma once

#include "Arcane/Core/Base.h"
#include "Arcane/Core/KeyCodes.h"
#include "Arcane/Core/MouseCodes.h"

namespace Arcane
{
	class ARC_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();

	};
}