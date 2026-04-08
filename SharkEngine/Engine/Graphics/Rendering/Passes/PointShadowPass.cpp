#include "PointShadowPass.h"
#include "Graphics/Framebuffer/PointShadowBuffer.h"
#include "Graphics/Resources/Shader.h"
#include "Managers/ShaderManager.h"
#include "Components/Rendering/MeshRendererComponent.h"
#include "Components/Rendering/LightComponent.h"
#include "Scene/Scene.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Shark::Graphics {

	using Shark::Components::LightData;
	using Shark::Components::MeshRendererComponent;
	using Shark::Managers::ShaderManager;

	PointShadowPass::PointShadowPass(int resolution)
	{
		shader = ShaderManager::Get().LoadShader(
			"SE_PointShadow",
			"Shaders/SE_PointShadow.vert.glsl",
			"Shaders/SE_PointShadow.frag.glsl",
			"Shaders/SE_PointShadow.geom.glsl"
		);

		for (int i = 0; i < MAX_SHADOW_POINT_LIGHTS; i++) {
			m_Buffers[i] = new PointShadowBuffer(resolution);
		}
	}

	PointShadowPass::~PointShadowPass()
	{
		End();
	}

	void PointShadowPass::Begin()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	void PointShadowPass::End()
	{
		glCullFace(GL_BACK);
	}

	void PointShadowPass::Execute(Shark::Scene * scene, std::vector<Shark::Components::LightData>&lights)
	{
		int shadowIndex = 0;

		for (auto& light : lights) {
			if (light.type != 1) continue;
			if (shadowIndex >= MAX_SHADOW_POINT_LIGHTS) break;

			RenderPointLight(scene, light, shadowIndex);
			shadowIndex++;
		}
	}

	void PointShadowPass::RenderPointLight(Shark::Scene* scene, const Shark::Components::LightData& light, int index)
	{
		glViewport(0, 0, m_Resolution, m_Resolution);
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPos = glm::vec3(light.position.x, light.position.y, light.position.z);

		// Build 6 view matrices — one per cubemap face
		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, m_FarPlane);

		std::vector<glm::mat4> shadowMatrices = {
		   proj * glm::lookAt(lightPos, lightPos + glm::vec3(1, 0, 0), glm::vec3(0,-1, 0)),
		   proj * glm::lookAt(lightPos, lightPos + glm::vec3(-1, 0, 0), glm::vec3(0,-1, 0)),
		   proj * glm::lookAt(lightPos, lightPos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
		   proj * glm::lookAt(lightPos, lightPos + glm::vec3(0,-1, 0), glm::vec3(0, 0,-1)),
		   proj * glm::lookAt(lightPos, lightPos + glm::vec3(0, 0, 1), glm::vec3(0,-1, 0)),
		   proj * glm::lookAt(lightPos, lightPos + glm::vec3(0, 0,-1), glm::vec3(0,-1, 0)),
		};

		shader->Use();

		for (int i = 0; i < 6; i++) {
			shader->SetMatrix4("UShadowMatrices[" + std::to_string(i + 3) + "]", shadowMatrices[i]);
		}

		shader->SetVector3("uLightPos", lightPos);
		shader->SetFloat("uFarPlane", m_FarPlane);

		// Draw all meshes
		for (auto* obj : scene->GetGameObjects()) {
			auto* meshRenderer = obj->GetComponent<MeshRendererComponent>();
			if (!meshRenderer) continue;
			auto* mesh = meshRenderer->GetMesh();
			if (!mesh) continue;
			shader->SetMatrix4("uModel", obj->GetTransform().GetModelMatrix());
			mesh->Draw();
		}

	}

	unsigned int PointShadowPass::GetCubemap(int index) const
	{
		if (index >= MAX_SHADOW_POINT_LIGHTS) return 0;

		return m_Buffers[index]->GetCubemap();
	}
}