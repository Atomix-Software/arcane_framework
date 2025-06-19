#include <arcpch.h>

#include "Arcane/Core/Application.h"
#include "Arcane/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Arcane
{
	std::unordered_map<KeyCode, bool> Input::m_PreviousKeyState;
	std::unordered_map<KeyCode, bool> Input::m_PreviousMouseState;

	std::unordered_map<KeyCode, bool> Input::m_JustPressedKeys;
	std::unordered_map<KeyCode, bool> Input::m_JustPressedMouseButtons;

	void Input::Update()
	{
		if (m_PreviousKeyState.empty() || m_PreviousMouseState.empty())
		{
			for (uint32_t k = Key::Space; k < Key::KeyLast; ++k)
				m_PreviousKeyState[k] = false;

			for (uint32_t b = Mouse::Button0; b < Mouse::ButtonLast; ++b)
				m_PreviousMouseState[b] = false;
		}

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		for (auto& [key, prevState] : m_PreviousKeyState)
		{
			bool currentState = glfwGetKey(window, key) == GLFW_PRESS;
			m_JustPressedKeys[key] = currentState && !prevState;
			prevState = currentState;
		}

		for (auto& [button, prevState] : m_PreviousMouseState)
		{
			bool currentState = glfwGetMouseButton(window, button) == GLFW_PRESS;
			m_JustPressedMouseButtons[button] = currentState && !prevState;
			prevState = currentState;
		}
	}

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		bool current = state == GLFW_PRESS;

		if (m_PreviousKeyState.find(keycode) == m_PreviousKeyState.end())
			m_PreviousKeyState[keycode] = current;

		return current;
	}

	bool Input::KeyJustPressed(KeyCode keycode)
	{
		if (m_JustPressedKeys.find(keycode) == m_JustPressedKeys.end())
			return false;

		return m_JustPressedKeys[keycode];
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		bool current = state == GLFW_PRESS;

		if (m_PreviousMouseState.find(button) == m_PreviousMouseState.end())
			m_PreviousMouseState[button] = current;

		return current;
	}

	bool Input::MouseButtonJustPressed(MouseCode button)
	{
		if (m_JustPressedMouseButtons.find(button) == m_JustPressedMouseButtons.end())
			return false;

		return m_JustPressedMouseButtons[button];
	}

	std::pair<float, float> Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		return { (float) mouseX, (float) mouseY };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePos();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePos();
		return y;
	}
}