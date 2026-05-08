#include "TerrainComponent.h"
#include "Core/GameObject.h"
#include "Core/Utilities/Debug.h"

namespace Shark::Components {

	void TerrainComponent::Render() {
		if (!m_Terrain) return;
		m_Terrain->Draw();
	}

	bool TerrainComponent::LoadTerrain(const std::string& path, float hScale, float xzSc) {
		if (path.empty()) {
			SE_ERR(Engine, "No path was provided...");
			return false;
		}

		HeightmapPath = path;
		HeightScale = hScale;
		xzScale = xzSc;

		m_Terrain = std::make_unique<Graphics::Terrain>();

		if (!m_Terrain->LoadFromHeightmap(path, hScale, xzSc)) {
			m_Terrain.reset();
			return false;
		}

		SE_LOG(Rendering, "TerrainComponent loaded: {}", path);
		return true;
	}
}