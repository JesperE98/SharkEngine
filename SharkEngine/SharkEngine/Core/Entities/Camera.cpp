#include "Camera.h"
#include <Core/Math/MathUtils.h>
#include <Core/Utilities/Time.h>

Camera::Camera(std::string name, float fov, float aspect, float nearPlane, float farPlane)
	: GameObject(name), m_FOV(fov), m_AspectRatio(aspect), m_NearClip(nearPlane), m_FarClip(farPlane), m_Yaw(0.f), m_Pitch(0.0f)
{
	std::cout << Time::CreateTimeStamp() << ": " << "Created Camera.\n";
	transform.position = Vector3(0.0f, 0.0f, -5.0f);
	transform.rotation = Quaternion();
	m_ProjectionMatrix = GetProjectionMatrix();
	UpdateVectors();
}

void Camera::Update(float deltaTime)
{
}

void Camera::Rotate(float yawDelta, float pitchDelta)
{
	m_Yaw += yawDelta;
	m_Pitch += pitchDelta;
	m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

	//Quaternion qPitch = Math::FromEulerDegrees(Vector3(m_Pitch, 0.f, 0.f));
	//Quaternion qYaw = Math::FromEulerDegrees(Vector3(0.f, m_Yaw, 0.f));

	//transform.rotation = qYaw * qPitch;

	transform.rotation = Math::FromEulerDegrees(Vector3(m_Pitch, m_Yaw, 0.0f));

	UpdateVectors();
}

void Camera::SetAspectRatio(float aspect)
{
	m_AspectRatio = aspect;
}

void Camera::UpdateProjectionMatrix()
{
	m_ProjectionMatrix = transform.Perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return transform.LookAt(transform.position, transform.position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return transform.Perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void Camera::UpdateVectors()
{
	// OLD IMPLEMENTATION USING YAW AND PITCH
	//glm::vec3 front;
	//front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	//front.y = sin(glm::radians(m_Pitch));
	//front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	//m_Front = glm::normalize(front);

	//m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	//m_Up = glm::normalize(glm::cross(m_Right, m_Front));

	m_Front = Math::RotateVector(transform.rotation, Vector3(0, 0, 1));
	m_Right = Math::RotateVector(transform.rotation, Vector3(1, 0, 0));
	m_Up = Math::RotateVector(transform.rotation, Vector3(0, 1, 0));

	// Normalize
	m_Front = Math::Normalize(m_Front);
	m_Right = Math::Normalize(m_Right);
	m_Up = Math::Normalize(m_Up);
}
