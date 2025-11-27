#pragma once

#include <Core/Math/MathUtils.h>
#include <string>
class Mesh;
class GameObject;
class Material;

class PrimitiveMesh
{
public:
	static GameObject* CreatePrimitveCube(GameObject* obj, Material* mat = nullptr);

private:
    // Debug a face by printing its winding
    static void DebugFace(const std::string& name,
        const Vector3& v0,
        const Vector3& v1,
        const Vector3& v2,
        const Vector3& v3, 
        const Vector3& expectedNormal);

	static Mesh* CreateCube();
};

