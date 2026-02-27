#include "EngineContext.h"
#include "Engine.h"
#include "Graphics/Rendering/ForwardRenderer.h"
#include "Managers/InputManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/MeshManager.h"
#include "Managers/Pathmanager.h"
#include "Managers/SceneManager.h"
#include "Managers/TextureManager.h"
#include "Managers/ShaderManager.h"
#include "Scene/Scene.h"
#include "Graphics/Resources/PrimitiveMesh.h"

#include <Source/Managers/LevelEditorManager.h>
#include <GLFW/glfw3.h>

namespace Shark::Core
{
	using Shark::Managers::PathManager;
	using Shark::Managers::InputManager;
	using Shark::Managers::MemoryManager;
	using Shark::Managers::MeshManager;
	using Shark::Managers::ShaderManager;
	using Shark::Managers::TextureManager;
	using Shark::Managers::SceneManager;
	using Shark::Graphics::ForwardRenderer;
	using Shark::Editor::LevelEditorManager;
	using Shark::Graphics::PrimitiveType;
	using Shark::Scene;

	void EngineContext::OnInitialize() {
		SE_LOG(Engine, "EngineContext::OnInitialize() - Initializing EngineContext!");

		/* ----------------- OnInitialize Engine ----------------- */
		m_Engine = new Engine();
		m_Engine->OnInitialize();

		/* ----------------- Create Renderer ----------------- */
		m_Renderer = new ForwardRenderer();
		m_Renderer->Init();

		/* ----------------- Window size ----------------- */
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		/* ----------------- OnInitialize Managers ----------------- */
		InputManager::Get().OnInitialize(m_Window);
		PathManager::Get().OnInitialize();

		/* ----------------- Scene ----------------- */
		Scene* initialScene = new Scene();

		SceneManager::Get().SetActiveScene(initialScene);

		/* ----------------- Example Objects ----------------- */
		MemoryManager::Get().CheckMemoryStatus();
		LevelEditorManager::Get().RequestModelLoad("Models/Viking_House.obj");
		LevelEditorManager::Get().RequestPrimitiveLoad(PrimitiveType::Cube);
	}

	void EngineContext::OnUpdate(float deltaTime) {

		m_Engine->Run();
		InputManager::Get().Update(deltaTime);
		SceneManager::Get().Update(deltaTime);
		MeshManager::Get().Update(deltaTime);
		ShaderManager::Get().Update(deltaTime);
		TextureManager::Get().Update(deltaTime);

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
