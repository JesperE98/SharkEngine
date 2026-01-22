#include "Components/MeshRendererComponent.h"
#include "Core/Utilities/Time.h"
#include "Engine.h"
#include "EngineContext.h"
#include "Entities/GameObject.h"
#include "Graphics/Rendering/ForwardRenderer.h"
#include "Graphics/Resources/PrimitiveMesh.h"
#include "Graphics/Resources/Texture.h"
#include "Managers/InputManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/MeshManager.h"
#include "Managers/Pathmanager.h"
#include "Math/Vector3.h"
#include "Scene/Scene.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace Shark::Core
{
	using Shark::Managers::PathManager;
	using Shark::Managers::InputManager;
	using Shark::Managers::MemoryManager;
	using Shark::Managers::MeshManager;
	using Shark::Entities::GameObject;
	using Shark::Graphics::ForwardRenderer;
	using Shark::Components::MeshRendererComponent;
	using Shark::Graphics::PrimitiveMesh;
	using Shark::Graphics::PrimitiveType;
	using Shark::Graphics::Texture;
	using Shark::Math::Vector3;
	using Shark::Scene;

	void EngineContext::OnInit() {

		/* ----------------- Initialize PathManager ----------------- */
		PathManager::GetInstance().Initialize();

		SHARK_LOG(Engine, "EngineContext::OnInit() - Initializing EngineContext!");

		/* ----------------- Initialize Engine ----------------- */
		m_Engine = new Engine();
		m_Engine->Initialize();

		/* ----------------- Create Renderer ----------------- */
		m_Renderer = new ForwardRenderer();
		m_Renderer->Init();

		/* ----------------- Window size ----------------- */
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		/* ----------------- Initialize InputSystem ----------------- */
		InputManager::Init(m_Window);

		/* ----------------- Scene + Camera ----------------- */
		float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		m_Scene = new Scene();
		m_Scene->CreateCamera(aspectRatio);

		/* ----------------- Example Objects ----------------- */
		MemoryManager::GetInstance().CheckMemoryStatus();

		GameObject* house = new GameObject("Viking_House");
		MeshManager::CreateFromObj(house, "Models/Viking_House.obj");
		house->GetComponent<MeshRendererComponent>()->GetMaterial()->m_Texture = new Texture("Textures/Viking_House.png");
		m_Scene->AddGameObject(house);
		house->GetTransform().position = Vector3(1.0f, 0.0f, 0.0f);
		house->GetTransform().scale = Vector3(10.f, 10.f, 10.f);

		MemoryManager::GetInstance().CheckMemoryStatus();
		GameObject* house2 = new GameObject("Viking_House2");
		MeshManager::CreateFromObj(house2, "Models/Viking_House.obj");
		house2->GetComponent<MeshRendererComponent>()->GetMaterial()->m_Texture = new Texture("Textures/Viking_House.png");
		m_Scene->AddGameObject(house2);
		house2->GetTransform().position = Vector3(3.0f, 0.0f, 0.0f);
		house2->GetTransform().scale = Vector3(10.f, 10.f, 10.f);

		MemoryManager::GetInstance().CheckMemoryStatus();
		GameObject* cube = new GameObject("MyCube");
		cube = PrimitiveMesh::CreatePrimitive(cube, PrimitiveType::Cube);
		m_Scene->AddGameObject(cube);
		cube->GetTransform().position = Vector3(-1.0f, 0.0f, 0.0f);

		MemoryManager::GetInstance().CheckMemoryStatus();
		GameObject* cube2 = new GameObject("MyCube2");
		cube = PrimitiveMesh::CreatePrimitive(cube2, PrimitiveType::Cube);
		m_Scene->AddGameObject(cube2);
		cube2->GetTransform().position = Vector3(-3.0f, 0.0f, 0.0f);
	}

	void EngineContext::OnUpdate(float deltaTime) {
		m_Engine->Run();
		m_Scene->Update(deltaTime);
		glGetError(); // Clear OpenGL errors each frame
	}

	void EngineContext::OnEnd() {
		SHARK_LOG(Engine, "EngineContext::OnEnd() - Shutting down EngineContext!");

		MeshManager::Get().Shutdown();

		m_Engine->Shutdown();

		if (m_Renderer) { delete m_Renderer; m_Renderer = nullptr; }

		if (m_Scene) { delete m_Scene; m_Scene = nullptr; }

		m_Window = nullptr;
	}

}
