#include "CameraController.h"
#include "InputManager.h"
#include "Core/Utilities/Time.h"

#include <iostream>

CameraController::CameraController(Camera* cam, float moveSpeed, float sensitivity)
	: m_Camera(cam), m_Speed(moveSpeed), m_MouseSensitivity(sensitivity)
{
	std::cout << Time::CreateTimeStamp() << ": " << "Created Camera controller and assigned " << m_Camera << " to the controller.\n";
}

void CameraController::Update(float deltaTime) {
	InputManager::Update(deltaTime);

	float velocity = m_Speed * deltaTime;

	if (InputManager::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {

		glfwSetInputMode(InputManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (InputManager::IsKeyPressed(GLFW_KEY_W)) {
			//std::cout << "Moving Camera Up. Cameras position = " << m_Camera->transform.position.y << ".\n";
			m_Camera->GetTransform().position += m_Camera->GetFront() * velocity;
		}
		if (InputManager::IsKeyPressed(GLFW_KEY_A)) {
			//std::cout << "Moving Camera Left. Cameras position = " << m_Camera->transform.position.x << ".\n";
			m_Camera->GetTransform().position += m_Camera->GetRight() * velocity;
		}
		if (InputManager::IsKeyPressed(GLFW_KEY_S)) {
			//std::cout << "Moving Camera Down. Cameras position = " << m_Camera->transform.position.y << ".\n";
			m_Camera->GetTransform().position -= m_Camera->GetFront() * velocity;
		}
		if (InputManager::IsKeyPressed(GLFW_KEY_D)) {
			//std::cout << "Moving Camera Right. Cameras position = " << m_Camera->transform.position.x << ".\n";
			m_Camera->GetTransform().position -= m_Camera->GetRight() * velocity;
		}

		float xOffset, yOffset;
		InputManager::GetMouseDelta(xOffset, yOffset);
		m_Camera->Rotate(-xOffset * m_MouseSensitivity, -yOffset * m_MouseSensitivity);
	}
	else {
		glfwSetInputMode(InputManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}


}