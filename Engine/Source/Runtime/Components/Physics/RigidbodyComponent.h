#ifndef PHYSICS_COMPONTENT_H
#define PHYSICS_COMPONTENT_H

#include "Components/Component.h"
#include "Math/Vector3.h"

namespace Shark::Components {

	class RigidbodyComponent : public Component {
	public:
		Math::Vector3 velocity{ 0.0f, 0.0f, 0.0f };
		Math::Vector3 acceleration{ 0.0f, 0.0f, 0.0f };

		float mass = 1.0f;
		float gravity = -20.0f; // Gravity force applied to the object (negative value for downward force)
		float drag = 0.85f;
		float bounceFactor = 0.0f; // How much velocity is retained after a collision (0 = no bounce, 1 = full bounce)

		bool bIsGrounded{ false }; // Whether the object is currently on the ground (used for jumping and gravity)
		bool bUseGravity{ true }; // Whether to apply gravity to this object

		RigidbodyComponent() {
			tickMode = TickMode::PlayOnly;
		}

		/*
drag		* @brief Applies a force to the object, changing its acceleration based on its mass.
		* @param force The force vector to apply to the object.
		*/
		void AddForce(const Math::Vector3& force);

		void Update(float deltaTime) override final;
	};
}
#endif // PHYSICS_COMPONTENT_H