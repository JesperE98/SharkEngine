#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Components/Component.h"
#include "Math/Vector3.h"

namespace Shark::Components {

	enum class LightType {
		Directional = 0,
		Point = 1,
	};

	struct LightData {
		int type;
		Shark::Math::Vector3 color;
		float intensity;

		// Specifics
		Shark::Math::Vector3 direction; // For Directional Light
		Shark::Math::Vector3 position;	// For Point Light
		float range;					// For Point Light Attenuation
	};

	class LightComponent : virtual public Component {
	public:

		LightType Type = LightType::Point;
		Shark::Math::Vector3 Color = { 1.0f, 1.0f, 1.0f };
		float Intensity = 1.0f;
		float Range = 10.0f;

		LightData GetLightData();
	};
}

#endif
