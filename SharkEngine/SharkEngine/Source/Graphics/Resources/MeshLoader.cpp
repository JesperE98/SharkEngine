#include "MeshLoader.h"
#include "Core/Components/MeshRendererComponent.h"
#include "Core/Utilities/Time.h"

void MeshLoader::CreateFromObj(GameObject* obj, const std::string& filePath, Material* mat) {
	Mesh* mesh = new Mesh();
	if (mesh->LoadMeshFromModel(filePath)) {
		Material* material = mat ? mat : new Material(); // if none mat wasn't provided, create default material
		obj->AddComponent(new MeshRendererComponent(obj, mesh, material));
		std::cout << Time::CreateTimeStamp() << ": MeshLoader - Loaded mesh from " << filePath << " and attached to GameObject: " << obj->GetName() << std::endl;
	}
	else {
		std::cerr << Time::CreateTimeStamp() << ": MeshLoader - Failed to load mesh from " << filePath << std::endl;
	}
}