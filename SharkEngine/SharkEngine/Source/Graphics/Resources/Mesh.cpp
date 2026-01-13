#include "Mesh.h"
#include "Core/Utilities/Time.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

Mesh::Mesh() {

}

Mesh::Mesh(const std::vector<Vertex>& verts)
	: vertices(verts)
{
	std::cout << Time::CreateTimeStamp() << ": " << "Creating Mesh (non-indexed).\n";
	UploadToGPU();
}

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& indices)
	:	vertices(verts), indices(indices)
{
	std::cout << Time::CreateTimeStamp() << ": " << "Creating Mesh (indexed).\n";
	UploadToGPU();
}

Mesh::~Mesh() {
	std::cout << Time::CreateTimeStamp() << ": " << "Destroying Mesh. " << std::endl;
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
}

void Mesh::Draw() const {
	//std::cout << Time::CreateTimeStamp() << ": DRAWING MESH.\n";
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
	std::ifstream file(filename);


	if(!vertices.empty()) 
		vertices.clear();
	if(!indices.empty()) 
		indices.clear(); // Optional, in case of reusing the mesh

	// Temporary storage for the seperate data pools
	std::vector<Vector3> temp_positions;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;

	std::string line;

	if (!file.is_open()) {
		std::cerr << Time::CreateTimeStamp() << ": " << "Mesh::LoadMeshFromModel - Failed to open file." << std::endl;
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
				v.position = temp_positions[vIdx - 1];

				// if UVs exists in this token
				if (vss >> vtIdx) {
					v.uV = temp_uvs[vtIdx - 1];
				}

				// If Normals exists in this token
				if (vss >> vnIdx) {
					v.normal = temp_normals[vnIdx - 1];
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

	std::cout << Time::CreateTimeStamp() << ": Mesh::LoadMeshFromModel - Loaded " << vertices.size() << " vertices from " << filename << std::endl;

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
	std::cout << Time::CreateTimeStamp() << ": Uploading Mesh to GPU...\n";

	std::cout << Time::CreateTimeStamp() << ": SETTING UP BUFFERS.\n";
	// Generate and bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Optional EBO
	if (!indices.empty()) {
		std::cout << Time::CreateTimeStamp() << ": SETTING UP INDICES.\n";
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	std::cout << Time::CreateTimeStamp() << ": SETTING UP ATTRIBUTES.\n";
	// Vertex Attributes
	glEnableVertexAttribArray(0); // Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);	
	
	glEnableVertexAttribArray(1); // Color/Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2); // UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}