#pragma once

#include "Components/SharkBehavior.h"

namespace Shark::Components {

	class CameraController : public SharkBehavior
	{
	public:
		explicit CameraController(float moveSpeed = 5.0f, float sensitivity = 0.1f);

		void Update(float deltaTime) override;

		void SetMoveSpeed(float value) { m_Speed = value; }
		void SetMouseSensitivity(float value) { m_MouseSensitivity = value; }

		float GetMoveSpeed() const			{ return m_Speed; }
		float GetMouseSensitivity() const	{ return m_MouseSensitivity; }
		float GetYaw() const				{ return m_Yaw; }
		float GetPitch() const				{ return m_Pitch; }
	private:

		float m_Yaw = 0.f;
		float m_Pitch = 0.f;
		float m_Speed;
		float m_MouseSensitivity;
	};


}
