#include "Core/Components/MeshRendererComponent.h"
#include "Core/Utilities/Time.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshManager.h"
#include <Core/Entities/GameObject.h>
#include <iostream>
#include <ostream>
#include <string>

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
		std::cerr << Time::CreateTimeStamp() << ": [MeshManager] Unknown PrimitiveType!" << std::endl;
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
		if(pair.second) {
			delete pair.second;
		}
	}
}

void MeshManager::CreateFromObj(GameObject* obj, const std::string& filePath, Material* mat) {

	if (!obj) {
		std::cerr << Time::CreateTimeStamp() << ": [MeshManager] Error: Passing null GameObject to CreateFromObj" << std::endl;
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