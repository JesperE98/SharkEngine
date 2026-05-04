#include "GoalTrigger.h"
#include "Core/GameObject.h"
#include "Physics/AABBComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"


namespace Shark::Components {

	void GoalTrigger::BeginPlay() {
		m_Player = nullptr;
		m_Triggered = false;
	}

	void GoalTrigger::Update(float deltaTime) {
		if (m_Triggered) return;

		// Lazy-find player
		if (!m_Player) {
			auto* scene = Core::SceneManager::Get().GetActiveScene();
			
			if (!scene) return;

			for (auto* obj : scene->GetGameObjects()) {
				if(obj && obj->GetName() == "Player"){
					m_Player = obj;
					break;
				}
			}

			if (!m_Player) return;
		}

		if (OverlapsPlayer()) {
			m_Triggered = true;
			SE_LOG(Engine, "Level Complete!");
			// TODO: load next level when SceneManager supports loading by file path

			if (!nextLevel.empty()) {
				Core::Message msg;
				msg.type = Core::EventType::LoadScene;
				msg.payload = nextLevel;
				Core::SceneManager::Get().inbox.Push(msg);
			}
		}
	}

	bool GoalTrigger::OverlapsPlayer() const {
		auto* myAABB = GetOwner()->GetComponent<AABBComponent>();
		auto* playerAABB = m_Player->GetComponent<AABBComponent>();

		if (!myAABB || !playerAABB) return false;

		const auto& a = GetOwner()->GetTransform().position + myAABB->m_Offset;
		const auto& b = m_Player->GetTransform().position + playerAABB->m_Offset;

		const auto& ae = myAABB->m_Extents;
		const auto& be = playerAABB->m_Extents;

		return std::abs(a.x - b.x) <= ( ae.x + be.x ) &&
			std::abs(a.y - b.y) <= ( ae.y + be.y ) &&
			std::abs(a.z - b.z) <= ( ae.z + be.z );
	}

}
