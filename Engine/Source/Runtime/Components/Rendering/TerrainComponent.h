#ifndef TERRAIN_COMPONENT_H
#define TERRAIN_COMPONENT_H

#include "Components/Component.h"
#include "Graphics/Resources/Terrain.h"

#include <string>

namespace Shark::Components {

	class TerrainComponent : public Component {
	public:

		// Tunable field
		std::string HeightmapPath;
		float HeightScale = 25.0f;
		float xzScale = 1.0f;

		TerrainComponent() {
			tickMode = TickMode::Always;
		}

		void Update(float deltaTime) override final {}
		void Render();

		bool LoadTerrain(const std::string& path, float hScale, float xzSc);

		Graphics::Terrain* GetTerrain() { return m_Terrain.get(); }
		const Graphics::Terrain* GetTerrain() const { return m_Terrain.get(); }

	private:
		std::unique_ptr<Graphics::Terrain> m_Terrain;
	};
}

#endif