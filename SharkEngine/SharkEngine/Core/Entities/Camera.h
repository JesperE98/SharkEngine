#pragma once

#include "GameObject.h"

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

	Vector3 GetFront() const { return m_Front; }
	Vector3 GetRight() const { return m_Right; }
	Vector3 GetUp() const { return m_Up; }


private:
#pragma region Member Variables
	float m_FOV = 0.f;
	float m_AspectRatio = 0.f;
	float m_NearClip = 0.f;
	float m_FarClip = 0.f;

	float m_Yaw = 0.f;
	float m_Pitch = 0.f;

	glm::mat4 m_ProjectionMatrix;

	Vector3 m_Front;
	Vector3 m_Right;
	Vector3 m_Up;
	Vector3 m_WorldUp = Vector3(0.0f, 1.0f, 0.0f);

	void UpdateVectors();

#pragma endregion
};

