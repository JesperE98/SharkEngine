#include "EngineContext.h"
#include "Graphics/Rendering/ForwardRenderer.h"
#include "Graphics/Resources/MeshManager.h"
#include "Graphics/Resources/ShaderManager.h"
#include "Graphics/Resources/TextureManager.h"
#include "Input/InputManager.h"
#include "IO/Pathmanager.h"
#include "Memory/MemoryManager.h"
#include "Scene/SceneManager.h"

#include "Core/Utilities/Debug.h"
#include "Core/App/EditorStateManager.h"
#include "Core/RecordsManager.h"

#include "Physics/PhysicsSystem.h"

#pragma region COMPONENTS
#include <Components/ComponentRegistry.h>
#include <Components/Logic/CameraComponent.h>
#include <Components/Logic/CameraController.h>
#include <Components/Rendering/LightComponent.h>
#include <Components/PlayerController.h>
#include <Components/Rendering/MeshRendererComponent.h>
#include <Components/Physics/AABBComponent.h>
#include <Components/Physics/RigidbodyComponent.h>
#include <Components/GoalTrigger.h>
#include "Components/UI/MainMenuComponent.h"
#include "Components/UI/LevelTimer.h"
#include "Components/Rendering/TerrainComponent.h"
#include "Components/AIController.h"

#pragma endregion

#include "Scene/Scene.h"
#include <Core/GameObject.h>
#include <Core/Spatial/OctreeSystem.h>

namespace Shark::Core
{
	using IO::PathManager;

	using Input::InputManager;

	using Memory::MemoryManager;

	using Resources::MeshManager;
	using Resources::ShaderManager;
	using Resources::TextureManager;

	using Graphics::ForwardRenderer;
	using Graphics::PrimitiveType;

	using Physics::PhysicsSystem;

	using Components::ComponentRegistry;
	using Components::GoalTrigger;
	using Components::CameraComponent;
	using Components::CameraController;
	using Components::LightComponent;
	using Components::LightType;
	using Components::MainMenuComponent;

	using Spatial::OctreeSystem;

	void EngineContext::Initialize() {
		SE_PROC(Engine, "Initializing Engine Context...");

		/* ----------------- Initialize Managers ----------------- */

		PathManager::Get().Initialize();
		RecordsManager::Get().Initialize();

		RegisterComponents();

		/* ----------------- Create Renderer ----------------- */
		m_Renderer = new ForwardRenderer();

		/* ----------------- Scene ----------------- */

		Components::AABB worldBounds;
		worldBounds.min = { -50, -50, -50 };
		worldBounds.max = { 50, 50, 50 };
		OctreeSystem::Get().Initialize(worldBounds, 5, 8);
		
		SceneManager::Get().LoadSceneFromFile("Content/Scenes/MainMenu.json");

		/* ----------------- Check on Memory ----------------- */
		InputManager::Get().Initialize(m_Window);
		MemoryManager::Get().CheckMemoryStatus();


		SE_SUCC(Engine, "Engine Context setup complete!");
	}

	void EngineContext::Update(float deltaTime) {
		InputManager::Get().Update(deltaTime);
		MeshManager::Get().Update(deltaTime);
		ShaderManager::Get().Update(deltaTime);
		TextureManager::Get().Update(deltaTime);
		SceneManager::Get().Update(deltaTime);

		Scene* scene = SceneManager::Get().GetActiveScene();
		if (scene) {
			scene->Update(deltaTime);
		}

		if (EditorStateManager::Get().IsPlaying()) {
			if (scene) {
				PhysicsSystem::Get().Update(deltaTime, scene);
			}
		}

		glGetError(); // Clear OpenGL errors each frame
	}

	void EngineContext::End() {
		SE_PROC(Engine, "Shutting down Engine Context...");
		MeshManager::Get().Shutdown();

		if (m_Renderer) { 
			delete m_Renderer; 
			m_Renderer = nullptr; 
		}

		m_Window = nullptr;
		SE_SUCC(Engine, "Engine Context shutdown complete!");
	}

	void EngineContext::RegisterComponents() {
		using namespace Shark::Components;

		ComponentRegistry::Get().Register<CameraComponent>("CameraComponent");
		ComponentRegistry::Get().Register<CameraController>("CameraController");
		ComponentRegistry::Get().Register<MeshRendererComponent>("MeshRendererComponent");
		ComponentRegistry::Get().Register<LightComponent>("LightComponent");
		ComponentRegistry::Get().Register<AABBComponent>("AABBComponent");
		ComponentRegistry::Get().Register<RigidbodyComponent>("RigidbodyComponent");
		ComponentRegistry::Get().Register<PlayerController>("PlayerController");
		ComponentRegistry::Get().Register<GoalTrigger>("GoalTrigger");
		ComponentRegistry::Get().Register<MainMenuComponent>("MainMenuComponent");
		ComponentRegistry::Get().Register<LevelTimer>("LevelTimer");
		ComponentRegistry::Get().Register<TerrainComponent>("TerrainComponent");
		ComponentRegistry::Get().Register<AIController>("AIController");
	}
}
