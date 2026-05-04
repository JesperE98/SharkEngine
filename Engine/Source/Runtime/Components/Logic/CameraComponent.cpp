#include "CameraComponent.h"
#include "Math/Transform.h"
#include "Core/GameObject.h"
#include <Core/App/EditorStateManager.h>
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>

namespace Shark::Components {

	using Math::Transform;
	using Math::Vector3;
	using Core::EditorStateManager;
	using Core::SceneManager;
	using Shark::Scene;

	void CameraComponent::SetAspectRatio(float aspect)
	{
		AspectRatio = aspect;
		UpdateProjectionMatrix();
	}

	void CameraComponent::SetFOV(float fov)
	{
		FOV = fov;
		UpdateProjectionMatrix();
	}

	void CameraComponent::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = Perspective(FOV, AspectRatio, NearClip, FarClip);
	}

	glm::mat4 CameraComponent::GetProjectionMatrix() const {
		return m_ProjectionMatrix;
	}

	glm::mat4 CameraComponent::GetViewMatrix() const {
		return m_Owner->GetTransform().GetViewMatrix();
	}
	void CameraComponent::Update(float deltaTime) {
		const bool isPlaying = EditorStateManager::Get().GetState() == EditorState::Play;
		mode = isPlaying ? Mode::ThirdPerson : Mode::Free;


		// Free mode: do nothing - CameraController handles editor flying
		if (mode == Mode::Free) {
			return;
		}

		// Auto-find the Player GameObject if no target set yet
		TryAutoFindPlayer();
		if (!followTarget) return;

		auto& selfTransform			= m_Owner->GetTransform();
		const auto& targetTransform = followTarget->GetTransform();

		Vector3  desiredPos = targetTransform.position + followOffset;

		// Smooth lerp toward desired
		float t = std::min(1.0f, followSpeed * deltaTime);
		selfTransform.position = Math::Lerp(selfTransform.position, desiredPos, t);
		
		// Look at the target
		selfTransform.LookAt(targetTransform.position);
	}

	void CameraComponent::TryAutoFindPlayer() {
		if (followTarget) return;

		auto* scene = SceneManager::Get().GetActiveScene();
		if (!scene) return;

		for (auto* obj : scene->GetGameObjects()) {
			if (obj && obj->GetName() == "Player") {
				followTarget = obj;
				SE_LOG(Engine, "ThirdPersonCamera auto-attached to '{}'", obj->GetName());
				break;
			}
		}
	}
}

