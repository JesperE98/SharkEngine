#pragma once
#include "Component.h"
#include "Graphics/Resources/Material.h"
#include "Graphics/Resources/Mesh.h"
#include "Core/Entities/GameObject.h"

class MeshRendererComponent : public Component
{
public:
	MeshRendererComponent(GameObject* owner, Mesh* mesh, Material* mat)
		: Component(owner, "MeshRenderer"), m_GameObject(owner), m_Mesh(mesh), m_Material(mat) {}

	void Update(float deltaTime) override {}

	void Render();

	Material* GetMaterial() const { return m_Material; }

private:
	Mesh* m_Mesh{ nullptr };
	Material* m_Material{ nullptr };
	GameObject* m_GameObject{ nullptr };
};

