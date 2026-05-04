#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "SharkBehavior.h"
#include "Math/Vector3.h"

namespace Shark::Components {

	class RigidbodyComponent;

	class PlayerController : public SharkBehavior {
	public:
#pragma region TUNABLE STATS

		float moveSpeed		= 8.0f;
		float jumpForce		= 10.0f;
		float dashForce		= 20.0f;
		float dashCooldown	= 1.0f;
		float dashDuration	= 0.15f;
		float killY			= -20.0f;

#pragma endregion

		void BeginPlay() override final;
		void Update(float deltaTime) override final;

	private:
		RigidbodyComponent* m_RigidbodyComp = nullptr;

		float m_DashTimer		= 0.0f;
		float m_DashActiveTimer = 0.0f;
		bool m_JumpPressedLast	= false;
		bool m_DashPressedLast	= false;

		Math::Vector3 m_SpawnPoint = { 0.0f, 0.0f, 0.0f };

		bool m_SpawnPointSet = false;
	};
}

#endif // PLAYER_CONTROLLER_H
