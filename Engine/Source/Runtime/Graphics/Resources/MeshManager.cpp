#include "MeshManager.h"
#include "Components/Rendering/MeshRendererComponent.h"
#include "Core/GameObject.h"
#include "Graphics/Resources/Material.h"
#include "Graphics/Resources/Mesh.h"
#include "Graphics/Resources/PrimitiveMesh.h"

namespace Shark::Resources {

	using Core::Message;
	using Core::MessageSystem;
	using Core::EventType;
	using Components::MeshRendererComponent;
	using Core::GameObject;
	using Graphics::Material;
	using Graphics::Mesh;
	using Graphics::PrimitiveType;
	using Graphics::PrimitiveMesh;

	void MeshManager::Update(float DeltaTime)
	{
		Message msg;
		while (inbox.Pop(msg)) {
			if (msg.type == EventType::LoadModel) {
				ProcessLoadRequest(msg.payload);
			}
			else if (msg.type == EventType::LoadPrimitiveType) {
				ProcessLoadRequest(static_cast<PrimitiveType>(std::stoi(msg.payload)));
			}
		}
	}

	void MeshManager::AddMeshToCache(const std::string& key, Mesh* mesh)
	{
		if (m_MeshCache.find(key) == m_MeshCache.end()) {
			m_MeshCache[key] = mesh;
		}
	}

	void MeshManager::SetResponseTarget(MessageSystem* target)
	{
		m_ResponseTarget = target;
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
			SE_WARN(Rendering, "Unknown PrimitiveType!");
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

		SE_WARN(Rendering, "Cache miss! Loading {} from disk...", filePath);
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
			SE_ERR(Rendering, "Passing null GameObject to CreateFromObj");
			return;
		}

		Mesh* newMesh = MeshManager::Get().LoadMesh(filePath);
		if (newMesh) {
			Material* material = mat ? mat : new Material(); // if none mat wasn't provided, create default material
			obj->AddComponent<MeshRendererComponent>(newMesh, material);
			SE_LOG(Rendering, "Loaded mesh from {} and attached to GameObject: {}", filePath, obj->GetName());
		}
		else {
			SE_ERR(Rendering, "Failed to load mesh from {}", filePath);
		}
	}

	template<>
	void MeshManager::ProcessLoadRequest<std::string>(const std::string& path)
	{
		SE_PROC(Rendering, "Handshake sent to {}", path);
		Mesh* loadedMesh = LoadMesh(path);

		if (loadedMesh) {
			Message reply;
			reply.type = EventType::ModelLoaded;
			reply.payload = path;
			reply.data = static_cast<void*>(loadedMesh);

			if (m_ResponseTarget) m_ResponseTarget->Push(reply);

		}
		else {
			Message errorMsg;
			errorMsg.type = EventType::ErrorMessage;
			errorMsg.payload = "Failed to load mesh at: " + path;
			
			if (m_ResponseTarget) m_ResponseTarget->Push(errorMsg);

			SE_ERR(Rendering, "Failed to load mesh at: {}", path);
		}
	}

	template<>
	void MeshManager::ProcessLoadRequest<PrimitiveType>(const PrimitiveType& type)
	{
		SE_PROC(Rendering, "Received request to load primitive type: {}", static_cast<int>(type));
		Mesh* loadedMesh = LoadMesh(type);

		if (loadedMesh) {
			Message reply;
			reply.type = EventType::PrimitiveTypeLoaded;
			reply.payload = std::to_string(static_cast<int>(type));
			reply.data = static_cast<void*>(loadedMesh);

			if (m_ResponseTarget) m_ResponseTarget->Push(reply);
		}
		else {
			Message errorMsg;
			errorMsg.type = EventType::ErrorMessage;
			errorMsg.payload = "Failed to load primitive type: " + std::to_string(static_cast<int>(type));
			if (m_ResponseTarget) m_ResponseTarget->Push(errorMsg);
			SE_ERR(Rendering, "Failed to load primitive type : {}", static_cast<int>(type));
		}
	}
}