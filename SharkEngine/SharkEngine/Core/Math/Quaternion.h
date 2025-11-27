#pragma once
#include <glm/gtc/quaternion.hpp>

struct Quaternion {
	float w, x, y, z;

#pragma region Constructors
	Quaternion() : w(1), x(0), y(0), z(0) {}
	Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
	Quaternion(const glm::quat& q) : w(q.w), x(q.x), y(q.y), z(q.z) {}
#pragma endregion

#pragma region Operators
	operator glm::quat() const { return glm::quat(w, x, y, z); }
	Quaternion operator*(const Quaternion& other) const { return Quaternion(glm::quat(*this) * glm::quat(other)); }
#pragma endregion

#pragma region Static Constants

	static const Quaternion Identity;

#pragma endregion
};