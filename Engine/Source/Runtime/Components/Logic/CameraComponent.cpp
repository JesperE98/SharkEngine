#include "CameraComponent.h"
#include "Math/Transform.h"
#include "Core/GameObject.h"

namespace Shark::Components {

	using Math::Transform;
	using Math::Vector3;

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
		return Perspective(FOV, AspectRatio, NearClip, FarClip);
	}

	glm::mat4 CameraComponent::GetViewMatrix() const {
		return m_Owner->GetTransform().GetViewMatrix();
	}
}

