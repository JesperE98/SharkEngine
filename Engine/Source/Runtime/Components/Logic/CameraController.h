#pragma once

#include "Components/SharkBehavior.h"

namespace Shark::Components {

	class CameraController : virtual public SharkBehavior
	{
	public:
		explicit CameraController(float moveSpeed = 5.0f, float sensitivity = 0.1f);

		virtual void Update(float deltaTime) override;

	private:

		float m_Yaw = 0.f;
		float m_Pitch = 0.f;
		float m_Speed;
		float m_MouseSensitivity;
	};


}
