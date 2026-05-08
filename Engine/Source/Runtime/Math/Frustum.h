#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm/glm.hpp>
#include <Components/Physics/AABBComponent.h>

namespace Shark::Math {

	struct Plane {
		glm::vec3 normal;
		float distance;

		float DistanceTo(const glm::vec3& point) const {
			return glm::dot(normal, point) + distance;
		}
	};

	/**
	 * @brief Determines whether an axis-aligned bounding box (AABB) is at least partially inside or intersects the view frustum defined by this Frustum's planes.
	 */
	struct Frustum {
		Plane planes[6];

		/**
		 * @brief Extracts the six view frustum planes (left, right, bottom, top, near, far) from a view-projection matrix, normalizes them, and stores them in the object's planes array.
		 * @param vp The combined view-projection matrix (glm::mat4) used to compute the frustum planes.
		 */
		void ExtractFromViewProjection(const glm::mat4& vp) {
			planes[0].normal = { vp[0][3] + vp[0][0], vp[1][3] + vp[1][0],  vp[2][3] + vp[2][0] };
			planes[0].distance = vp[3][3] + vp[3][0];

			planes[1].normal = { vp[0][3] - vp[0][0], vp[1][3] - vp[1][0],  vp[2][3] - vp[2][0] };
			planes[1].distance = vp[3][3] - vp[3][0];

			planes[2].normal = { vp[0][3] + vp[0][1], vp[1][3] + vp[1][1],  vp[2][3] + vp[2][1] };
			planes[2].distance = vp[3][3] + vp[3][1];

			planes[3].normal = { vp[0][3] - vp[0][1], vp[1][3] - vp[1][1],  vp[2][3] - vp[2][1] };
			planes[3].distance = vp[3][3] - vp[3][1];

			planes[4].normal = { vp[0][3] + vp[0][2], vp[1][3] + vp[1][2],  vp[2][3] + vp[2][2] };
			planes[4].distance = vp[3][3] + vp[3][2];

			planes[5].normal = { vp[0][3] - vp[0][2], vp[1][3] - vp[1][2],  vp[2][3] - vp[2][2] };
			planes[5].distance = vp[3][3] - vp[3][2];

			for (int i = 0; i < 6; i++) {
				float length = glm::length(planes[i].normal);
				planes[i].normal /= length;
				planes[i].distance /= length;
			}
		}

		/**
		 * @brief Determines whether an axis-aligned bounding box (AABB) is inside or intersects the view frustum defined by the object's six planes.
		 * @param aabb The axis-aligned bounding box to test for visibility.
		 * @return true if the AABB is at least partially inside or intersects the frustum (not fully outside any plane); false otherwise.
		 */
		bool IsAABBVisible(const Components::AABB& aabb) const {
			for (int i = 0; i < 6; i++) {
				const float epsilon = 0.5f; // small tolerance

				// Positive vertex - farthest along the plane normal
				glm::vec3 pVertex;
				pVertex.x = ( planes[i].normal.x >= 0 ) ? aabb.max.x : aabb.min.x;
				pVertex.y = ( planes[i].normal.y >= 0 ) ? aabb.max.y : aabb.min.y;
				pVertex.z = ( planes[i].normal.z >= 0 ) ? aabb.max.z : aabb.min.z;


				// Negative vertex - closest along the plane normal
				//glm::vec3 nVertex;
				//nVertex.x = ( planes[i].normal.x >= 0 ) ? aabb.min.x : aabb.max.x;
				//nVertex.y = ( planes[i].normal.y >= 0 ) ? aabb.min.y : aabb.max.y;
				//nVertex.z = ( planes[i].normal.z >= 0 ) ? aabb.min.z : aabb.max.z;

				//// If the negative vertex is outside, the whole AABB is outside
				//if (planes[i].DistanceTo(nVertex) > 0.0f) {

				//}

				if (planes[i].DistanceTo(pVertex) < -epsilon) {
					return false;
				}
			}

			return true;
		}
	};
}

#endif