#include "LevelEditorManager.h"
#include "Source/Panels/InspectorPanel.h"

#pragma region Engine Includes
#include <Core/Messaging/MessageQueue.h>
#include <Core/Utilities/Debug.h>
#include <Components/Rendering/MeshRendererComponent.h>
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

	using Shark::Components::MeshRendererComponent;
	using Shark::Core::EngineMessage;
	using Shark::Core::MessageType;
	using Shark::Core::GameObject;
	using Shark::Editor::InspectorPanel;
	using Shark::Graphics::Material;
	using Shark::Graphics::Mesh;
	using Shark::Graphics::Texture;
	using Shark::Graphics::PrimitiveType;
	using Shark::Managers::MeshManager;
	using Shark::Managers::SceneManager;
	using Shark::Managers::TextureManager;
	using Shark::Managers::ShaderManager;
	using Shark::Math::Vector3;
	using Shark::Scene;

	void LevelEditorManager::Init()
	{
	}

	void LevelEditorManager::Update(float DeltaTime)
	{
		EngineMessage msg;

		while (inbox.Pop(msg)) {
			ReceiveMessage(msg);
		}
	}

	void LevelEditorManager::Shutdown()
	{
	}

	void LevelEditorManager::RequestModelLoad(const std::string& path)
	{
		EngineMessage msg;
		msg.type = MessageType::LoadModel;
		msg.payload = path;

		MeshManager::Get().inbox.Push(msg);
		
		SE_REQ(Editor, "LevelEditorManager::RequestModelLoad() - Sent load request for: {}", path);
	}

	void LevelEditorManager::RequestPrimitiveLoad(PrimitiveType type)
	{
		EngineMessage msg;
		msg.type = MessageType::LoadPrimitiveType;
		msg.payload = std::to_string(static_cast<int>(type));

		MeshManager::Get().inbox.Push(msg);
	}

	void LevelEditorManager::RequestTextureLoad(const std::string& path)
	{
		EngineMessage msg;
		msg.type = MessageType::LoadTexture;
		msg.payload = path;
		TextureManager::Get().inbox.Push(msg);
	}

	void LevelEditorManager::ReceiveMessage(const EngineMessage& msg) {
		GameObject* selected = InspectorPanel::Get().GetSelectedObject();

		switch (msg.type) {
			case MessageType::ModelLoaded:
			case MessageType::PrimitiveTypeLoaded: {
				Mesh* loadedMesh = static_cast<Mesh*>(msg.data);
				if (selected && selected->GetComponent<MeshRendererComponent>()) {
					selected->GetComponent<MeshRendererComponent>()->SetMesh(loadedMesh);
					SE_LOG(Editor, "LevelEditorManager::ReceiveMessage() - Updated existing object primitive mesh: {}", msg.payload);
				}
				else {
					msg.type == MessageType::ModelLoaded ? LoadModel(msg) : LoadPrimitive(msg);
				}
				break;
			}

			case MessageType::TextureLoaded: {
				if (selected) {
					auto* renderer = selected->GetComponent<MeshRendererComponent>();
					if (renderer && renderer->GetMaterial()) {
						renderer->GetMaterial()->SetTexture(msg.payload);
						SE_SUCC(Editor, "Applied Texture: {} to {}", msg.payload, selected->GetName());
					}
				}
				break;
			}
				
			case MessageType::ShaderLoaded: {
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
				

			case MessageType::ErrorMessage: {
				SE_ERR(Editor, "LevelEditorManager::ReceiveMessage() - Error: {}", msg.payload);
				break;
			}
				
		}
	}

	void LevelEditorManager::LoadModel(const EngineMessage& msg) {
		GameObject* obj = new GameObject(msg.payload);
		Mesh* loadedMesh = static_cast<Mesh*>(msg.data);

		if(!loadedMesh) {
			SE_ERR(Editor, "LevelEditorManager::LoadModel() - Received null mesh for: {}", msg.payload);
			return;
		}

		Material* mat = new Material();

		// Manual Texture for now. Goona remove this when I have Texture Manager that sets teh texture via the editor later on
		/*mat->m_Texture = new Texture("Textures/Viking_House.png");*/

		obj->AddComponent<MeshRendererComponent>(loadedMesh, mat);

		// Add to scene
		Scene* scene = SceneManager::Get().GetActiveScene();

		if (scene) {
			scene->AddGameObject(obj);

			obj->GetTransform().position = Vector3(-0.5f, 0.0f, 0.0f);
			obj->GetTransform().scale = Vector3(10.f, 10.f, 10.f);
			SE_SUCC(Editor, "LevelEditorManager::ReceiveMessage() - Successfully created GameObject: {}", msg.payload);
		}
		else {
			SE_ERR(Editor, "LevelEditorManager::ReceiveMessage() - Failed to create GameObject for loaded model: {}", msg.payload);
			delete obj;
		}
	}

	void LevelEditorManager::LoadPrimitive(const EngineMessage& msg) {
		Mesh* loadedMesh = static_cast<Mesh*>(msg.data);
		if(!loadedMesh) {
			SE_ERR(Editor, "LevelEditorManager::LoadPrimitive() - Received null mesh for primitive type: {}", msg.payload);
			return;
		}

		GameObject* obj = new GameObject("Cube");

		Material* mat = new Material();

		obj->AddComponent<MeshRendererComponent>(loadedMesh, mat);

		Scene* scene = SceneManager::Get().GetActiveScene();
		if (scene) {
			scene->AddGameObject(obj);

			obj->GetTransform().position = Vector3(0.5f, 0.0f, 0.0f);
			obj->GetTransform().scale = Vector3(1.f, 1.f, 1.f);
			SE_SUCC(Editor, "LevelEditorManager::LoadPrimitive() - Successfully created primitive GameObject: {}", obj->GetName());
		}
		else {
			SE_ERR(Editor, "LevelEditorManager::LoadPrimitive() - Failed to create GameObject for primitive type: {}", obj->GetName());
			delete obj;
		}
	}
}