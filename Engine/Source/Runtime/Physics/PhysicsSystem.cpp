#include "PhysicsSystem.h"
#include "Scene/Scene.h"
#include "Core/GameObject.h"
#include "Components/Physics/AABBComponent.h"
#include "Components/Physics/RigidBodyComponent.h"
#include "Math/Vector3.h"

#include <vector>


namespace Shark::Physics {

	using Components::AABBComponent;
	using Components::RigidbodyComponent;
	using Components::AABB;
	using Core::GameObject;
	using Math::Vector3;
	using Shark::Scene;

	void PhysicsSystem::Update(float deltaTime, Scene* scene)
	{
		const auto& objects = scene->GetGameObjects();

		// Need to differentiate dynamic and static objects apart from ech other
		std::vector<GameObject*> dynamicObjects;
		std::vector<GameObject*> staticObjects;

		for (GameObject* obj : objects) {
			AABBComponent* aabb = obj->GetComponent<AABBComponent>();
			if (!aabb) continue;

			if (aabb->bIsStatic) 
			{
				staticObjects.push_back(obj);
			}
			else
			{
				dynamicObjects.push_back(obj);
			}
		}

		// Update physics (gravity + velocity)
		for (GameObject* obj : dynamicObjects) {
			RigidbodyComponent* physics	= obj->GetComponent<RigidbodyComponent>();

			if (physics)
				physics->Update(deltaTime);
		}

		// Resolve collisions
		for (GameObject* dynObj : dynamicObjects) {
			RigidbodyComponent* physics = dynObj->GetComponent<RigidbodyComponent>();
			AABBComponent* dynAABB = dynObj->GetComponent<AABBComponent>();

			if (!physics || !dynAABB) continue;

			physics->bIsGrounded = false;

			for (GameObject* statObj : staticObjects) {
				AABBComponent* statAABB = statObj->GetComponent<AABBComponent>();

				if (!statAABB) continue;

				AABB a = dynAABB->GetWorldAABB();
				AABB b = statAABB->GetWorldAABB();

				if (!a.Intersects(b)) continue;

				ResolveCollision(dynObj, physics, dynAABB, statAABB);
			}
		}
	}

	void PhysicsSystem::ResolveCollision(
		GameObject* dynamicObj, 
		RigidbodyComponent* physics,
		AABBComponent* dynamicAABB, 
		AABBComponent* staticAABB)
	{
		AABB a = dynamicAABB->GetWorldAABB();
		AABB b = staticAABB->GetWorldAABB();

		Vector3 pen = a.GetPenetrationDepth(b);

		// Resolve along axis of minimum penetration
		if (pen.x < pen.y && pen.x < pen.z) {
			// X axis
			float dir = a.GetCenter().x < b.GetCenter().x ? -1.0f : 1.0f;
			dynamicObj->GetTransform().position.x += pen.x * dir;
			physics->velocity.x = 0.0f;
		}
		else if (pen.y < pen.x && pen.y < pen.z) {
			// Y axis - landing on top or hitting ceiling
			float dir = a.GetCenter().y < b.GetCenter().y ? -1.0f : 1.0f;
			dynamicObj->GetTransform().position.y += pen.y * dir;

			if (dir < 0) {
				// Ceiling hit
				physics->velocity.y = 0.0f;
			}
			else {
				// Landing
				physics->velocity.y *= -physics->bounceFactor;
				physics->bIsGrounded = true;
			}
		}
		else {
			// Z Axis
			float dir = a.GetCenter().z < b.GetCenter().z ? -1.0f : 1.0f;
			dynamicObj->GetTransform().position.z += pen.z * dir;
			physics->velocity.z = 0.0f;
		}
	}
}

