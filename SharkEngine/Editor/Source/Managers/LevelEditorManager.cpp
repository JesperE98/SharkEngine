#include "LevelEditorManager.h"
#include "Source/EditorWindows/InspectorWindow.h"

#pragma region Engine Includes
#include <Core/Utilities/Debug.h>
#include <Components/Rendering/MeshRendererComponent.h>
#include <Components/Physics/RigidbodyComponent.h>
#include <Components/Physics/AABBComponent.h>
#include <Core/GameObject.h>
#include <Managers/MeshManager.h>
#include <Managers/SceneManager.h>
#include <Scene/Scene.h>
#include <Math/MathUtils.h>
#include <Graphics/Resources/PrimitiveMesh.h>
#include <Managers/TextureManager.h>
#include <Managers/ShaderManager.h>

#pragma endregion

namespace Shark::Editor {

	using Components::MeshRendererComponent;
	using Components::RigidbodyComponent;
	using Components::AABBComponent;
	using Core::Message;
	using Core::EventType;
	using Core::GameObject;
	using Editor::InspectorWindow;
	using Graphics::Material;
	using Graphics::Mesh;
	using Graphics::Texture;
	using Graphics::PrimitiveType;
	using Managers::MeshManager;
	using Managers::SceneManager;
	using Managers::TextureManager;
	using Managers::ShaderManager;
	using Math::Vector3;
	using Shark::Scene;

	void LevelEditorManager::Initialize()
	{
	}

	void LevelEditorManager::Update(float DeltaTime)
	{
		Message msg;

		while (inbox.Pop(msg)) {
			ReceiveMessage(msg);
		}
	}

	void LevelEditorManager::Shutdown()
	{
		if (m_InspectorWindow) m_InspectorWindow = nullptr;
	}

	void LevelEditorManager::RequestModelLoad(const std::string& path)
	{
		Message msg;
		msg.type = EventType::LoadModel;
		msg.payload = path;

		MeshManager::Get().inbox.Push(msg);
		
		SE_REQ(Editor, "LevelEditorManager::RequestModelLoad() - Sent load request for: {}", path);
	}

	void LevelEditorManager::RequestPrimitiveLoad(PrimitiveType type)
	{
		Message msg;
		msg.type = EventType::LoadPrimitiveType;
		msg.payload = std::to_string(static_cast<int>(type));

		MeshManager::Get().inbox.Push(msg);
	}

	void LevelEditorManager::RequestDiffTextureLoad(const std::string& path)
	{
		Message msg;
		msg.type = EventType::LoadTexture;
		msg.payload = path;
		TextureManager::Get().inbox.Push(msg);

		SE_REQ(Editor, "LevelEditorManager::RequestDiffTextureLoad() - Sent load request for: {}", path);
	}

	void LevelEditorManager::RequestSpecTextureLoad(const std::string& path)
	{
		Message msg;
		msg.type = EventType::LoadTexture;
		msg.payload = path;
		TextureManager::Get().inbox.Push(msg);

		SE_REQ(Editor, "LevelEditorManager::RequestSpecTextureLoad() - Sent load request for: {}", path);
	}

	void LevelEditorManager::ReceiveMessage(const Message& msg) {
		GameObject* selected = m_InspectorWindow->GetSelectedObject();

		switch (msg.type) {
			case EventType::ModelLoaded:
			case EventType::PrimitiveTypeLoaded: {
				Mesh* loadedMesh = static_cast<Mesh*>(msg.data);
				if (selected && selected->GetComponent<MeshRendererComponent>()) {
					MeshRendererComponent* renderer = selected->GetComponent<MeshRendererComponent>();
					renderer->SetMesh(loadedMesh);

					if(!renderer->GetMaterial()) {
						renderer->SetMaterial(new Material());
					}

					SE_LOG(Editor, "LevelEditorManager::ReceiveMessage() - Updated existing object primitive mesh: {}", msg.payload);
				}
				else {
					msg.type == EventType::ModelLoaded ? LoadModel(msg) : LoadPrimitive(msg);
				}

				SE_LOG(Editor, "Swapping mesh. New Vertex count: {}", loadedMesh->indices.size());
				break;
			}

			case EventType::TextureLoaded: {
				if (selected) {
					auto* renderer = selected->GetComponent<MeshRendererComponent>();
					if (renderer && renderer->GetMaterial()) {
						renderer->GetMaterial()->SetTexture(msg.payload);
						SE_SUCC(Editor, "Applied Texture: {} to {}", msg.payload, selected->GetName());
					}
				}
				break;
			}
				
			case EventType::ShaderLoaded: {
				if (selected) {
					auto* renderer = selected->GetComponent<MeshRendererComponent>();
					if (renderer && renderer->GetMaterial()) {
						renderer->GetMaterial()->SetShader(
							msg.payload,
							msg.payload + ".vert.glsl",
							msg.payload + ".frag.glsl"
						);

						SE_SUCC(Editor, "Applied Shader: {} to {}", msg.payload, selected->GetName());
					}
				}
				break;
			}
				

			case EventType::ErrorMessage: {
				SE_ERR(Editor, "LevelEditorManager::ReceiveMessage() - Error: {}", msg.payload);
				break;
			}
				
		}
	}

	void LevelEditorManager::LoadModel(const Message& msg) {
		GameObject* obj = new GameObject(msg.payload);
		Mesh* loadedMesh = static_cast<Mesh*>(msg.data);

		if(!loadedMesh) {
			SE_ERR(Editor, "LevelEditorManager::LoadModel() - Received null mesh for: {}", msg.payload);
			return;
		}

		Material* mat = new Material();

		obj->AddComponent<MeshRendererComponent>(loadedMesh, mat);
		//obj->AddComponent<RigidBodyComponent>();
		//obj->AddComponent<AABBComponent>(Vector3(5.0f, 0.5f, 5.0f), false);

		// Add to scene
		Scene* scene = SceneManager::Get().GetActiveScene();

		if (scene) {
			scene->AddGameObject(obj);

			obj->GetTransform().position = Vector3(0.0f, 0.0f, 0.0f);
			obj->GetTransform().scale = Vector3(10.f, 10.f, 10.f);
			obj->GetComponent<MeshRendererComponent>()->GetMaterial()->SetTexture("Textures/Viking_House.png");
			SE_SUCC(Editor, "LevelEditorManager::ReceiveMessage() - Successfully created GameObject: {}", msg.payload);
		}
		else {
			SE_ERR(Editor, "LevelEditorManager::ReceiveMessage() - Failed to create GameObject for loaded model: {}", msg.payload);
			delete obj;
		}
	}

	void LevelEditorManager::LoadPrimitive(const Message& msg) {
		Mesh* loadedMesh = static_cast<Mesh*>(msg.data);
		if(!loadedMesh) {
			SE_ERR(Editor, "LevelEditorManager::LoadPrimitive() - Received null mesh for primitive type: {}", msg.payload);
			return;
		}

		GameObject* obj = new GameObject("Platform");
		obj->GetTransform().position = { 0.0f, -0.5f, 0.0f };
		obj->GetTransform().SetScale({ 10.0f, 0.5f, 10.0f });
		Material* mat = new Material();

		obj->AddComponent<MeshRendererComponent>(loadedMesh, mat);
		obj->AddComponent<AABBComponent>(Vector3(0.5f, 0.5f, 0.5f), true);

		Scene* scene = SceneManager::Get().GetActiveScene();
		if (scene) {
			scene->AddGameObject(obj);

			SE_SUCC(Editor, "LevelEditorManager::LoadPrimitive() - Successfully created primitive GameObject: {}", obj->GetName());
		}
		else {
			SE_ERR(Editor, "LevelEditorManager::LoadPrimitive() - Failed to create GameObject for primitive type: {}", obj->GetName());
			delete obj;
		}
	}
}