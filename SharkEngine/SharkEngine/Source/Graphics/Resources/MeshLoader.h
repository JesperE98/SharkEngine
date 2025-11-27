#pragma once

#include <string>

class GameObject;
class Material;

class MeshLoader {
public:
	static void CreateFromObj(GameObject* obj, const std::string& filePath, Material* mat = nullptr);
};