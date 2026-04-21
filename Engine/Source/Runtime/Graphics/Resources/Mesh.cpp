#include "Core/Utilities/Debug.h"
#include "IO/PathManager.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Mesh.h"

#include <fstream>
#include <glad/glad.h>

namespace Shark::Graphics {

	using IO::PathManager;
	using IO::PathCategory;
	using Math::Vector2;
	using Math::Vector3;

	Mesh::Mesh() {

	}

	Mesh::Mesh(const std::vector<Vertex>& verts)
		: vertices(verts)
	{
		SE_LOG(Rendering, "Creating Mesh (non-indexed) with {} vertices.", vertices.size());
		UploadToGPU();
	}

	Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& indices)
		: vertices(verts), indices(indices)
	{
		SE_LOG(Rendering, "Creating Mesh (indexed) with {} vertices and {} indices.",
			vertices.size(), indices.size());
		UploadToGPU();
	}

	Mesh::~Mesh() {
		SE_LOG(Rendering, "Mesh::~Mesh() - Destroying Mesh.");
		if (VAO) glDeleteVertexArrays(1, &VAO);
		if (VBO) glDeleteBuffers(1, &VBO);
		if (EBO) glDeleteBuffers(1, &EBO);
	}

	void Mesh::Draw() const {
		glBindVertexArray(VAO);
		if (!indices.empty()) {
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
		}
		glBindVertexArray(0);
	}

	bool Mesh::LoadMeshFromModel(const std::string& filename)
	{

		std::string fullPath = PathManager::Get().GetPath(PathCategory::Content, filename);

		std::ifstream file(fullPath);

		if (!vertices.empty())
			vertices.clear();
		if (!indices.empty())
			indices.clear(); // Optional, in case of reusing the mesh

		// Temporary storage for the seperate data pools
		std::vector<Vector3> temp_positions;
		std::vector<Vector3> temp_colors; // Optional, in case vertex colors are included in the file
		std::vector<Vector2> temp_uvs;
		std::vector<Vector3> temp_normals;

		std::string line;

		if (!file.is_open()) {
			SE_WARN(Rendering, "Mesh::LoadMeshFromModel() - Failed to open file.");
			return false;
		}

		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string prefix;
			iss >> prefix;

			if (prefix == "v") { // Vertex position
				Vector3 pos;
				iss >> pos.x >> pos.y >> pos.z;
				temp_positions.push_back(pos);

				// Try to read vertex color (optional) if it exists in the file
				Vector3 col; // Default white color
				if (iss >> col.x >> col.y >> col.z) {
					// If color data exists, we can store it in the vertex color attribute later when we construct the final vertices
					temp_colors.push_back(col);
				}
			}
			else if (prefix == "vt") // Extra objective : UVs
			{
				Vector2 uv;
				iss >> uv.x >> uv.y;
				temp_uvs.push_back(uv);
			}
			else if (prefix == "vn") // Extra objective : Normals
			{
				Vector3 norm;
				iss >> norm.x >> norm.y >> norm.z;
				temp_normals.push_back(norm);
			}
			else if (prefix == "f") { // Face indices
				std::string vertexData;
				std::vector<unsigned int> faceVertexIndices;

				while (iss >> vertexData) {
					// Parse the "v/vt/vn" format
					unsigned int vIdx = 0, vtIdx = 0, vnIdx = 0;

					// Replace the slashes with spaces to make parsing easier
					for (auto& c : vertexData) {
						if (c == '/') {
							c = ' ';
						}
					}

					std::istringstream vss(vertexData);

					vss >> vIdx;

					Vertex v{};

					// Position
					if (vIdx > 0 && vIdx <= temp_positions.size()) {
						v.position = temp_positions[vIdx - 1];
					}

					// Color (optional)
					if (!temp_colors.empty() && vIdx > 0 && vIdx <= temp_colors.size()) {
						v.color = temp_colors[vIdx - 1];
					}
					else {
						v.color = Vector3(1.0f, 1.0f, 1.0f); // Default to white if no color data
					}

					// if UVs exists in this token
					if (vss >> vtIdx) {
						if(vtIdx > 0) v.uV = temp_uvs[vtIdx - 1];
					}

					// If Normals exists in this token
					if (vss >> vnIdx) {
						if(vnIdx > 0) v.normal = temp_normals[vnIdx - 1];
					}

					// For now, adds every vertex as unique
					vertices.push_back(v);
					faceVertexIndices.push_back(static_cast<unsigned int>(vertices.size() - 1));
				}

				// Triangulize supports quads
				auto tris = Triangulize(faceVertexIndices);
				indices.insert(indices.end(), tris.begin(), tris.end());
			}
		}

		file.close();

		SE_LOG(Rendering, "Mesh::LoadMeshFromModel() - Loaded {} vertices from {}",
			vertices.size(), fullPath);

		UploadToGPU();
		return true;
	}

	std::vector<unsigned int> Mesh::Triangulize(const std::vector<unsigned int>& faceIndices)
	{
		std::vector<unsigned int> result;

		if (faceIndices.size() < 3) {
			return result; // Invalid face
		}

		for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
			result.push_back(faceIndices[0]);
			result.push_back(faceIndices[i]);
			result.push_back(faceIndices[i + 1]);
		}

		return result;
	}

	void Mesh::UploadToGPU()
	{
		SE_LOG(Rendering, "Uploading Mesh to GPU.");

		SE_LOG(OpenGL, "SETTING UP BUFFERS.");
		// Generate and bind VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		SE_LOG(OpenGL, "SETTING UP VERTEX ATTRIBUTES.");

		// Vertex Attributes
		glEnableVertexAttribArray(0); // Position - Location 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1); // Color - Location 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(2); // UV - Location 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

		glEnableVertexAttribArray(3); // Normal - Location 3
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));

		// Optional EBO
		if (!indices.empty()) {
			SE_LOG(OpenGL, "SETTING UP INDICES.");
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		}

		glBindVertexArray(0);
	}
}