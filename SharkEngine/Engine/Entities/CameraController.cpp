#include "CameraController.h"
#include "Managers/InputManager.h"
#include "Core/Utilities/Time.h"
#include "Core/Utilities/Debug.h"

namespace Shark::Entities {

	using Shark::Managers::InputManager;

	CameraController::CameraController(Camera* cam, float moveSpeed, float sensitivity)
		: m_Camera(cam), m_Speed(moveSpeed), m_MouseSensitivity(sensitivity)
	{
		SHARK_LOG(Rendering, "CameraController::CameraController() - Created Camera controller and assigned {} to the controller.", m_Camera->GetName());
	}

	void CameraController::Update(float deltaTime) {
		InputManager::Update(deltaTime);

		float velocity = m_Speed * deltaTime;

		if (InputManager::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {

			glfwSetInputMode(InputManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (InputManager::IsKeyPressed(GLFW_KEY_W)) {
				m_Camera->GetTransform().position += m_Camera->GetFront() * velocity;
			}
			if (InputManager::IsKeyPressed(GLFW_KEY_A)) {
				m_Camera->GetTransform().position += m_Camera->GetRight() * velocity;
			}
			if (InputManager::IsKeyPressed(GLFW_KEY_S)) {
				m_Camera->GetTransform().position -= m_Camera->GetFront() * velocity;
			}
			if (InputManager::IsKeyPressed(GLFW_KEY_D)) {
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
}
