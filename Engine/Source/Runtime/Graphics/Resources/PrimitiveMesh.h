#ifndef ENGINE_GRAPHICS_RESOURCES_PRIMITIVEMESH_H
#define ENGINE_GRAPHICS_RESOURCES_PRIMITIVEMESH_H

#include <string>

namespace Shark::Math {
    struct Vector3;
}

namespace Shark::Core {
    class GameObject;
}

namespace Shark::Graphics {
    class Material;
    class Mesh;
}

namespace Shark::Graphics {

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
        static Shark::Core::GameObject* CreatePrimitive(Shark::Core::GameObject* obj, Shark::Graphics::PrimitiveType type, Shark::Graphics::Material* mat = nullptr);
        static Shark::Graphics::Mesh* CreateCube();
    private:
        // Debug a face by printing its winding
        static void DebugFace(const std::string& name,
            const Shark::Math::Vector3& v0,
            const Shark::Math::Vector3& v1,
            const Shark::Math::Vector3& v2,
            const Shark::Math::Vector3& v3,
            const Shark::Math::Vector3& expectedNormal);

    };
}

#endif // ENGINE_GRAPHICS_RESOURCES_PRIMITIVEMESH_H