#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "MathUtils.h"

namespace Shark::Math {
	struct Transform
	{
		Vector3 position{ 0.0f, 0.0f, 0.0f };
		Vector3 scale{ 1.0f, 1.0f, 1.0f };
		Vector3 eulerAngle{ 0.0f, 0.0f, 0.0f };

		Quaternion rotation; // Euler angles in degrees (pitch, yaw, roll)

		void Translate(const Vector3& translation) {
			position += translation;
		}

		// Converts Euler degrees to a Quaternion and adds it to current rotation
		void Rotate(const Vector3& eulerAngles) {
			Quaternion delta = Math::FromEulerDegrees(eulerAngles);
			rotation = delta * rotation; // Note: order matters for quaternion multiplication
		}

		// Multiplies current scale by a factor
		void SetScale(const Vector3& newScale) {
			scale = newScale;
		}

		void LookAt(const Vector3& target, const Vector3& up = Vector3(0, 1, 0)) {
			glm::vec3 forward = glm::normalize(glm::vec3(target) - glm::vec3(position));
			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(up)));
			glm::vec3 newUp = glm::cross(right, forward);

			glm::mat3 rotMat(right, newUp, -forward); // -forward because camera looks down -Z
			rotation = glm::quat_cast(rotMat);
		}

		void UpdateQuaternion() {
			rotation = FromEulerDegrees(eulerAngle);
		}
#pragma region Direction Vectors Functions
		Vector3 GetForward() const {
			return rotation * Vector3(0.0f, 0.0f, -1.0f);
		}

		Vector3 GetUp() const {
			return rotation * Vector3(0.0f, 1.0f, 0.0f);
		}
		
		Vector3 GetRight() const {
			return rotation * Vector3(1.0f, 0.0f, 0.0f);
		}

		
#pragma endregion

		void Reset() {
			position = Vector3(0.f, 0.f, 0.f);
			rotation = Quaternion();
			scale = Vector3(1.f, 1.f, 1.f);
		}

		// Returns the model matrix combining position, rotation, and scale
		glm::mat4 GetModelMatrix() const {

			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, glm::vec3(position)); // Translate
			model = model * glm::mat4_cast(glm::quat(rotation)); // Rotate
			model = glm::scale(model, glm::vec3(scale)); // Scale

			return model;
		}

		glm::mat4 GetViewMatrix() const {
			return glm::lookAt(
				static_cast<glm::vec3>(position), 
				static_cast<glm::vec3>(position) + static_cast<glm::vec3>(GetForward()), 
				glm::vec3(GetUp()));
		}
	};

}

#endif