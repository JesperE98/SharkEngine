#ifndef MESH_H
#define MESH_H

#include "IRenderable.h"
#include <vector>
#include <string>
#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace Shark::Graphics
{
	struct Vertex {
		Math::Vector3 position;	// v
		Math::Vector3 color;		// vc
		Math::Vector2 uV;			// f
		Math::Vector3 normal;		// vn
	};

	class Mesh : public IRenderable
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