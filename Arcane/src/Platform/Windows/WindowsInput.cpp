#include <arcpch.h>

#include "Arcane/Core/Application.h"
#include "Arcane/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Arcane
{
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
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