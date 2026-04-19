#pragma once

#include <vec4.hpp>

namespace Shark::Math {
	struct Vector4 {

		float x, y, z, w;

		Vector4() : x(0), y(0), z(0), w(0) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vector4(const glm::vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

#pragma region Operators
		operator glm::vec4() const { return glm::vec4(x, y, z, w); }

		Vector4 operator+(const Vector4& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
		Vector4 operator-(const Vector4& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
		Vector4 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
		Vector4 operator/(float scalar) const { return { x / scalar, y / scalar, z / scalar, w / scalar }; }

		Vector4& operator +=(const Vector4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
		Vector4& operator -=(const Vector4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
#pragma endregion

#pragma region Static Constants
		static const Vector4 Zero;
		static const Vector4 One;
#pragma endregion
	};
}