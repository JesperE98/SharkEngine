#pragma once
#include "Component.h"
#include "Source/Graphics/Resources/Material.h"
#include "Source/Graphics/Resources/Mesh.h"
#include "Core/Entities/GameObject.h"

class MeshRendererComponent : public Component
{
public:
	MeshRendererComponent(GameObject* owner, Mesh* mesh, Material* mat)
		: Component(owner, "MeshRenderer"), m_GameObject(owner), m_Mesh(mesh), m_Material(mat) {}

	~MeshRendererComponent() override = default;

	void Update(float deltaTime) override {}

	void Render();

	// --- Asset Getters ---
	Mesh* GetMesh() const;
	Material* GetMaterial() const;

	// --- Asset Setters ---
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* mat);

private:
	Mesh* m_Mesh{ nullptr };
	Material* m_Material{ nullptr };
	GameObject* m_GameObject{ nullptr };
};

