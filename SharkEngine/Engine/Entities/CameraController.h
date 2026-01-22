#pragma once

#include "Camera.h"

namespace Shark::Entities {

	class CameraController
	{
	public:
		CameraController(Camera* cam, float moveSpeed = 5.0f, float sensitivity = 0.1f);

		void Update(float deltaTime);

	private:

		Camera* m_Camera{ nullptr };
		float m_Speed = 0.0f;
		float m_MouseSensitivity = 0.0f;
	};


}
