#include "MeshRendererComponent.h"

#include <GLFW/glfw3.h>

void MeshRendererComponent::Render() {
	if (!m_Material || !m_Mesh) return;

	// Compute if the transform flips winding
	glm::mat4 model = m_GameObject->GetTransform().GetModelMatrix();
	float determinant = glm::determinant(glm::mat3(model));

	if (determinant < 0.0f) {
		glFrontFace(GL_CW); // Flips the winding if negative value
	}

	if (m_Mesh && m_Material) {
		m_Material->Bind();
		m_Mesh->Draw();
	}

	if (determinant < 0.0f) {
		glFrontFace(GL_CCW);
	}
}