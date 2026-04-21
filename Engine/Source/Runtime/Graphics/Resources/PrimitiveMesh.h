#ifndef ENGINE_GRAPHICS_RESOURCES_PRIMITIVEMESH_H
#define ENGINE_GRAPHICS_RESOURCES_PRIMITIVEMESH_H

#include <string>

namespace Shark::Math { struct Vector3; }
namespace Shark::Core { class GameObject; }

namespace Shark::Graphics {

    class Material;
    class Mesh;

    enum class PrimitiveType : uint8_t {
        None,
        Cube,
        Sphere, // For future use
        Cylinder, // For future use
        Plane // For future use
    };

    class PrimitiveMesh
    {
    public:
        static Core::GameObject* CreatePrimitive(Core::GameObject* obj, PrimitiveType type, Material* mat = nullptr);
        static Graphics::Mesh* CreateCube();
    private:
        // Debug a face by printing its winding
        static void DebugFace(const std::string& name,
            const Math::Vector3& v0,
            const Math::Vector3& v1,
            const Math::Vector3& v2,
            const Math::Vector3& v3,
            const Math::Vector3& expectedNormal);

    };
}

#endif // ENGINE_GRAPHICS_RESOURCES_PRIMITIVEMESH_H