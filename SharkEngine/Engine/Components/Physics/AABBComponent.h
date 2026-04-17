#ifndef AABB_COMPONENT_H
#define AABB_COMPONENT_H

#include "Components/Component.h"
#include "Math/Vector3.h"

namespace Shark::Components {
	struct AABB {
		Shark::Math::Vector3 min{ 0.0f, 0.0f, 0.0f };
		Shark::Math::Vector3 max{ 0.0f, 0.0f, 0.0f };

		/*
		*@brief Checks if this AABB intersects with another AABB.
		* @param other The other AABB to check against.
		* @return True if the AABBs intersect, false otherwise.
		*/
		bool Intersects(const AABB& other) const {
			return (min.x <= other.max.x && max.x >= other.min.x) &&
				   (min.y <= other.max.y && max.y >= other.min.y) &&
				   (min.z <= other.max.z && max.z >= other.min.z);
		}

		/*
		*@brief Get overlap depth between this AABB and another AABB. This is useful for resolving collisions.
		* @param other The other AABB to check against.
		* @return A Vector3 representing the penetration depth along each axis.
			Positive values indicate overlap, negative values indicate separation.
		*/
		Shark::Math::Vector3 GetPenetrationDepth(const AABB& other) {
			float dx = std::min(max.x, other.max.x) - std::max(min.x, other.min.x);
			float dy = std::min(max.y, other.max.y) - std::max(min.y, other.min.y);
			float dz = std::min(max.z, other.max.z) - std::max(min.z, other.min.z);
			
			return Shark::Math::Vector3(dx, dy, dz);
		}

		/*
		* @brief Get the center point of the AABB.
		* @return A Vector3 representing the center of the AABB.
		*/
		Shark::Math::Vector3 GetCenter() const {
			return (min + max) * 0.5f;
		}

		/*
		* @brief Get the size (width, height, depth) of the AABB.
		* @return A Vector3 representing the size of the AABB along each axis.
		*/
		Shark::Math::Vector3 GetSize() const {
			return max - min;
		}
	};

	class AABBComponent : public Component {
	public:
		Shark::Math::Vector3 m_Offset{ 0.0f, 0.0f, 0.0f };
		Shark::Math::Vector3 m_Extents{ 0.5f, 0.5f, 0.5f };
		bool bIsStatic{ false }; // static = Immoveable collider, dynamic = Movable collider

		AABBComponent() = default;
		explicit AABBComponent(const Shark::Math::Vector3& extents, bool isStatic = true)
			: m_Extents(extents), bIsStatic(isStatic) {}

		/*
		* @return Get the AABB in world space, taking into account the GameObject's transform and the component's offset.
		*/
		AABB GetWorldAABB() const;

		void Update(float deltaTime) override final {}
	};
}
#endif // AABB_COMPONENT_H