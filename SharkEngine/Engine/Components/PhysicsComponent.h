#ifndef PHYSICS_COMPONTENT_H
#define PHYSICS_COMPONTENT_H

#include "Component.h"
#include "Math/Vector3.h"

namespace Shark::Components {

	class PhysicsComponent : public Component {
	public:
		Shark::Math::Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
		Shark::Math::Vector3 m_Acceleration{ 0.0f, 0.0f, 0.0f };

		float m_Mass = 1.0f;
		float m_Gravity = -20.0f; // Gravity force applied to the object (negative value for downward force)
		float m_Drag = 0.85f;
		float m_BounceFactor = 0.0f; // How much velocity is retained after a collision (0 = no bounce, 1 = full bounce)

		bool bIsGrounder{ false }; // Whether the object is currently on the ground (used for jumping and gravity)
		bool bUseGravity{ true }; // Whether to apply gravity to this object

		PhysicsComponent() = default;

		/*
		* @brief Applies a force to the object, changing its acceleration based on its mass.
		* @param force The force vector to apply to the object.
		*/
		void AddForce(const Shark::Math::Vector3& force);

		void Update(float deltaTime) override final;
	};
}
#endif // PHYSICS_COMPONTENT_H