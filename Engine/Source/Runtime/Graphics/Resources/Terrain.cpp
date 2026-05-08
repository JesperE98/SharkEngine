#include "Terrain.h"
#include "Core/Utilities/Debug.h"
#include "IO/PathManager.h"
#include "Terrain.h"

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <cmath>

namespace Shark::Graphics {

	Terrain::~Terrain() {
		if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
		if (m_VBO) glDeleteBuffers(1, &m_VBO);
		if (m_IBO) glDeleteBuffers(1, &m_IBO);
	}

	bool Terrain::LoadFromHeightmap(const std::string& heightmapPath, float heightScale, float xzScale) {
		
		if (heightmapPath.empty()) {
			SE_ERR(Rendering, "Height Map Path was null or empty. Path: {}", heightmapPath);
			return false;
		}

		if (!heightScale) {
			SE_ERR(Rendering, "Height Scale was invalid number. Value: {}", heightScale);
			return false;
		}
		m_HeightScale = heightScale;

		if (!xzScale) {
			SE_ERR(Rendering, "XZ Scale was invalid number. Value: {}", xzScale);
			return false;
		}
		m_XZScale = xzScale;

		// Load heightmap
		std::string fullPath = IO::PathManager::Get().GetPath(IO::PathCategory::Content, heightmapPath);

		int channels;
		stbi_set_flip_vertically_on_load(false); // terrain heightmaps usually aren't flipped
		unsigned char* data = stbi_load(fullPath.c_str(), &m_Width, &m_Height, &channels, 1);

		if (!data) {
			SE_ERR(Rendering, "Failed to load heightmap: {}", fullPath);
			return false;
		}

		SE_SUCC(Rendering, "Loaded heightmap {}x{} from {}", m_Width, m_Height, heightmapPath);

		// Store height data for collision queries
		m_HeightData.resize(m_Width * m_Height);
		for (int i = 0; i < m_Width * m_Height; i++) {
			m_HeightData[i] = ( data[i] / 255.0f ) * m_HeightScale;
		}

		std::vector<TerrainVertex> vertices;
		std::vector<unsigned int> indices;

		generateVertices(data, vertices);
		generateIndices(indices);
		uploadToGPU(vertices, indices);

		stbi_image_free(data);

		SE_LOG(Rendering, "Terrain created: {} vertices, {} triangles", vertices.size(), indices.size() / 3);

		return true;
	}

	void Terrain::Draw() const {
		if (m_VAO == 0) return;

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	float Terrain::GetHeightAt(float localX, float localZ) const {
		if (m_HeightData.empty()) {
			SE_ERR(Rendering, "Height Data map are empty.");
			return 0.0f;
		}

		float halfWidth = ( m_Width - 1 ) * m_XZScale * 0.5f;
		float halfDepth = ( m_Height - 1 ) * m_XZScale * 0.5f;

		// Convert world coords to grid coords
		float gridX = (localX + halfWidth) / m_XZScale;
		float gridZ = (localZ + halfDepth) / m_XZScale;

		// Clamp to valid range
		if (gridX < 0) gridX = 0;
		if (gridZ < 0) gridZ = 0;
		if (gridX >= m_Width - 1)	gridX = (float)( m_Width - 2 );
		if (gridZ >= m_Height - 1)	gridZ = (float)(m_Height - 2);

		// Integer grid scroll
		int x0 = (int)gridX;
		int z0 = (int)gridZ;
		int x1 = x0 + 1;
		int z1 = z0 + 1;

		// Fractional part
		float fx = gridX - x0;
		float fz = gridZ - z0;

		// Bilinear interpolation
		float h00 = m_HeightData[z0 * m_Width + x0];
		float h10 = m_HeightData[z0 * m_Width + x1];
		float h01 = m_HeightData[z1 * m_Width + x0];
		float h11 = m_HeightData[z1 * m_Width + x1];

		float h0 = h00 + fx * ( h10 - h00 );
		float h1 = h01 + fx * ( h11 - h01 );

		return h0 + fz * ( h1 - h0 );
	}

	void Terrain::generateVertices(unsigned char* data, std::vector<TerrainVertex>& vertices) {
		if (vertices.empty()) {
			vertices.reserve(m_Width * m_Height);
		}

		float halfWidth = ( m_Width - 1 ) * m_XZScale * 0.5f;
		float halfDepth = ( m_Height - 1 ) * m_XZScale * 0.5f;

		for (int z = 0; z < m_Height; z++) {
			for (int x = 0; x < m_Width; x++) {
				float height = m_HeightData[z * m_Width + x];

				TerrainVertex v{};

				// Position
				v.position[0] = x * m_XZScale - halfWidth;
				v.position[1] = height;
				v.position[2] = z * m_XZScale - halfDepth;

				// Color - just white, the texutre provides the actual color
				v.color[0] = 1.0f;
				v.color[1] = 1.0f;
				v.color[2] = 1.0f;

				// UV
				v.uv[0] = static_cast<float>( x ) / static_cast<float>( m_Width - 1 );
				v.uv[1] = static_cast<float>( z ) / static_cast<float>( m_Height - 1 );

				// Normal - computes from neighboring heights
				float hL = ( x > 0 )			? m_HeightData[z * m_Width + ( x - 1 )] : height;
				float hR = ( x < m_Width - 1 )	? m_HeightData[z * m_Width + ( x + 1 )] : height;
				float hD = ( z > 0 )			? m_HeightData[(z - 1 ) * m_Width + x] : height;
				float hU = ( z < m_Height - 1 ) ? m_HeightData[(z + 1 ) * m_Width + x] : height;

				float nx = ( hL - hR );
				float ny = 2.0f * m_XZScale;
				float nz = ( hD - hU );
				float len = std::sqrt(nx * nx + ny * ny + nz * nz);

				v.normal[0] = nx / len;
				v.normal[1] = ny / len;
				v.normal[2] = nz / len;

				vertices.push_back(v);
			}
		}
	}

	void Terrain::generateIndices(std::vector<unsigned int>& indices) {
		if (indices.empty()) {
			// Two triangles per quad cell
			indices.reserve(( m_Width - 1 ) * ( m_Height - 1 ) * 6);
		}

		for (int z = 0; z < m_Height - 1; z++) {
			for (int x = 0; x < m_Width - 1; x++) {
				unsigned int topLeft = z * m_Width + x;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = ( z + 1 ) * m_Width + x;
				unsigned int bottomRight = bottomLeft + 1;

				// Triangle 1
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);

				// Triangle 2
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}

		m_IndexCount = static_cast<int>( indices.size() );
	}

	void Terrain::uploadToGPU(const std::vector<TerrainVertex>& vertices, const std::vector<unsigned int>& indices) {
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainVertex), vertices.data(), GL_STATIC_DRAW);

		// Position attribute - location 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)offsetof(TerrainVertex, position));
		glEnableVertexAttribArray(0);

		// Color attribute - location 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)offsetof(TerrainVertex, color));
		glEnableVertexAttribArray(1);

		// UV attribute - location 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)offsetof(TerrainVertex, uv));
		glEnableVertexAttribArray(2);

		// Normal attribute - location 3
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)offsetof(TerrainVertex, normal));
		glEnableVertexAttribArray(3);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}

