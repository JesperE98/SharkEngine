#pragma once

#include <glm/vec3.hpp>
#include <iostream>

struct Vector3 {

	float x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(const glm::vec3& v) : x(v.x), y(v.y), z(v.z) {}

#pragma region Operators

	operator glm::vec3() const { return glm::vec3(x, y, z); }

	Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
	Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
	Vector3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Vector3 operator/(float scalar) const { return { x / scalar, y / scalar, z / scalar }; }

	Vector3& operator +=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
	Vector3& operator -=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
#pragma endregion

#pragma region Static Constants
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Left;
	static const Vector3 Right;
	static const Vector3 Forward;
	static const Vector3 Backward;
#pragma endregion
};

inline std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
	os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}