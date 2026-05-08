#ifndef TERRAIN_H
#define TERRAIN_H

#include "IRenderable.h"
#include <vector>
#include <string>

namespace Shark::Graphics {
	
	struct TerrainVertex {
		float position[3];
		float color[3];
		float uv[2];
		float normal[3];
	};

	class Terrain : public IRenderable {
	public:

		Terrain() = default;
		~Terrain();

		bool LoadFromHeightmap(const std::string& heightmapPath, float heightScale = 25.0f, float xzScale = 1.0f);
		void Draw() const override final;

		// Height query for collision - returns interpolated height at world (x, z)
		float GetHeightAt(float worldX, float worldZ) const;

		float GetWidth() const { return m_XZScale * ( m_Width - 1 ); }
		float GetDepth() const { return m_XZScale * ( m_Height - 1 ); }

	private:

		unsigned int m_VBO;
		unsigned int m_VAO;
		unsigned int m_IBO;

		int m_Width;
		int m_Height;
		int m_IndexCount = 0;
		float m_HeightScale = 25.0f;
		float m_XZScale = 1.0f;

		std::vector<float> m_HeightData;

		void generateVertices(unsigned char* data, std::vector<TerrainVertex>& vertices);;
		void generateIndices(std::vector<unsigned int>& indices);
		void uploadToGPU(const std::vector<TerrainVertex>& vertices, const std::vector<unsigned int>& indices);
	};
}

#endif // TERRAIN_H