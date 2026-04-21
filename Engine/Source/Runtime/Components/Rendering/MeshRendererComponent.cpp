#include "MeshRendererComponent.h"
#include "Core/GameObject.h"

#include <glad/glad.h>

namespace Shark::Components {

	using Core::GameObject;
	using Graphics::Mesh;
	using Graphics::Material;

	MeshRendererComponent::~MeshRendererComponent()
	{
		if (m_Material) {
			delete m_Material;
		}
	}

	void MeshRendererComponent::Render() {
		if (!m_Material || !m_Mesh) return;

		// Compute if the transform flips winding
		glm::mat4 model = GetOwner()->GetTransform().GetModelMatrix();
		m_Material->GetShader()->SetMatrix4("uModel", GetOwner()->GetTransform().GetModelMatrix());

		float determinant = glm::determinant(glm::mat3(model));
		if (determinant < 0.0f) glFrontFace(GL_CW); // Flips the winding if negative value

		m_Material->Bind();
		m_Mesh->Draw();

		if (determinant < 0.0f) glFrontFace(GL_CCW);
	}

	void MeshRendererComponent::SetMesh(Mesh* mesh)
	{
		m_Mesh = mesh ? mesh : nullptr;
	}

	void MeshRendererComponent::SetMaterial(Material* mat)
	{
		m_Material = mat ? mat : nullptr;
	}

	Mesh* MeshRendererComponent::GetMesh() const
	{
		return m_Mesh;
	}

	Material* MeshRendererComponent::GetMaterial() const
	{
		return m_Material;
	}

}