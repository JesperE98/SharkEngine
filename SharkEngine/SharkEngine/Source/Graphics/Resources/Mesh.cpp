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
	std::string line;

	if (!file.is_open()) {
		std::cerr << Time::CreateTimeStamp() << ": " << "Mesh::LoadMeshFromModel - Failed to open file." << std::endl;
		return false;
	}

	if(!vertices.empty()) 
		vertices.clear();
	if(!indices.empty()) 
		indices.clear(); // Optional, in case of reusing the mesh

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v") { // Vertex position
			Vertex vertex{};
			iss >> vertex.position.x >> vertex.position.y >> vertex.position.z;

			this->vertices.push_back(vertex);
		}
		else if (prefix == "f") { // Face indices
			std::vector<unsigned int> faceIndices;
			std::string token;

			while (iss >> token) {
				std::istringstream tokenStream(token);
				std::string vStr;
				std::getline(tokenStream, vStr, '/');
				faceIndices.push_back(std::stoi(vStr) - 1); // OBJ indices are 1-based
			}

			auto triangle = Triangulize(faceIndices);
			indices.insert(indices.end(), triangle.begin(), triangle.end());
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