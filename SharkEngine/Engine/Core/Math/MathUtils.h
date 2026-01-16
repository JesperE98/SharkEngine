#pragma once

#include <cmath>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

namespace Math {

#pragma region Vector2 Helper Functions

	/**
	* @brief Computes the dot product of two 2D vectors.
	* @param a First vector.
	* @param b Second vector.
	* @return Scalar dot product.
	*/
	inline float Dot(const Vector2& a, const Vector2& b) {
		return glm::dot(glm::vec2(a), glm::vec2(b));
	}

	/**
	* @brief Computes the length (magnitude) of a 2D vector.
	* @param v Input vector.
	* @return Length of the vector.
	*/
	inline float Length(const Vector2& v) {
		return glm::length(glm::vec2(v));
	}

	/**
	* @brief Computes the distance between two 2D vectors.
	* @param a First vector.
	* @param b Second vector.
	* @return Distance between a and b.
	*/
	inline float Distance(const Vector2& a, const Vector2& b) {
		return glm::distance(glm::vec2(a), glm::vec2(b));
	}

	/**
	* @brief Normalizes a 2D vector to unit length.
	* @param v Input vector.
	* @return Normalized vector (length = 1), or zero vector if input length is 0.
	*/
	inline Vector2 Normalize(const Vector2& v) {
		return Vector2(glm::normalize(glm::vec2(v)));
	}

	/**
	* @brief Linearly interpolates between two 2D vectors.
	* @param a Start vector.
	* @param b End vector.
	* @param t Interpolation factor in [0,1].
	* @return Interpolated vector.
	*/
	inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
		return Vector2(glm::mix(glm::vec2(a), glm::vec2(b), t));
	}
	
#pragma endregion

#pragma region Vector3 Helper Functions
	/**
	* @brief Computes the dot product of two 3D vectors.
	* @param a First vector.
	* @param b Second vector.
	* @return Scalar dot product.
	*/
	inline float Dot(const Vector3& a, const Vector3& b) {
		return glm::dot(glm::vec3(a), glm::vec3(b));
	}

	/**
	* @brief Computes the length (magnitude) of a 3D vector.
	* @param v Input vector.
	* @return Length of the vector.
	*/
	inline float Length(const Vector3& v) {
		return glm::length(glm::vec3(v));
	}

	/**
	* @brief Computes the distance between two 3D vectors.
	* @param a First vector.
	* @param b Second vector.
	* @return Distance between a and b.
	*/
	inline float Distance(const Vector3& a, const Vector3& b) {
		return glm::distance(glm::vec3(a), glm::vec3(b));
	}

	/**
	* @brief Normalizes a 3D vector to unit length.
	* @param v Input vector.
	* @return Normalized vector (length = 1), or zero vector if input length is 0.
	*/
	inline Vector3 Normalize(const Vector3& v) {
		return Vector3(glm::normalize(glm::vec3(v)));
	}

	/**
	* @brief Computes the cross product of two 3D vectors.
	* @param a First vector.
	* @param b Second vector.
	* @return Vector perpendicular to both a and b.
	*/
	inline Vector3 Cross(const Vector3& a, const Vector3& b) {
		return Vector3(glm::cross(glm::vec3(a), glm::vec3(b)));
	}

	/**
	* @brief Linearly interpolates between two 3D vectors.
	* @param a Start vector.
	* @param b End vector.
	* @param t Interpolation factor in [0,1].
	* @return Interpolated vector.
	*/
	inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
		return Vector3(glm::mix(glm::vec3(a), glm::vec3(b), t));
	}

#pragma endregion

#pragma region Vector4 Helper Functions
	/**
	* @brief Computes the dot product of two 4D vectors.
	* @param a First vector.
	* @param b Second vector.
	* @return Scalar dot product.
	*/
	inline float Dot(const Vector4& a, const Vector4& b) {
		return glm::dot(glm::vec4(a), glm::vec4(b));
	}

	/**
	* @brief Computes the length (magnitude) of a 4D vector.
	* @param v Input vector.
	* @return Length of the vector.
	*/
	inline float Length(const Vector4& v) {
		return glm::length(glm::vec4(v));
	}

	/**
	* @brief Computes the distance between two 4D vectors.
	* @param a First vector.
	* @param b Second vector.
	* @return Distance between a and b.
	*/
	inline float Distance(const Vector4& a, const Vector4& b) {
		return glm::distance(glm::vec4(a), glm::vec4(b));
	}

	/**
	* @brief Normalizes a 4D vector to unit length.
	* @param v Input vector.
	* @return Normalized vector (length = 1), or zero vector if input length is 0.
	*/
	inline Vector4 Normalize(const Vector4& v) {
		return Vector4(glm::normalize(glm::vec4(v)));
	}

	/**
	* @brief Linearly interpolates between two 4D vectors.
	* @param a Start vector.
	* @param b End vector.
	* @param t Interpolation factor in [0,1].
	* @return Interpolated vector.
	*/
	inline Vector4 Lerp(const Vector4& a, const Vector4& b, float t) {
		return Vector4(glm::mix(glm::vec4(a), glm::vec4(b), t));
	}
#pragma endregion

#pragma region Quaternion Helper Functions
	/**
	* @brief Normalizes a quaternion to unit length.
	* @param q Input quaternion.
	* @return Normalized quaternion.
	*/
	inline Quaternion Normalize(const Quaternion& q) {
		return Quaternion(glm::normalize(glm::quat(q)));
	}

	/**
	* @brief Performs spherical linear interpolation (slerp) between two quaternions.
	* @param a Starting quaternion.
	* @param b Ending quaternion.
	* @param t Interpolation factor [0,1].
	* @return Interpolated quaternion.
	*/
	inline Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
		return Quaternion(glm::slerp(glm::quat(a), glm::quat(b), t));
	}

	/**
	* @brief Creates a quaternion from Euler angles in degrees.
	* @param eulerDegrees Euler angles (pitch, yaw, roll) in degrees.
	* @return Quaternion representing the rotation.
	*/
	inline Quaternion FromEulerDegrees(const Vector3& eulerDegrees) {
		//return Quaternion(glm::quat(glm::radians(glm::vec3(eulerDegrees))));
		return Quaternion(glm::yawPitchRoll(
			glm::radians(eulerDegrees.y),
			glm::radians(eulerDegrees.x),
			glm::radians(eulerDegrees.z)
		));
	}

	/**
	* @brief Converts a quaternion to Eualer angles in degrees.
	* @param q Input quaternion.
	* @return Euler angles (pitch, yaw, roll) in degrees as a Vector3.
	*/
	inline Vector3 ToEulerDegrees(const Quaternion& q) {
		return Vector3(glm::degrees(glm::eulerAngles(glm::quat(q))));
	}

	/**
	* @brief Rotates a vector by a quaternion.
	* @param q Rotation quaternion.
	* @param v Input vector.
	* @return Rotated vector.
	*/
	inline Vector3 RotateVector(const Quaternion& q, const Vector3& v) {
		return Vector3(glm::rotate(glm::quat(q), glm::vec3(v)));
	}
#pragma endregion
}