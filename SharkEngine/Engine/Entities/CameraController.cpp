#include "CameraController.h"
#include "Managers/InputManager.h"
#include "Core/Utilities/Time.h"
#include "Core/Utilities/Debug.h"

namespace Shark::Entities {

	using Shark::Managers::InputManager;

	CameraController::CameraController(Camera* cam, float moveSpeed, float sensitivity)
		: m_Camera(cam), m_Speed(moveSpeed), m_MouseSensitivity(sensitivity)
	{
		SE_LOG(Rendering, "CameraController::CameraController() - Created Camera controller and assigned {} to the controller.", m_Camera->GetName());
	}

	void CameraController::Update(float deltaTime) {

		InputManager& inputManager = InputManager::Get();
		float velocity = m_Speed * deltaTime;

		if (inputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {

			glfwSetInputMode(inputManager.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (inputManager.IsKeyPressed(GLFW_KEY_W)) {
				m_Camera->GetTransform().position += m_Camera->GetFront() * velocity;
			}
			if (inputManager.IsKeyPressed(GLFW_KEY_A)) {
				m_Camera->GetTransform().position += m_Camera->GetRight() * velocity;
			}
			if (inputManager.IsKeyPressed(GLFW_KEY_S)) {
				m_Camera->GetTransform().position -= m_Camera->GetFront() * velocity;
			}
			if (inputManager.IsKeyPressed(GLFW_KEY_D)) {
				m_Camera->GetTransform().position -= m_Camera->GetRight() * velocity;
			}

			float xOffset, yOffset;
			inputManager.GetMouseDelta(xOffset, yOffset);
			m_Camera->Rotate(-xOffset * m_MouseSensitivity, -yOffset * m_MouseSensitivity);
		}
		else {
			glfwSetInputMode(inputManager.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}
