#pragma once

#include <Core/Math/MathUtils.h>
#include <string>
class Mesh;
class GameObject;
class Material;

enum class PrimitiveType {
    None,
    Cube,
    Sphere, // For future use
	Cylinder, // For future use
	Plane // For future use
};

class PrimitiveMesh
{
public:
	static GameObject* CreatePrimitive(GameObject* obj, PrimitiveType type, Material* mat = nullptr);
    static Mesh* CreateCube();
private:
    // Debug a face by printing its winding
    static void DebugFace(const std::string& name,
        const Vector3& v0,
        const Vector3& v1,
        const Vector3& v2,
        const Vector3& v3, 
        const Vector3& expectedNormal);

};

