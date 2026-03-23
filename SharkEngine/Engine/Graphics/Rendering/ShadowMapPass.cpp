#include "ShadowMapPass.h"
#include "Graphics/Framebuffer/ShadowBuffer.h"
#include "Graphics/Resources/Shader.h"
#include "Components/Rendering/MeshRendererComponent.h"
#include "Components/Rendering/LightComponent.h"
#include "Scene/Scene.h"
#include "Managers/ShaderManager.h"
#include <glad/glad.h>

namespace Shark::Graphics {

	using Shark::Graphics::ShadowBuffer;
	using Shark::Components::LightData;
	using Shark::Components::MeshRendererComponent;
	using Shark::Managers::ShaderManager;


	ShadowMapPass::ShadowMapPass(int resolution) : m_Resolution(resolution)
	{
		m_ShadowShader = ShaderManager::Get().LoadShader("SE_Shadow", "Shaders/SE_Shadow.vert.glsl", "Shaders/SE_Shadow.frag.glsl");

		m_ShadowBuffer = std::make_unique<ShadowBuffer>(resolution, resolution);
	}

	ShadowMapPass::~ShadowMapPass()
	{
		End();
	}

	void ShadowMapPass::Begin()
	{
		//glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
	}

	void ShadowMapPass::Execute(Scene* scene, const LightData& mainLight)
	{
		m_ShadowBuffer->BindBuffer();
		glViewport(0, 0, m_ShadowBuffer->GetWidth(), m_ShadowBuffer->GetHeight());
		glClear(GL_DEPTH_BUFFER_BIT);
		m_ShadowShader->Use();


		float nearPlane = -100.0f;
		float farPlane = 100.0;
		float size = 35.0f;

		// Directional Light uses Orthographic Projection
		glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, nearPlane, farPlane);

		glm::vec3 lightDir = glm::normalize(glm::vec3(mainLight.direction));

		glm::vec3 lightPos = -lightDir * 30.0f;

		glm::vec3 up = (glm::abs(lightDir.y) > 0.99f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), up);

		// Calculate Light Space Matrix
		m_LightSpaceMatrix = lightProjection * lightView;

		m_ShadowShader->SetMatrix4("uLightSpaceMatrix", m_LightSpaceMatrix);

		//for (auto* obj : scene->GetGameObjects()) {
		//	auto* renderer = obj->GetComponent<MeshRendererComponent>();
		//	if (!renderer) continue;

		//	m_ShadowShader->SetMatrix4("uModel", obj->GetTransform().GetModelMatrix());
		//	renderer->Render();
		//}
		for (auto* obj : scene->GetGameObjects()) {
			auto* meshRenderer = obj->GetComponent<MeshRendererComponent>();
			if (!meshRenderer) continue;

			// 1. Get the RAW Mesh and Transform
			auto* mesh = meshRenderer->GetMesh(); // Assuming you have this getter
			if (!mesh) continue;

			// 2. Set the matrix on our Shadow Shader (already Bound)
			m_ShadowShader->SetMatrix4("uModel", obj->GetTransform().GetModelMatrix());

			// 3. DRAW THE RAW MESH ONLY (Avoid the MeshRenderer->Render() call)
			// This ensures NO other shader is used.
			mesh->Draw();
		}
		//SE_LOG(Rendering, "Shadow Pass: Drawing {} objects with Shadow Map ID: {}",
		//	scene->GetGameObjects().size(), m_ShadowBuffer->GetDepthTexture());

		m_ShadowBuffer->UnbindBuffer();
	}

	void ShadowMapPass::SetShadowMapTexture()
	{
	}

	void ShadowMapPass::SetLightSpaceMatrix(const glm::mat4& matrix)
	{
		m_LightSpaceMatrix = matrix;
	}

	void ShadowMapPass::End()
	{
		glCullFace(GL_BACK); // Restore normal culling
	}


	unsigned int ShadowMapPass::GetShadowMapTexture() const
	{
		return m_ShadowBuffer->GetDepthTexture();
	}
}

