#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Mesh.h"
#include "PrimitiveMesh.h"

#include <unordered_map>
#include <string>


class GameObject;
class Material;

class MeshManager {
public:
	static MeshManager& Get() {
		static MeshManager instance;
		return instance;
	}

	void AddMeshToCache(const std::string& key, Mesh* mesh);

	Mesh* GetMesh(const std::string& filePath);
	Mesh* LoadMesh(PrimitiveType type);
	Mesh* LoadMesh(const std::string& filePath);

	void Shutdown();
	static void CreateFromObj(GameObject* obj, const std::string& filePath, Material* mat = nullptr);

private:

	MeshManager() = default;
	std::unordered_map<std::string, Mesh*> m_MeshCache;
};

#endif