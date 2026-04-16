#include "PhysicsComponent.h"
#include "Core/GameObject.h"

namespace Shark::Components {

	using Shark::Math::Vector3;

	void PhysicsComponent::AddForce(const Shark::Math::Vector3& force) {
		// F = m * a => a = F / m
		m_Acceleration += force / m_Mass;
	}

	void PhysicsComponent::Update(float deltaTime) {
		if(bUseGravity){
			m_Velocity.y += m_Gravity * deltaTime; // Apply gravity to vertical velocity
		}

		// Apply horizontal drag
		m_Velocity.x *= m_Drag;
		m_Velocity.z *= m_Drag;

		// Apply acceleration to velocity
		m_Velocity += m_Acceleration * deltaTime;
		m_Acceleration = Vector3(0, 0, 0); // Reset acceleration after applying it

		// Move owner's transform based on velocity
		GetOwner()->GetTransform().position += m_Velocity * deltaTime;
	}
}