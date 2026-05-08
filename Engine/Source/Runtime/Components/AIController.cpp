#include "AIController.h"
#include "Core/GameObject.h"
#include "Core/Utilities/Debug.h"
#include "Physics/AABBComponent.h"


namespace Shark::Components {
	void AIController::BeginPlay() {
	
	}

	void AIController::Update(float deltaTime) {
		if (!m_Initialized) {
			m_StartPos = GetOwner()->GetTransform().position;
			m_EndPos = m_StartPos;
			m_EndPos.x += PatrolDistance;
			m_Initialized = true;
		}

		auto& pos = GetOwner()->GetTransform().position;

		// Move toward target
		Math::Vector3 target = m_MovingToEnd ? m_EndPos : m_StartPos;
		float dx = target.x - pos.x;
		float dz = target.z - pos.z;
		float dist = std::sqrt(dx * dx + dz * dz);

		if (dist < 0.3f) {
			m_MovingToEnd = !m_MovingToEnd;
		} else {
			float nx = dx / dist;
			float nz = dz / dist;
			pos.x += nx * PatrolSpeed * deltaTime;
			pos.z += nz * PatrolSpeed * deltaTime;
		}
	}

	void AIController::OnPlayerHit() {
		SE_LOG(Engine, "Enemy: I hit the player!");
	}

	AIController::~AIController() {}
}