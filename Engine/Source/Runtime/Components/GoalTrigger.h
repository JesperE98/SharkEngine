#ifndef GOAL_TRIGGER_H
#define GOAL_TRIGGER_H

#include "SharkBehavior.h"
#include <string>

namespace Shark::Components{

	class GoalTrigger : public SharkBehavior {
	public:
		GoalTrigger() {
			tickMode = TickMode::PlayOnly;
		}

		// Tunable
		std::string nextLevel = ""; // empty = just log "complete" for now

		void BeginPlay() override final;
		void Update(float deltaTime) override final;

	private:
		Core::GameObject* m_Player = nullptr;
		bool m_Triggered = false;

		bool OverlapsPlayer() const;
	};
}


#endif // GOAL_TRIGGER_H