#ifndef SHARK_OCTREE_SYSTEM_H
#define SHARK_OCTREE_SYSTEM_H

#include "Octree.h"

#include <memory>

namespace Shark::Core {
	class GameObject;
}

namespace Shark {
	class Scene;
}

namespace Shark::Components {
	struct AABB;
	class AABBComponent;
}

namespace Shark::Math {
	struct Frustum;
}

namespace Shark::Spatial {


	class OctreeSystem {
	public:
		static OctreeSystem& Get();
		
		OctreeSystem(const OctreeSystem&)				= delete;
		OctreeSystem& operator=(const OctreeSystem&)	= delete;

		/**
		 * @brief Initializes the spatial structure using the provided world bounds and configuration limits.
		 * @param worldBounds Axis-aligned bounding box that defines the world bounds to initialize (passed by const reference).
		 * @param maxDepth Maximum allowed depth of the structure; controls recursion depth (default 5).
		 * @param maxItemsPerNode Maximum number of items per node before a split occurs (default 8).
		 */
		void Initialize(const Components::AABB& worldBounds,
						int maxDepth = 5,
						int maxItemsPerNode = 8);

		/**
		 * @brief Rebuilds internal data structures from the provided scene.
		 * @param scene Pointer to the Shark::Scene to use as the source for rebuilding.
		 */
		void RebuildFromScene(Shark::Scene* scene);

		/**
		 * @brief Inserts the specified game object.
		 * @param obj Pointer to the Core::GameObject to insert.
		 */
		void Insert(Core::GameObject* obj);
		/**
		 * @brief Removes the specified Core::GameObject from its managing context.
		 * @param obj Pointer to the Core::GameObject to remove. Behavior is undefined if nullptr.
		 */
		bool Remove(Core::GameObject* obj);

		void Clear();

		/**
		 * @brief Returns all game objects that intersect or are contained within the specified axis-aligned bounding box.
		 * @param region Axis-aligned bounding box that defines the query area. Objects that intersect or lie inside this box are included in the result.
		 * @return A vector of pointers to Core::GameObject representing objects found in or intersecting the region. The vector may be empty if no objects match.
		 */
		std::vector<Core::GameObject*> QueryRegion(const Components::AABB& region) const;

		/**
		 * @brief Returns the size associated with the object without modifying it.
		 * @return The size as a size_t value (typically a count, such as number of elements or bytes).
		 */
		size_t Size() const;


		bool IsDebugDrawEnabled() const { return m_DebugDraw; }
		void SetDebugDrawEnabled(bool enabled) { m_DebugDraw = enabled; }

		/**
		 * @brief Invokes a callback for each node's axis-aligned bounding box and its depth in the hierarchy.
		 * @param fn Callback invoked for each node; receives (const Components::AABB& bounds, int depth).
		 */
		void ForEachNodeBoundsWithDepth(const std::function<void(const Components::AABB&, int)>& fn) const;

		std::vector<Core::GameObject*> QueryFrustum(const Math::Frustum& frustum) const;
	private:

		OctreeSystem() = default;
		~OctreeSystem() = default;

		std::unique_ptr<Octree<Core::GameObject*>> m_Tree;

		Components::AABB m_WorldBounds;
		int m_MaxDepth = 5;
		int m_MaxItemsPerNode = 8;
		bool m_Initialized = false;
		bool m_DebugDraw = true;
	};
}

#endif