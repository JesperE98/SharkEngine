#pragma once

#include <glm/vec2.hpp>

struct Vector2 {

	float x, y;

	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y){}
	Vector2(const glm::vec2& v) : x(v.x), y(v.y){}

#pragma region Operators
	operator glm::vec2() const { return glm::vec2(x, y); }

	Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
	Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }
	Vector2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
	Vector2 operator/(float scalar) const { return { x / scalar, y / scalar }; }

	Vector2& operator +=(const Vector2& other) { x += other.x; y += other.y; return *this; }
	Vector2& operator -=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
#pragma endregion

#pragma region Static Constants
	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 Up;
	static const Vector2 Down;
	static const Vector2 Left;
	static const Vector2 Right;
#pragma endregion
};