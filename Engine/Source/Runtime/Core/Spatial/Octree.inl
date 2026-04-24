#pragma once

#include "Octree.h"
#include <Core/Utilities/Debug.h>

namespace Shark::Spatial {

	template<typename T>
	Octree<T>::Octree(const AABB& worldBounds, int maxDepth, int maxItemsPerNode)
		: m_MaxDepth(maxDepth), m_MaxItemsPerNode(maxItemsPerNode) {

		m_Root = std::make_unique<Node>();
		m_Root->bounds = worldBounds;
		m_Root->depth = 0;
	}

	template<typename T>
	void Octree<T>::Clear() {
		AABB rootBounds = m_Root->bounds;
		m_Root = std::make_unique<Node>();
		m_Root->bounds = rootBounds;
		m_Root->depth = 0;
		m_ItemLookup.clear();
	}

	template<typename T>
	void Octree<T>::Insert(T payload, const AABB& bounds) {
		if (!m_Root->bounds.Contains(bounds)) {
			SE_WARN(Engine, "Octree: inserted item outside world bounds.");
			return;
		}
		InsertInto(m_Root.get(), payload, bounds);
	}

	template<typename T>
	void Octree<T>::InsertInto(Node* node, T payload, const AABB& bounds) {
		// Try to push deeper if already subdivided
		if (!node->isLeaf) {
			int childIdx = GetChildIndex(node, bounds);
			if (childIdx >= 0) {
				InsertInto(node->children[childIdx].get(), payload, bounds);
				return;
			}

			// Straddle multiple children - store here ("loose" placement)
			node->items.emplace_back(payload, bounds);
			m_ItemLookup[payload] = node;
			return;
		}

		node->items.emplace_back(payload, bounds);
		m_ItemLookup[payload] = node;

		// Subdivide if overflowing and not at max depth
		if (node->items.size() > static_cast<size_t>( m_MaxItemsPerNode )
			&& node->depth < m_MaxDepth) {
			Subdivide(node);
		}
	}

	template<typename T>
	void Octree<T>::Subdivide(Node* node) {
		node->isLeaf = false;

		// Create 8 children
		for (int i = 0; i < 8; ++i) {
			node->children[i] = std::make_unique<Node>();
			node->children[i]->bounds = GetChildBounds(node->bounds, i);
			node->children[i]->depth = node->depth + 1;
		}

		// Redistribute items
		auto oldItems = std::move(node->items);
		node->items.clear();

		for (auto& [payload, bounds] : oldItems) {
			int childIdx = GetChildIndex(node, bounds);
			if (childIdx >= 0) {
				node->children[childIdx]->items.emplace_back(payload, bounds);
				m_ItemLookup[payload] = node->children[childIdx].get();
			} else {
				// Straddles children - keep at this level (loose octree style)
				node->items.emplace_back(payload, bounds);
				m_ItemLookup[payload] = node;
			}
		}
	}

	template<typename T>
	int Octree<T>::GetChildIndex(const Node* node, const AABB& bounds) const {
		Vector3 center = node->bounds.GetCenter();
		int idx = 0;

		if (bounds.min.x >= center.x) idx |= 1;
		else if (bounds.max.x > center.x) return -1; // straddle x

		if (bounds.min.y >= center.y) idx |= 2;
		else if (bounds.max.y > center.y) return -1; // straddle y

		if (bounds.min.z >= center.z) idx |= 4;
		else if (bounds.max.z > center.z) return -1; // straddle z

		return idx;
	}

	template<typename T>
	typename Octree<T>::AABB Octree<T>::GetChildBounds(const AABB& parent, int childIndex) const {
		Vector3 center = parent.GetCenter();
		AABB child;

		child.min.x = ( childIndex & 1 ) ? center.x : parent.min.x;
		child.max.x = ( childIndex & 1 ) ? parent.max.x : center.x;

		child.min.y = ( childIndex & 2 ) ? center.y : parent.min.y;
		child.max.y = ( childIndex & 2 ) ? parent.max.y : center.y;

		child.min.z = ( childIndex & 4 ) ? center.z : parent.min.z;
		child.max.z = ( childIndex & 4 ) ? parent.max.z : center.z;

		return child;
	}

	template<typename T>
	bool Octree<T>::Remove(T payload) {
		auto it = m_ItemLookup.find(payload);
		if (it == m_ItemLookup.end()) return false;

		Node* node = it->second;
		auto& items = node->items;
		auto itemIt = std::find_if(items.begin(), items.end(), [&](const auto& pair) {
			return pair.first == payload;
		});

		if (itemIt != items.end()) {
			items.erase(itemIt);
			m_ItemLookup.erase(it);
			return true;
		}
		return false;
	}

	template<typename T>
	void Octree<T>::ForEachNodeBounds(const std::function<void(const AABB&)>& fn) const {
		VisitNodes(m_Root.get(), fn);
	}

	template<typename T>
	void Octree<T>::VisitNodes(const Node* node, const std::function<void(const AABB&)>& fn) const {

		if (!node) return;
		fn(node->bounds);
		if (!node->isLeaf) {
			for (const auto& child : node->children) {
				VisitNodes(child.get(), fn);
			}
		}
	}
}