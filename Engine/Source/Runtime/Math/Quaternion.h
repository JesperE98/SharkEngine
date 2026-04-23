#pragma once

#include "Vector3.h"
#include <glm/gtc/quaternion.hpp>

namespace Shark::Math {

	struct Vector3;

	struct Quaternion {
		float w, x, y, z;

#pragma region Constructors
		Quaternion() : w(1), x(0), y(0), z(0) {}
		Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
		Quaternion(const glm::quat& q) : w(q.w), x(q.x), y(q.y), z(q.z) {}

		Quaternion(const Vector3& eulerDegrees) {
			glm::quat q = glm::quat(glm::radians(glm::vec3(
				eulerDegrees.x, eulerDegrees.y, eulerDegrees.z
			)));

			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
		}
#pragma endregion

#pragma region Operators

		operator glm::quat() const { return glm::quat(w, x, y, z); }

		Quaternion operator*(const Quaternion& other) const { return Quaternion(glm::quat(*this) * glm::quat(other)); }
		Quaternion& operator=(const Vector3& eulerDegrees) {
			glm::quat q = glm::quat(glm::radians(glm::vec3(
				eulerDegrees.x, eulerDegrees.y, eulerDegrees.z
			)));
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
			
			return *this;
		}

		Vector3 operator*(const Vector3& other) const {
			glm::vec3 rotated = glm::quat(*this) * glm::vec3(other.x, other.y, other.z);
			return Vector3(rotated.x, rotated.y, rotated.z);
		}

#pragma endregion

#pragma region Static Constants

		static const Quaternion Identity;

#pragma endregion
	};
}