#include "OctreeSystem.h"
#include "Scene/Scene.h"
#include "Core/GameObject.h"
#include "Core/Utilities/Debug.h"
#include "Components/Physics/AABBComponent.h"
#include "Math//Frustum.h"



namespace Shark::Spatial {

	OctreeSystem& OctreeSystem::Get() {
		static OctreeSystem instance;
		return instance;
	}

	void OctreeSystem::Initialize(const Components::AABB& worldBounds, int maxDepth, int maxItemsPerNode) {
		m_WorldBounds		= worldBounds;
		m_MaxDepth			= maxDepth;
		m_MaxItemsPerNode	= maxItemsPerNode;

		m_Tree = std::make_unique<Octree<Core::GameObject*>>(worldBounds, maxDepth, maxItemsPerNode);

		m_Initialized = true;

		SE_LOG(Engine, "OctreeSystem initialized - world bounds: ({},{},{}) to ({},{},{}), maxDepth={}, maxItems={}",
			   worldBounds.min.x, worldBounds.min.y, worldBounds.min.z,
			   worldBounds.max.x, worldBounds.max.y, worldBounds.max.z,
			   maxDepth, maxItemsPerNode);
	}

	void OctreeSystem::RebuildFromScene(Scene* scene) {
		if (!m_Initialized) {
			SE_WARN(Engine, "OctreeSystem:RebuildFromScene called before Initialize.");
			return;
		}

		if (!scene) {
			Clear();
			return;
		}

		m_Tree->Clear();

		int inserted = 0;
		for (Core::GameObject* obj : scene->GetGameObjects()) {
			if (!obj) continue;
			Components::AABBComponent* aabb = obj->GetComponent<Components::AABBComponent>();
			if (!aabb) continue;

			m_Tree->Insert(obj, aabb->GetWorldAABB());
			++inserted;
		}

		SE_LOG(Engine, "OctreeSystem rebuild: {} objects inserted.", inserted);
	}

	void OctreeSystem::Insert(Core::GameObject* obj) {
		if (!m_Tree || !obj) return;

		Components::AABBComponent* aabb = obj->GetComponent<Components::AABBComponent>();
		if (!aabb) return;

		m_Tree->Insert(obj, aabb->GetWorldAABB());
	}
	
	bool OctreeSystem::Remove(Core::GameObject* obj) {
		if (!m_Tree || !obj) return false;
		return m_Tree->Remove(obj);
	}
	
	void OctreeSystem::Clear() {
		if (m_Tree) m_Tree->Clear();
	}
	
	std::vector<Core::GameObject*> OctreeSystem::QueryRegion(const Components::AABB& region) const {
		// For now - placeholder. Phase 3 (frustrum culling) will need this fleshed out.
		// Fallback: empty result.
		// TODO: implement actual region query in Octree<T>
		std::vector<Core::GameObject*> result;
		return result;
	}
	
	size_t OctreeSystem::Size() const {
		return m_Tree ? m_Tree->Size() : 0;
	}
	
	void OctreeSystem::ForEachNodeBoundsWithDepth(const std::function<void(const Components::AABB&, int)>& fn) const {
		if (m_Tree) m_Tree->ForEachNodeBoundsWithDepth(fn);
	}
	std::vector<Core::GameObject*> OctreeSystem::QueryFrustum(const Math::Frustum& frustum) const {
		std::vector<Core::GameObject*> result;

		if (m_Tree) {
			m_Tree->QueryFrustum(frustum, result);
		}

		return result;
	}
}