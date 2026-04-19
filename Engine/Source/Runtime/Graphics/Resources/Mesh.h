#ifndef MESH_H
#define MESH_H

#include "Interfaces/IRenderable.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include <vector>
#include <string>

namespace Shark::Graphics
{
	struct Vertex {
		Shark::Math::Vector3 position;	// v
		Shark::Math::Vector3 color;		// vc
		Shark::Math::Vector2 uV;			// f
		Shark::Math::Vector3 normal;		// vn
	};

	class Mesh : public Shark::Interfaces::IRenderable
	{
	public:
		unsigned int VBO{ 0 };
		unsigned int VAO{ 0 };
		unsigned int EBO{ 0 };

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Mesh();
		Mesh(const std::vector<Vertex>& verts);
		Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& indices);
		virtual ~Mesh();

		void Draw() const override;
		bool LoadMeshFromModel(const std::string& filename);
		std::vector<unsigned int> Triangulize(const std::vector<unsigned int>& faceIndices);

	private:

		void UploadToGPU();

	};
}

#endif