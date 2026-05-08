#include "PlayerController.h"
#include "AIController.h"

#include "Core/GameObject.h"
#include "Physics/RigidbodyComponent.h"
#include "Rendering/TerrainComponent.h"

#include "Physics/AABBComponent.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include <GLFW/glfw3.h>

namespace Shark::Components {

	using Math::Vector3;

	void PlayerController::BeginPlay()
	{
		m_RigidbodyComp = GetOwner()->GetComponent<RigidbodyComponent>();

		if (!m_RigidbodyComp) {
			m_RigidbodyComp = GetOwner()->AddComponent<RigidbodyComponent>();
		}
	}

	void PlayerController::Update(float deltaTime)
	{
		if (!m_RigidbodyComp) {
			m_RigidbodyComp = GetOwner()->GetComponent<RigidbodyComponent>();
			if (!m_RigidbodyComp) return;
		}

		GLFWwindow* window = glfwGetCurrentContext();
		if (!window) return;

		// Capture spawn position on first frame
		if (!m_SpawnPointSet) {
			m_SpawnPoint = GetOwner()->GetTransform().position;
			m_SpawnPointSet = true;
		}

		// Death plane - respawn if player falls below killY
		if (GetOwner()->GetTransform().position.y < killY) {
			GetOwner()->GetTransform().position = m_SpawnPoint;
			m_RigidbodyComp->velocity = { 0.0f, 0.0f, 0.0f };
			SE_LOG(Engine, "Player respawned");
		}

		if (m_DashTimer > 0.0f)			m_DashTimer -= deltaTime;
		if (m_DashActiveTimer > 0.0f)	m_DashActiveTimer -= deltaTime;

		updateMovement(window);

		// ===== TERRAIN COLLISION
		auto* scene = Core::SceneManager::Get().GetActiveScene();
		if (scene) {
			for (auto* obj : scene->GetGameObjects()) {
				auto* terrainComp = obj->GetComponent<Components::TerrainComponent>();
				if (!terrainComp || !terrainComp->GetTerrain()) continue;

				auto& terrainTransform = obj->GetTransform();
				auto& playerPos = GetOwner()->GetTransform().position;

				// Convert player position to terrain-local space
				float localX = playerPos.x - terrainTransform.position.x;
				float localZ = playerPos.z - terrainTransform.position.z;

				float terrainHeight = terrainComp->GetTerrain()->GetHeightAt(localX, localZ);
				terrainHeight += terrainTransform.position.y; // offset by terrain's world Y

				/*SE_LOG(Engine, "Player Y: {} | Terrain H: {} | LocalX: {} | LocalZ: {}",
					   playerPos.y, terrainHeight, localX, localZ);*/

				// Push player up if below terrain
				float playerFeetY = playerPos.y - 0.5f; // half the player's height
				if (playerFeetY < terrainHeight) {
					playerPos.y = terrainHeight + 0.5f;
					m_RigidbodyComp->velocity.y = 0.0f;
					m_RigidbodyComp->bIsGrounded = true;
				}
			}
		}

		// ===== ENEMY COLLISION CHECK
		for (auto* obj : scene->GetGameObjects()) {
			if (!obj || obj->bMarkedForDeletion) continue;
			if (obj == GetOwner()) continue;

			auto* enemy = obj->GetComponent<AIController>();
			if (!enemy) continue;

			// Simple distance-based collision
			auto& playerPos = GetOwner()->GetTransform().position;
			auto& enemyPos = obj->GetTransform().position;

			float dx = playerPos.x - enemyPos.x;
			float dy = playerPos.y - enemyPos.y;
			float dz = playerPos.z - enemyPos.z;
			float distSq = dx * dx + dy * dy + dz * dz;

			float hitRadius = 1.5f;

			if (distSq < hitRadius * hitRadius) {
				enemy->OnPlayerHit();
				GetOwner()->GetTransform().position = m_SpawnPoint;
				m_RigidbodyComp->velocity = { 0.0f, 0.0f, 0.0f };
				SE_LOG(Engine, "Player hit by enemy! Respawning...");
				break;
			}
		}

	}

	void PlayerController::updateMovement(GLFWwindow* window) {

		Vector3 moveDir = Vector3(0, 0, 0);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDir.z -= 1.0f;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDir.z += 1.0f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDir.x -= 1.0f;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDir.x += 1.0f;

		// Normalize diagonal movement
		float len = std::sqrt(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
		if (len > 0.001f) {
			moveDir.x /= len;
			moveDir.z /= len;
		}

		updateDash(window, moveDir, len);
		updateJump(window);

		// Applying horizontal velocity directly for snappier movement than forces
		if (m_DashActiveTimer <= 0.0f) {
			m_RigidbodyComp->velocity.x = moveDir.x * moveSpeed;
			m_RigidbodyComp->velocity.z = moveDir.z * moveSpeed;
		}
	}

	void PlayerController::updateJump(GLFWwindow* window) {
		bool jumpPressed = ( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS );

		if (jumpPressed && !m_JumpPressedLast && m_RigidbodyComp->bIsGrounded) {
			m_RigidbodyComp->velocity.y = jumpForce;
		}
		m_JumpPressedLast = jumpPressed;
	}

	void PlayerController::updateDash(GLFWwindow* window, const Vector3& moveDir, float len) {
		bool dashPressed = ( glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS );

		if (dashPressed && !m_DashPressedLast && m_DashTimer <= 0.0f && len > 0.001f) {

			m_RigidbodyComp->velocity.x = moveDir.x * dashForce;
			m_RigidbodyComp->velocity.z = moveDir.z * dashForce;

			m_DashTimer = dashCooldown;
			m_DashActiveTimer = dashDuration;

			m_RigidbodyComp->velocity.y = 0.0f;
		}

		m_DashPressedLast = dashPressed;
	}
}