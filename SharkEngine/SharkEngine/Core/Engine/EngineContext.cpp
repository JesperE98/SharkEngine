#include "EngineContext.h"
#include "Core/Engine/Scene.h"
#include "Core/Input/InputManager.h"
#include "Graphics/Rendering/ForwardRenderer.h"
#include "Core/Utilities/Time.h"
#include "Graphics/Resources/MeshLoader.h"
#include "Graphics/Resources/PrimitiveMesh.h"
#include "Core/Utilities/Memory.h"

#include <iostream>

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
	MeshLoader::CreateFromObj(house, "Assets/Models/Viking_House.obj");
	house->GetTransform().position = Vector3(0.0f, 0.0f, 0.0f);
	m_Scene->AddGameObject(house);

	GameObject* cube = new GameObject("MyCube");
	cube = PrimitiveMesh::CreatePrimitveCube(cube);
	m_Scene->AddGameObject(cube);

	Memory::CheckMemoryStatus();
}

void EngineContext::OnUpdate(float deltaTime) {
	m_Engine->Run();
	m_Scene->Update(deltaTime);
}

void EngineContext::OnEnd() {
	std::cout << Time::CreateTimeStamp() << ": " << "Closing EngineContext!\n";

	m_Engine->Shutdown();

	if (m_Renderer) { delete m_Renderer; m_Renderer = nullptr; }

	if (m_Scene) { delete m_Scene; m_Scene = nullptr; }

	m_Window = nullptr;
}
