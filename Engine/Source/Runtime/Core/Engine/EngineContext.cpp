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
#include "Physics/PhysicsSystem.h"
#include "Core/App/EditorStateManager.h"

#include "Scene/Scene.h"
#include <Components/Logic/CameraComponent.h>
#include <Components/Logic/CameraController.h>
#include <Core/GameObject.h>
#include <Components/Rendering/LightComponent.h>

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

	using Components::CameraComponent;
	using Components::CameraController;
	using Components::LightComponent;
	using Components::LightType;

	void EngineContext::Initialize() {
		SE_PROC(Engine, "Initializing Engine Context...");

		/* ----------------- Initialize Managers ----------------- */

		PathManager::Get().Initialize();

		/* ----------------- Create Renderer ----------------- */
		m_Renderer = new ForwardRenderer();

		/* ----------------- Scene ----------------- */
		SceneManager::Get().RegisterScene("EditorDefault", [](Scene* scene) {
			// Empty default scene
			GameObject* cam = new GameObject("Main Camera");
			cam->AddComponent<CameraComponent>(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
			cam->AddComponent<CameraController>(5.0f, 0.1f);
			cam->GetTransform().position = { 0.0f, 0.0f, 5.0f };
			scene->AddGameObject(cam);

			GameObject* dLight = new GameObject("Directional Light");
			dLight->AddComponent<LightComponent>();
			dLight->GetComponent<LightComponent>()->Type = LightType::Directional;
			dLight->GetComponent<LightComponent>()->Color = { 1.0f, 1.0f, 1.0f };
			dLight->GetTransform().position = { 0.0f, 5.0f, 0.0f };
			dLight->GetTransform().rotation = { -45.0f, -45.0f, 0.0f };
			scene->AddGameObject(dLight);

			// Optional with Point Light
			//GameObject* pLight = new GameObject("Point Light");
			//pLight->AddComponent<LightComponent>();
			//pLight->GetComponent<LightComponent>()->Type = LightType::Point;
			//pLight->GetComponent<LightComponent>()->Color = { 1.0f, 1.0f, 1.0f };
			//pLight->GetTransform().position = { 0.0f, 1.5f, 0.0f };
			//pLight->GetTransform().Rotate({ 0.0f, 0.0f, 0.0f });
			//scene->AddGameObject(pLight);
			});

		SceneManager::Get().LoadScene("EditorDefault");

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
}
