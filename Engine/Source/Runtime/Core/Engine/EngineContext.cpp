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

	void EngineContext::Initialize() {
		SE_PROC(Engine, "Initializing Engine Context...");

		/* ----------------- Initialize Managers ----------------- */

		PathManager::Get().Initialize();

		/* ----------------- Create Renderer ----------------- */
		m_Renderer = new ForwardRenderer();

		/* ----------------- Scene ----------------- */
		SceneManager::Get().RegisterScene("EditorDefault", [](Scene* scene) {
			// Empty default scene
			//auto* cameraObj = new GameObject("Main Camera2");
			//cameraObj->GetTransform().position = { 0.0f, 2.0f, 10.0f };
			//cameraObj->AddComponent<CameraComponent>(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
			//scene->AddGameObject(cameraObj);

			//// default directional light
			//auto* sunObj = new GameObject("Directional Light");
			//sunObj->GetTransform().position = { 0.0f, 10.0f, 0.0f };
			//sunObj->GetTransform().Rotate({ -45.0f, 30.0f, 0.0f });
			//sunObj->AddComponent<LightComponent>()->Type = LightType::Directional;
			//scene->AddGameObject(sunObj);
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
