#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"

#include "Core/Math/MathUtils.h"

inline glm::mat4 Translate(const glm::mat4& m, const Vector3& v) {
	return glm::translate(m, glm::vec3(v));
}

// Apply incremental rotation
inline glm::mat4 Rotate(const glm::mat4& m, const Quaternion& q) {
	return m * glm::mat4_cast(glm::quat(q));
}

inline glm::mat4 Scale(const glm::mat4& m, const Vector3& v) {
	return glm::scale(m, glm::vec3(v));
}

struct Transform
{
	Vector3 position{ 0.0f, 0.0f, 0.0f };
	Vector3 scale	{ 1.0f, 1.0f, 1.0f };
	Quaternion rotation; // Euler angles in degrees (pitch, yaw, roll)
	// Returns the model matrix combining position, rotation, and scale
	glm::mat4 GetModelMatrix() const {

		glm::mat4 model = glm::mat4(1.0f);

		model = Translate(model, position); // Translate
		model = Rotate(model, rotation); // Rotate
		model = Scale(model, scale); // Scale

		return model;
	}

	inline glm::mat4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up) const {
		return glm::lookAt(glm::vec3(eye), glm::vec3(center), glm::vec3(up));
	}

	inline glm::mat4 Perspective(const float fov, const float aspect, const float near, const float far) const {
		return glm::perspective(fov, aspect, near, far);
	}

	void Reset() {
		position	= Vector3(0.f, 0.f, 0.f);
		rotation	= Quaternion();
		scale		= Vector3(1.f, 1.f, 1.f);
	}
};

#endif