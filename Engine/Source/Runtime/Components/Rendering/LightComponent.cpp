#include "LightComponent.h"
#include "Core/GameObject.h"

namespace Shark::Components {

	using Core::GameObject;

	LightData LightComponent::GetLightData()
	{
		LightData data;
		data.type = static_cast<int>(Type);
		data.color = Color;
		data.intensity = Intensity;
		data.range = Range;

		// Get Transform data from the owner
		data.position = GetOwner()->GetTransform().position;

		// Direction is usually the "Forward" vector of the object
		data.direction = GetOwner()->GetTransform().GetForward();

		return data;
	}
	
}


