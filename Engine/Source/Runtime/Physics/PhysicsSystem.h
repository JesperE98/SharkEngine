#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <vector>

namespace Shark					{ class Scene; }
namespace Shark::Core			{ class GameObject; }
namespace Shark::Components		{ class RigidbodyComponent; class AABBComponent; }

namespace Shark::Physics {

	class PhysicsSystem {
	public:
		static PhysicsSystem& Get() {
			static PhysicsSystem instance;
			return instance;
		}

		void Update(float deltaTime, Shark::Scene* scene);

	private:
		std::vector<Core::GameObject*> m_DynamicObjects;
		std::vector<Core::GameObject*> m_StaticObjects;


		void ResolveCollision(
			Core::GameObject* dynamicObj,
			Components::RigidbodyComponent* physics,
			Components::AABBComponent* dynamicAABB,
			Components::AABBComponent* staticAABB
		);
	};
}

#endif // PHYSICS_SYSTEM_H