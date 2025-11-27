#include "InputManager.h"

GLFWwindow* InputManager::m_Window{ nullptr };
double InputManager::m_LastX = 0.0f;
double InputManager::m_LastY = 0.0f;
float InputManager::m_MouseDeltaX = 0.0f;
float InputManager::m_MouseDeltaY = 0.0f;
bool InputManager::m_FirstMouse = true;

void InputManager::Init(GLFWwindow* window)
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
