#include "SkyPass.h"
#include "Graphics/Framebuffer/QuadBuffer.h"
#include "Graphics/Resources/Shader.h"
#include "Graphics/Resources/ShaderManager.h"
#include "Components/Rendering/LightComponent.h"

namespace Shark::Graphics {

	using Shark::Managers::ShaderManager;
	using Shark::Components::LightData;

	SkyPass::SkyPass(int width, int height)
	{
		shader = ShaderManager::Get().LoadShader(
			"SE_Sky",
			"Shaders/SE_Sky.vert.glsl",
			"Shaders/SE_Sky.frag.glsl"
		);

		m_QuadBuffer = new QuadBuffer(width, height);
	}

	SkyPass::~SkyPass()
	{
		if (m_QuadBuffer) delete m_QuadBuffer;

		m_QuadBuffer = nullptr;
	}

	void SkyPass::Begin()
	{
		// Render behind everything - no depth writing
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);
	}

	void SkyPass::Execute(Scene* scene, const std::vector<Shark::Components::LightData>& mainLight)
	{
		shader->Use();

		// Light uniforms
		glm::vec3 lightDir = glm::normalize(glm::vec3(
			mainLight[0].direction.x,
			mainLight[0].direction.y,
			mainLight[0].direction.z
		));

		shader->SetVector3("uLightDir",		lightDir);
		shader->SetVector3("uSunColor",		m_SunColor);
		shader->SetFloat("uSunSize",		m_SunSize);

		// Camera uniforms
		shader->SetMatrix4("uInvProjection",	m_InvProjection);
		shader->SetMatrix4("uInvView",			m_InvView);

		m_QuadBuffer->Draw();
	}

	void SkyPass::End()
	{
		// Restore state for subsequent passes
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void SkyPass::SetCameraData(const glm::mat4& invView, const glm::mat4& invProjection)
	{
		m_InvView		= invView;
		m_InvProjection = invProjection;
	}
}

