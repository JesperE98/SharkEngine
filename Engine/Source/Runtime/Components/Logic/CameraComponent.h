#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Components/Component.h"
#include <Math/Vector3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Shark::Math { struct Transform; }
namespace Shark::Core { class GameObject; }


namespace Shark::Components {

    class CameraComponent : public Component
    {
    public:
        enum class Mode : uint8_t {
            Free,
            ThirdPerson
        };

        CameraComponent() : CameraComponent(45.0f, 1.77f, 0.1f, 1000.0f) {}

		CameraComponent(float fov, float aspectRatio, float nearClip, float farClip)
			: FOV(fov), AspectRatio(aspectRatio), NearClip(nearClip), FarClip(farClip) {
            UpdateProjectionMatrix();
            tickMode = TickMode::Always;
		}

#pragma region Component Properties
        float FOV = 45.0f;
        float NearClip = 0.1f;
        float FarClip = 1000.0f;
		float AspectRatio = 1.77f; // Default to 16:9
        glm::mat4 m_ProjectionMatrix = 0.1f;

        // Follow-camera fields
        Mode mode                       = Mode::Free;
        Core::GameObject* followTarget  = nullptr;
        Math::Vector3 followOffset      = { 0.0f, 5.0f, 10.0f };
        float followSpeed               = 5.0f;
#pragma endregion

#pragma region Component Functions

        void SetAspectRatio(float aspect);
        void SetFOV(float fov);
		void UpdateProjectionMatrix();

        glm::mat4 GetProjectionMatrix() const;

        // Get the position and orientation from the GameObject's Transform
        glm::mat4 GetViewMatrix() const;

        virtual void Update(float deltaTime) override final;
#pragma endregion

    private:

        bool m_AutoFindTriedPlayer = false;

        glm::mat4 Perspective(float fov, float aspect, float nearPlane, float farPlane) const {
            return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
		}
        void TryAutoFindPlayer();
    };
}

#endif // CAMERACOMPONENT_H