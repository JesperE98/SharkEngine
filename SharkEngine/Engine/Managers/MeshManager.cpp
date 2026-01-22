#include "MeshManager.h"
#include "Components/MeshRendererComponent.h"
#include "Core/Utilities/Time.h"
#include "Entities/GameObject.h"
#include "Graphics/Resources/Material.h"
#include "Graphics/Resources/Mesh.h"
#include "Graphics/Resources/PrimitiveMesh.h"

#include <iostream>
#include <ostream>

namespace Shark::Managers {

	using Shark::Core::EngineMessage;
	using Shark::Core::MessageType;
	using Shark::Components::MeshRendererComponent;
	using Shark::Entities::GameObject;
	using Shark::Graphics::Mesh;
	using Shark::Graphics::Material;
	using Shark::Graphics::PrimitiveType;
	using Shark::Graphics::PrimitiveMesh;

	void MeshManager::Update(float DeltaTime)
	{
		EngineMessage msg;
		while (inbox.Pop(msg)) {
			if (msg.type == MessageType::LoadModel) {
				ProcessLoadRequest(msg.payload);
			}
		}
	}

	void MeshManager::AddMeshToCache(const std::string& key, Mesh* mesh)
	{
		if (m_MeshCache.find(key) == m_MeshCache.end()) {
			m_MeshCache[key] = mesh;
		}
	}

	Mesh* MeshManager::GetMesh(const std::string& filePath)
	{
		if (m_MeshCache.find(filePath) != m_MeshCache.end()) {
			return m_MeshCache[filePath];
		}
		return nullptr;
	}

	Mesh* MeshManager::LoadMesh(PrimitiveType type)
	{
		std::string key = "";

		// Go through every primitive type and assign key
		switch (type) {
		case PrimitiveType::None:
			return nullptr;

		case PrimitiveType::Cube:
			key = "Engine::Primitive_Cube";
			break;

		case PrimitiveType::Sphere:
			break;
		case PrimitiveType::Cylinder:
			break;
		case PrimitiveType::Plane:
			break;
		default:
			return nullptr;
		}

		// If found in cache, return it
		Mesh* cachedMesh = GetMesh(key);
		if (cachedMesh != nullptr) {
			return cachedMesh;
		}

		// Not found, create new mesh
		Mesh* newMesh = nullptr;
		switch (type) {
		case PrimitiveType::None:
			return nullptr;

		case PrimitiveType::Cube:
			newMesh = PrimitiveMesh::CreateCube();
			break;

		case PrimitiveType::Sphere:
			break;
		case PrimitiveType::Cylinder:
			break;
		case PrimitiveType::Plane:
			break;
		default:
			SHARK_WARN(Rendering, "MeshManager::LoadMesh() - Unknown PrimitiveType!");
			return nullptr;
		}

		// Add to cache
		if (newMesh) AddMeshToCache(key, newMesh);

		return newMesh;
	}

	Mesh* MeshManager::LoadMesh(const std::string& filePath)
	{
		// 1. Check Cache
		Mesh* cachedMesh(GetMesh(filePath));

		if (cachedMesh != nullptr) {
			return cachedMesh;
		}

		SHARK_WARN(Rendering, "MeshManager::LoadMesh() - Cache miss! Loading {} from disk...", filePath);
		// 2. Load from disk
		Mesh* newMesh = new Mesh();
		if (newMesh->LoadMeshFromModel(filePath)) {
			m_MeshCache[filePath] = newMesh;
			return newMesh;
		}

		// 3. Cleanup if failed
		delete newMesh;
		return nullptr;
	}

	void MeshManager::Shutdown()
	{
		for (auto& pair : m_MeshCache) {
			if (pair.second) {
				delete pair.second;
			}
		}
	}

	void MeshManager::CreateFromObj(GameObject* obj, const std::string& filePath, Material* mat) {

		if (!obj) {
			SHARK_ERR(Rendering, "MeshManager::CreateFromObj() - Passing null GameObject to CreateFromObj");
			return;
		}

		Mesh* newMesh = MeshManager::Get().LoadMesh(filePath);
		if (newMesh) {
			Material* material = mat ? mat : new Material(); // if none mat wasn't provided, create default material
			obj->AddComponent(new MeshRendererComponent(obj, newMesh, material));
			SHARK_LOG(Rendering, "MeshManager::CreateFromObj() - Loaded mesh from {} and attached to GameObject: {}", filePath, obj->GetName());
		}
		else {
			SHARK_ERR(Rendering, "MeshManager::CreateFromObj() - Failed to load mesh from {}", filePath);
		}
	}

	void MeshManager::ProcessLoadRequest(const std::string& path)
	{
		Mesh* loadedMesh = LoadMesh(path);

		if (loadedMesh) {
			EngineMessage reply;
			reply.type = MessageType::ModelLoaded;
			reply.payload = path;
			reply.data = static_cast<void*>(loadedMesh);

			//LevelEditorManager::Get().inbox.Push(reply);

			SHARK_LOG(Rendering, "MeshManager::ProcessLoadRequest() - Handshake sent to {}", path);
		}
		else {
			EngineMessage errorMsg;
			errorMsg.type = MessageType::ErrorMessage;
			errorMsg.payload = "Failed to load mesh at: " + path;
			//LevelEditorManager::Get().inbox.Push(errorMsg);
		}
	}
}