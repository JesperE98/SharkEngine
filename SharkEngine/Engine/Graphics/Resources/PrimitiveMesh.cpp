#include "PrimitiveMesh.h"
#include "Components/MeshRendererComponent.h"
#include "Core/Utilities/Debug.h"
#include "Entities/GameObject.h"
#include "Managers/MeshManager.h"
#include "Material.h"
#include "Math/MathUtils.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace Shark::Graphics {

	using Shark::Math::Vector2;
	using Shark::Math::Vector3;
	using Shark::Entities::GameObject;
	using Shark::Components::MeshRendererComponent;
	using Shark::Managers::MeshManager;

	void PrimitiveMesh::DebugFace(const std::string& name, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& expectedNormal)
	{
		// Compute face normal from first triangle
		Vector3 cross = Math::Cross(v1 - v0, v2 - v0);
		Vector3 normal = Math::Normalize(cross);

		// Compare with expected outward normal
		float dot = Math::Dot(normal, expectedNormal);
		bool flipped = dot < 0.0f;

		SE_LOG(Rendering, " PrimitiveMesh::DebugFace - Face: {} | Normal: {} -> {}",
			name, normal, (flipped ? "FLIPPED" : "OK"));
	}

	Mesh* PrimitiveMesh::CreateCube()
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		// 8 cube corner
		Vector3 corners[8] = {
			{-0.5f, -0.5f, -0.5f }, // 0 back-bottom-left
			{ 0.5f, -0.5f, -0.5f }, // 1 back-bottom-right
			{ 0.5f,	 0.5f, -0.5f }, // 2 back-top-right
			{-0.5f,  0.5f, -0.5f }, // 3 back-top-left
			{-0.5f, -0.5f,  0.5f }, // 4 front-bottom-left
			{ 0.5f, -0.5f,  0.5f }, // 5 front-bottom-right
			{ 0.5f,  0.5f,  0.5f }, // 6 front-top-right
			{-0.5f,  0.5f,  0.5f }, // 7 front-top-left
		};

		// Each face defined by 4 corner indices (CCW order, outward normal)
		int faces[6][4] = {
			{4, 5, 6, 7}, // Front (+Z)
			{1, 0, 3, 2}, // Back (–Z)
			{0, 4, 7, 3}, // Left (–X)
			{5, 1, 2, 6}, // Right (+X)
			{0, 1, 5, 4}, // Bottom (–Y)
			{3, 7, 6, 2}  // Top (+Y)
		};

		// Outward normals per face
		Vector3 normals[6] = {
			{0, 0, 1},   // Front
			{0, 0, -1},  // Back
			{-1, 0, 0},  // Left
			{1, 0, 0},   // Right
			{0, -1, 0},  // Bottom
			{0, 1, 0}    // Top
		};

		// Simple UVs for each face
		Vector2 uvs[4] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		// Loop through face
		for (int f = 0; f < 6; ++f) {
			int baseIndex = static_cast<int>(vertices.size());

			for (int v = 0; v < 4; ++v) {
				Vertex vertex;
				vertex.position = corners[faces[f][v]];
				vertex.normal = normals[f]; // Compute per-face normal if needed
				vertex.uV = uvs[v];
				vertex.color = Vector3(1.0f, 1.0f, 1.0f); // White, or vary per face
				vertices.push_back(vertex);
			}

			// Add 2 triangles (quad split)
			indices.push_back(baseIndex + 0);
			indices.push_back(baseIndex + 1);
			indices.push_back(baseIndex + 2);

			indices.push_back(baseIndex + 0);
			indices.push_back(baseIndex + 2);
			indices.push_back(baseIndex + 3);


			Vector3 v0 = corners[faces[f][0]];
			Vector3 v1 = corners[faces[f][1]];
			Vector3 v2 = corners[faces[f][2]];
			Vector3 v3 = corners[faces[f][3]];

			//DebugFace("Face " + std::to_string(f), v0, v1, v2, v3, normals[f]);
			//// Debug info
			//std::cout << "Face " << f
			//	<< " normal: " << normals[f]
			//	<< " winding: CCW" << std::endl;

		}

		SE_LOG(Rendering, "PrimitiveMesh::CreateCube() - Created Cube Mesh with {} vertices and {} triangles.",
			vertices.size(), indices.size());

		return new Mesh(vertices, indices);
	}

	GameObject* PrimitiveMesh::CreatePrimitive(GameObject* obj, PrimitiveType type, Material* mat)
	{
		if (!obj) {
			SE_ERR(Rendering, "PrimitiveMesh::CreatePrimitive - Passed null GameObject!");
			return nullptr;
		}

		Mesh* mesh = MeshManager::Get().LoadMesh(type);

		if (mesh) {
			Material* matetrial = mat ? mat : new Material(); // if none mat wasn't provided, create default material
			obj->AddComponent(new MeshRendererComponent(obj, mesh, matetrial));

			std::string typeName = (type == PrimitiveType::Cube) ? "Cube" :
				(type == PrimitiveType::Sphere) ? "Sphere" :
				(type == PrimitiveType::Cylinder) ? "Cylinder" :
				(type == PrimitiveType::Plane) ? "Plane" : "Unknown";

			SE_LOG(Rendering, "Created {} primitive and attached to GameObject: {}", typeName, obj->GetName());
		}

		return obj;
	}
}