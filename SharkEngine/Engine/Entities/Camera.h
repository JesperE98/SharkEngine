#ifndef ENGINE_ENTITIES_CAMERA_H
#define ENGINE_ENTITIES_CAMERA_H

#include "GameObject.h"

#include <glm/ext/matrix_float4x4.hpp>

namespace Shark::Math { struct Vector3; }

namespace Shark::Entities {

	class Camera : public GameObject
	{
	public:
		Camera(std::string name, float fov, float aspect, float nearPlane, float farPlane);

		void Update(float deltaTime) override;

		void Rotate(float yawDelta, float pitchDelta);
		void SetAspectRatio(float aspect);
		void UpdateProjectionMatrix();

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

		Shark::Math::Vector3 GetFront() const { return m_Front; }
		Shark::Math::Vector3 GetRight() const { return m_Right; }
		Shark::Math::Vector3 GetUp() const { return m_Up; }


	private:
#pragma region Member Variables
		float m_FOV = 0.f;
		float m_AspectRatio = 0.f;
		float m_NearClip = 0.f;
		float m_FarClip = 0.f;

		float m_Yaw = 0.f;
		float m_Pitch = 0.f;

		glm::mat4 m_ProjectionMatrix;

		Shark::Math::Vector3 m_Front;
		Shark::Math::Vector3 m_Right;
		Shark::Math::Vector3 m_Up;
		Shark::Math::Vector3 m_WorldUp = Shark::Math::Vector3(0.0f, 1.0f, 0.0f);

		void UpdateVectors();

#pragma endregion
	};
}

#endif // !ENGINE_ENTITIES_CAMERA_H