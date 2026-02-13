#include "EngineContext.h"
#include "Engine.h"
#include "Graphics/Rendering/ForwardRenderer.h"
#include "Managers/InputManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/MeshManager.h"
#include "Managers/Pathmanager.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"

#include <Source/Managers/LevelEditorManager.h>
#include <GLFW/glfw3.h>

namespace Shark::Core
{
	using Shark::Managers::PathManager;
	using Shark::Managers::InputManager;
	using Shark::Managers::MemoryManager;
	using Shark::Managers::MeshManager;
	using Shark::Managers::SceneManager;
	using Shark::Graphics::ForwardRenderer;
	using Shark::Editor::LevelEditorManager;
	using Shark::Scene;

	void EngineContext::OnInit() {
		SE_LOG(Engine, "EngineContext::OnInit() - Initializing EngineContext!");

		/* ----------------- Initialize Engine ----------------- */
		m_Engine = new Engine();
		m_Engine->Initialize();

		/* ----------------- Create Renderer ----------------- */
		m_Renderer = new ForwardRenderer();
		m_Renderer->Init();

		/* ----------------- Window size ----------------- */
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		/* ----------------- Initialize Managers ----------------- */
		InputManager::Get().Initialize(m_Window);
		PathManager::Get().Initialize();

		/* ----------------- Scene + Camera ----------------- */
		float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		Scene* startingScene = new Scene();
		startingScene->CreateCamera(aspectRatio);

		SceneManager::Get().SetActiveScene(startingScene);

		/* ----------------- Example Objects ----------------- */
		MemoryManager::Get().CheckMemoryStatus();
		LevelEditorManager::Get().RequestModelLoad("Models/Viking_House.obj");
	}

	void EngineContext::OnUpdate(float deltaTime) {

		InputManager::Get().Update(deltaTime);
		SceneManager::Get().Update(deltaTime);
		MeshManager::Get().Update(deltaTime);

		m_Engine->Run();

		glGetError(); // Clear OpenGL errors each frame
	}

	void EngineContext::OnEnd() {
		SE_LOG(Engine, "EngineContext::OnEnd() - Shutting down EngineContext!");

		MeshManager::Get().Shutdown();

		m_Engine->Shutdown();

		if (m_Renderer) { delete m_Renderer; m_Renderer = nullptr; }

		m_Window = nullptr;
	}

}
