#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Components/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Shark::Math { struct Transform; }

namespace Shark::Components {

    class CameraComponent : public Component
    {
    public:
		CameraComponent(float fov, float aspectRatio, float nearClip, float farClip)
			: FOV(fov), AspectRatio(aspectRatio), NearClip(nearClip), FarClip(farClip) {
            UpdateProjectionMatrix();
            tickMode = TickMode::EditOnly;
		}

#pragma region Component Properties
        float FOV = 45.0f;
        float NearClip = 0.1f;
        float FarClip = 1000.0f;
		float AspectRatio = 1.77f; // Default to 16:9
        glm::mat4 m_ProjectionMatrix = 0.1f;
#pragma endregion

#pragma region Component Functions

        void SetAspectRatio(float aspect);
        void SetFOV(float fov);
		void UpdateProjectionMatrix();

        glm::mat4 GetProjectionMatrix() const;

        // Get the position and orientation from the GameObject's Transform
        glm::mat4 GetViewMatrix() const;

#pragma endregion

    private:

        glm::mat4 Perspective(float fov, float aspect, float nearPlane, float farPlane) const {
            return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
		}
    };
}

#endif // CAMERACOMPONENT_H