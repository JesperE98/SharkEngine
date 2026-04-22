#include "ShadowMapPass.h"
#include "Graphics/Framebuffer/ShadowBuffer.h"
#include "Graphics/Resources/Shader.h"
#include "Components/Rendering/MeshRendererComponent.h"
#include "Components/Rendering/LightComponent.h"
#include "Scene/Scene.h"
#include "Graphics/Resources/ShaderManager.h"

namespace Shark::Graphics {

	using Graphics::ShadowBuffer;
	using Components::LightData;
	using Components::MeshRendererComponent;
	using Resources::ShaderManager;


	ShadowMapPass::ShadowMapPass(int resolution) : m_Resolution(resolution)
	{
		SE_PROC(Rendering, "Creating Shadow Map Pass...");
		shader = ShaderManager::Get().LoadShader(
			"SE_Shadow", 
			"SE_Shadow.vert.glsl", 
			"SE_Shadow.frag.glsl"
		);

		for (int i = 0; i < 4; i++) {
			m_ShadowBuffers[i] = std::make_unique<ShadowBuffer>(resolution, resolution);
		}
		SE_SUCC(Rendering, "Shadow Map Pass complete.");
	}

	ShadowMapPass::~ShadowMapPass()
	{
		End();
	}

	void ShadowMapPass::Begin()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	void ShadowMapPass::Execute(Scene* scene, const std::vector<LightData>& lights)
	{
		m_ActiveShadowCount = 0;
		shader->Use();

		for (const auto& light : lights) {

			if (light.type != 0 || m_ActiveShadowCount >= 4) continue;

			m_ShadowBuffers[m_ActiveShadowCount]->BindBuffer();
			glViewport(0, 0, m_Resolution, m_Resolution);
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(2.0f, 4.0f);

			float size = 35.0f;
			float nearPlane = 0.1f;
			float farPlane = 200.0;

			// Directional Light uses Orthographic Projection
			glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, nearPlane, farPlane);
			glm::vec3 lightDir = glm::normalize(glm::vec3(light.direction));
			glm::vec3 lightPos = -lightDir * 50.0f;
			glm::vec3 up = (glm::abs(lightDir.y) > 0.99f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);

			glm::vec3 sceneCenter(0.0f); // Assuming the scene is centered at the origin
			int count = 0;
			for (auto* obj : scene->GetGameObjects()) {
				auto pos = obj->GetTransform().position;
				sceneCenter += glm::vec3(pos.x, pos.y, pos.z);
				count++;
			}

			if (count > 0) sceneCenter /= count;

			glm::mat4 lightView = glm::lookAt(lightPos, sceneCenter, up);

			// Calculate Light Space Matrix
			m_LightSpaceMatrices[m_ActiveShadowCount] = lightProjection * lightView;

			shader->SetMatrix4("uLightSpaceMatrix", m_LightSpaceMatrices[m_ActiveShadowCount]);


			for (auto* obj : scene->GetGameObjects()) {
				auto* meshRenderer = obj->GetComponent<MeshRendererComponent>();
				if (!meshRenderer) continue;

				// 1. Get the RAW Mesh and Transform
				auto* mesh = meshRenderer->GetMesh(); // Assuming you have this getter
				if (!mesh) continue;

				// 2. Set the matrix on our Shadow Shader (already Bound)
				shader->SetMatrix4("uModel", obj->GetTransform().GetModelMatrix());

				// 3. DRAW THE RAW MESH ONLY (Avoid the MeshRenderer->Render() call)
				// This ensures NO other shader is used.
				mesh->Draw();
			}

			glDisable(GL_POLYGON_OFFSET_FILL);
			m_ShadowBuffers[m_ActiveShadowCount]->UnbindBuffer();
			m_ActiveShadowCount++;
		}

	}

	void ShadowMapPass::SetShadowMapTexture()
	{
	}

	void ShadowMapPass::SetLightSpaceMatrix(int index, const glm::mat4& matrix)
	{
		m_LightSpaceMatrices[index] = matrix;
	}

	glm::mat4 ShadowMapPass::GetLightSpaceMatrix(int index) const
	{
		if (index < m_ActiveShadowCount) return m_LightSpaceMatrices[index];
		return	glm::mat4(1.0f);
	}

	void ShadowMapPass::End()
	{
		glCullFace(GL_BACK); // Restore normal culling
	}


	unsigned int ShadowMapPass::GetShadowMapTexture(int index) const
	{
		if (index < m_ActiveShadowCount) return m_ShadowBuffers[index]->GetDepthTexture();
		return 0;
	}
}

