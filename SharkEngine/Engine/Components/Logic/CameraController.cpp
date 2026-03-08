#include "CameraController.h"
#include "Managers/InputManager.h"
#include "Core/GameObject.h"

#include <Source/Managers/WindowManager.h>

namespace Shark::Components {

	using Shark::Managers::InputManager;
	using Shark::Core::GameObject;
	using Shark::Math::Transform;
	using Shark::Math::Vector3;
	using Shark::Editor::WindowManager;

	CameraController::CameraController(float moveSpeed, float sensitivity) 
		: m_Speed(moveSpeed), m_MouseSensitivity(sensitivity)
	{
		SE_LOG(Rendering, "CameraController::CameraController() - Initializing Camera controller with move speed {} and mouse sensitivity {}.", m_Speed, m_MouseSensitivity);
	}

	void CameraController::Update(float deltaTime) {

		if (!m_WindowManager->IsSceneViewportFocused() || !m_WindowManager->IsSceneViewportFocused()) return;

		InputManager& inputManager = InputManager::Get();
		Transform& transform = GetOwner()->GetTransform();
		float velocity = m_Speed * deltaTime;

		if (inputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			glfwSetInputMode(inputManager.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// Camera Movement logic
			if (inputManager.IsKeyPressed(GLFW_KEY_W)) {
				transform.Translate(transform.GetForward() * velocity);
			}
			if (inputManager.IsKeyPressed(GLFW_KEY_A)) {
				transform.Translate(transform.GetRight() * -velocity);
			}
			if (inputManager.IsKeyPressed(GLFW_KEY_S)) {
				transform.Translate(transform.GetForward() * -velocity);
			}
			if (inputManager.IsKeyPressed(GLFW_KEY_D)) {
				transform.Translate(transform.GetRight() * velocity);
			}


			// --- Rotation (FPS Style) ---
			float xOffset, yOffset;
			inputManager.GetMouseDelta(xOffset, yOffset);

			// Update the internal Euler trackers
			m_Yaw -= xOffset * m_MouseSensitivity;
			m_Pitch += yOffset * m_MouseSensitivity;

			// Clamp pitch to prevent flipping upside down
			m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

			// Apply total rotation to the transform
			transform.rotation = Math::FromEulerDegrees(Vector3(m_Pitch, m_Yaw, 0.0f));
		}
		else {
			glfwSetInputMode(inputManager.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}
