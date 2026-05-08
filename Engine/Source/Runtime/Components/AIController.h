#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H

#include "SharkBehavior.h"
#include "Math/Vector3.h"

namespace Shark::Components {
	class AIController : public SharkBehavior {
	public:
		// Serializable fiels
		float PatrolSpeed = 3.0f;
		float PatrolDistance = 10.0f;

		AIController() {
			tickMode = TickMode::PlayOnly;
		}
		~AIController();

		void BeginPlay() override final;
		void Update(float deltaTime) override final;

		void OnPlayerHit();

	private:
		Math::Vector3 m_StartPos{};
		Math::Vector3 m_EndPos{};
		
		bool m_MovingToEnd = true;
		bool m_Initialized = false;

	};
}

#endif // AI_CONTROLLER_H