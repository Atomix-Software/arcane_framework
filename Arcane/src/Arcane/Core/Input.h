#pragma once

#include "Arcane/Core/Base.h"
#include "Arcane/Core/KeyCodes.h"
#include "Arcane/Core/MouseCodes.h"

namespace Arcane
{
	class ARC_API Input
	{
	public:
		static void Update();

		static bool IsKeyPressed(KeyCode keycode);
		static bool KeyJustPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static bool MouseButtonJustPressed(MouseCode button);

		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();

	private:
		static std::unordered_map<KeyCode, bool> m_PreviousKeyState;
		static std::unordered_map<KeyCode, bool> m_PreviousMouseState;

		static std::unordered_map<KeyCode, bool> m_JustPressedKeys;
		static std::unordered_map<KeyCode, bool> m_JustPressedMouseButtons;

	};
}