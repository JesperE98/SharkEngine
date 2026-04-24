#ifndef SHARK_OCTREE_H
#define SHARK_OCTREE_H

#include <array>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <Components/Physics/AABBComponent.h>
#include <Math/Vector3.h>

namespace Shark::Spatial {

	template<typename T>
	class Octree {
	public:
		using AABB		= Components::AABB;
		using Vector3	= Math::Vector3;

		explicit Octree(
			const AABB& worldBounds,
			int maxDepth = 5,
			int maxItemsPerNode = 8);

		~Octree() = default;

		void Clear();
		void Insert(T payload, const AABB& bounds);
		bool Remove(T payload);
		size_t Size() const { return m_ItemLookup.size(); }
		void ForEachNodeBounds(const std::function<void(const AABB&)>& fn) const;

	private:
		struct Node {
			AABB bounds;
			int depth = 0;
			bool isLeaf = true;
			std::vector<std::pair<T, AABB>> items;
			std::array<std::unique_ptr<Node>, 8> children;
		};

		std::unique_ptr<Node> m_Root;
		int m_MaxDepth;
		int m_MaxItemsPerNode;

		std::unordered_map<T, Node*> m_ItemLookup;

		void InsertInto(Node* node, T payload, const AABB& bounds);
		void Subdivide(Node* node);
		int GetChildIndex(const Node* node, const AABB& bounds) const;
		AABB GetChildBounds(const AABB& parentBounds, int childIndex) const;
		void VisitNodes(const Node* node, const std::function<void(const AABB&)>& fn) const;
	};
}

#include "Octree.inl"

#endif // SHARK_OCTREE_H