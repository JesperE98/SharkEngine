#include "RigidbodyComponent.h"
#include "Core/GameObject.h"

namespace Shark::Components {

	using Math::Vector3;

	void RigidbodyComponent::AddForce(const Vector3& force) {
		// F = m * a => a = F / m
		acceleration += force / mass;
	}

	void RigidbodyComponent::Update(float deltaTime) {
		if(bUseGravity){
			velocity.y += gravity * deltaTime; // Apply gravity to vertical velocity
		}
		else {
			velocity.y = 0.0f;
		}

		

		// Apply horizontal drag
		velocity.x *= drag;
		velocity.z *= drag;

		// Apply acceleration to velocity
		velocity += acceleration * deltaTime;
		acceleration = Vector3(0, 0, 0); // Reset acceleration after applying it

		// Move owner's transform based on velocity
		GetOwner()->GetTransform().position += velocity * deltaTime;
	}
}