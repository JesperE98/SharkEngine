#include "AABBComponent.h"
#include "Core/GameObject.h"

namespace Shark::Components {

	using Shark::Math::Vector3;

	AABB AABBComponent::GetWorldAABB() const
	{
		auto& transform = GetOwner()->GetTransform();
		Vector3 center = transform.position + m_Offset;

		// Scale extents by obect scale
		Vector3 scaledExtents = {
			m_Extents.x * transform.scale.x,
			m_Extents.y * transform.scale.y,
			m_Extents.z * transform.scale.z
		};


		return AABB{ center - scaledExtents, center + scaledExtents };
	}
}