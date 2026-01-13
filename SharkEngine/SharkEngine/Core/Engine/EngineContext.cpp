#include "EngineContext.h"
#include "Core/Engine/Scene.h"
#include "Core/Input/InputManager.h"
#include "Core/Components/MeshRendererComponent.h"
#include "Graphics/Rendering/ForwardRenderer.h"
#include "Core/Utilities/Time.h"
#include "Graphics/Resources/MeshManager.h"
#include "Graphics/Resources/PrimitiveMesh.h"
#include "Core/Utilities/Memory.h"

#include <iostream>
#include "SharkEngine.h"
#include <Core/Entities/GameObject.h>
#include <Core/Math/Vector3.h>
#include <Graphics/Resources/Texture.h>
#include <GLFW/glfw3.h>

void EngineContext::OnInit() {
	std::cout << Time::CreateTimeStamp() << ": " << "Initializing EngineContext!\n";

	/* ----------------- Initialize Engine ----------------- */
	m_Engine = new SharkEngine();
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
	Memory::CheckMemoryStatus();
	GameObject* house = new GameObject("Viking_House");
	MeshManager::CreateFromObj(house, "Assets/Models/Viking_House.obj");
	house->GetComponent<MeshRendererComponent>()->GetMaterial()->m_Texture = new Texture("Assets/Models/Viking_House.png");
	m_Scene->AddGameObject(house);
	house->GetTransform().position = Vector3(1.0f, 0.0f, 0.0f);
	house->GetTransform().scale = Vector3(10.f, 10.f, 10.f);

	Memory::CheckMemoryStatus();
	GameObject* house2 = new GameObject("Viking_House2");
	MeshManager::CreateFromObj(house2, "Assets/Models/Viking_House.obj");
	house2->GetComponent<MeshRendererComponent>()->GetMaterial()->m_Texture = new Texture("Assets/Models/Viking_House.png");
	m_Scene->AddGameObject(house2);
	house2->GetTransform().position = Vector3(3.0f, 0.0f, 0.0f);
	house2->GetTransform().scale = Vector3(10.f, 10.f, 10.f);

	Memory::CheckMemoryStatus();
	GameObject* cube = new GameObject("MyCube");
	cube = PrimitiveMesh::CreatePrimitive(cube, PrimitiveType::Cube);
	m_Scene->AddGameObject(cube);
	cube->GetTransform().position = Vector3(-1.0f, 0.0f, 0.0f);

	Memory::CheckMemoryStatus();
	GameObject* cube2 = new GameObject("MyCube2");
	cube = PrimitiveMesh::CreatePrimitive(cube2, PrimitiveType::Cube);
	m_Scene->AddGameObject(cube2);
	cube2->GetTransform().position = Vector3(-3.0f, 0.0f, 0.0f);
}

void EngineContext::OnUpdate(float deltaTime) {
	m_Engine->Run();
	m_Scene->Update(deltaTime);
}

void EngineContext::OnEnd() {
	std::cout << Time::CreateTimeStamp() << ": " << "Closing EngineContext!\n";

	MeshManager::Get().Shutdown();

	m_Engine->Shutdown();

	if (m_Renderer) { delete m_Renderer; m_Renderer = nullptr; }

	if (m_Scene) { delete m_Scene; m_Scene = nullptr; }

	m_Window = nullptr;
}
