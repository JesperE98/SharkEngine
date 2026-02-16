#include "LevelEditorManager.h"

#pragma region Engine Includes
#include <Core/Messaging/MessageQueue.h>
#include <Core/Utilities/Debug.h>
#include <Components/Rendering/MeshRendererComponent.h>
#include <Core/GameObject.h>
#include <Graphics/Resources/Texture.h>
#include <Managers/MeshManager.h>
#include <Managers/SceneManager.h>
#include <Scene/Scene.h>
#include <Math/MathUtils.h>
#pragma endregion

namespace Shark::Editor {

	using Shark::Scene;
	using Shark::Graphics::Material;
	using Shark::Graphics::Mesh;
	using Shark::Graphics::Texture;
	using Shark::Managers::MeshManager;
	using Shark::Managers::SceneManager;
	using Shark::Core::EngineMessage;
	using Shark::Core::MessageType;
	using Shark::Core::GameObject;
	using Shark::Components::MeshRendererComponent;
	using Shark::Math::Vector3;

	void LevelEditorManager::Init()
	{
	}

	void LevelEditorManager::Update(float DeltaTime)
	{
		EngineMessage msg;

		while (inbox.Pop(msg)) {
			ProcessEngineReply(msg);
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

	void LevelEditorManager::ReceiveMessage(const EngineMessage& msg) {

		if (msg.type == MessageType::ModelLoaded) {
			SE_LOG(Editor, "LevelEditorManager::ReceiveMessage() - Received loaded model: {}", msg.payload);
			
			Mesh* loadedMesh = static_cast<Mesh*>(msg.data);

			GameObject* obj = new GameObject(msg.payload);

			Material* mat = new Material();

			// Manual Texture for now. Goona remove this when I have Texture Manager that sets teh texture via the editor later on
			mat->m_Texture = new Texture("Textures/Viking_House.png");

			obj->AddComponent<MeshRendererComponent>(loadedMesh, mat);

			// Add to scene
			Scene* scene = SceneManager::Get().GetActiveScene();

			if (scene) {
				scene->AddGameObject(obj);

				obj->GetTransform().position = Vector3(1.0f, 0.0f, 0.0f);
				obj->GetTransform().scale = Vector3(10.f, 10.f, 10.f);
				SE_SUCC(Editor, "LevelEditorManager::ReceiveMessage() - Successfully created GameObject: {}", msg.payload);
			}
			else {
			SE_ERR(Editor, "LevelEditorManager::ReceiveMessage() - Failed to create GameObject for loaded model: {}", msg.payload);
				delete obj;
				delete mat;
				delete loadedMesh;
			}
		}
		else if (msg.type == MessageType::ErrorMessage) {
			SE_ERR(Editor, "LevelEditorManager::ReceiveMessage() - Error: {}", msg.payload);
		}
	}

	void LevelEditorManager::ProcessEngineReply(const EngineMessage& msg)
	{
		if (msg.type == MessageType::ModelLoaded) {
			SE_SUCC(Editor, "LevelEditorManager::ProcessEngineReply() - Received loaded model: {}", msg.payload);
			Mesh* loadedMesh = static_cast<Mesh*>(msg.data);
			m_ActiveMeshes.push_back(loadedMesh);
		}
		else if (msg.type == MessageType::ErrorMessage) {
			SE_ERR(Editor, "LevelEditorManager::ProcessEngineReply() - Error: {}", msg.payload);
		}
	}
}