#include "InputManager.h"

namespace Shark::Input {

	InputManager& InputManager::Get()
	{
		static InputManager instance;
		return instance;
	}
	void InputManager::OnInitialize(GLFWwindow* window)
	{
		m_Window = window;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}

	void InputManager::Update(float deltaTime) {
		double xPos, yPos;

		glfwGetCursorPos(m_Window, &xPos, &yPos);

		if (m_FirstMouse) {
			m_LastX = xPos;
			m_LastY = yPos;
			m_FirstMouse = false;
		}

		m_MouseDeltaX = static_cast<float>(xPos - m_LastX);
		m_MouseDeltaY = static_cast<float>(m_LastY - yPos); // Reversed

		m_LastX = xPos;
		m_LastY = yPos;
	}

	void InputManager::GetMouseDelta(float& xOffset, float& yOffset)
	{
		xOffset = m_MouseDeltaX;
		yOffset = m_MouseDeltaY;
	}

	bool InputManager::IsKeyPressed(int key)
	{
		return glfwGetKey(m_Window, key) == GLFW_PRESS;
	}

	bool InputManager::IsMouseButtonPressed(int key)
	{
		return glfwGetMouseButton(m_Window, key) == GLFW_PRESS;
	}

}
