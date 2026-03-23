#include "ForwardRenderer.h"
#include "ForwardRenderPass.h"
#include "ShadowMapPass.h"
#include "Core/Engine/EngineContext.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Components/Rendering/LightComponent.h"
#include "Scene/Scene.h"

namespace Shark::Graphics {

	using Shark::Scene;
	using Shark::Components::CameraComponent;
	using Shark::Components::LightComponent;
	using Shark::Components::LightData;
	using Shark::Graphics::ShadowBuffer;

	ForwardRenderer::ForwardRenderer()
	{
		Init();
	}

	ForwardRenderer::~ForwardRenderer()
	{
		if (m_SceneFb) delete m_SceneFb;
		//if (m_ShadowFb) delete m_ShadowFb;
		if (m_ForwardPass) delete m_ForwardPass;
		if (m_ShadowPass) delete m_ShadowPass;


		m_SceneFb = nullptr;
		m_ShadowFb = nullptr;
		m_ForwardPass = nullptr;
		m_ShadowPass = nullptr;
	}

	void ForwardRenderer::Init()
	{
		SE_LOG(Rendering, "Initializing ForwardRenderer.");

		m_SceneFb = new Framebuffer(Shark::Core::WINDOW_WIDTH, Shark::Core::WINDOW_HEIGHT);

		// Creating Shadow pass (High Resolution for crisp shadows)
		m_ShadowPass = new ShadowMapPass(2048);

		// Creating Forward Pass and adding it to list
		m_ForwardPass = new ForwardRenderPass(m_SceneFb);

		SE_LOG(Rendering, "ForwardRenderer initialized successfully.");
		SE_LOG(Rendering, "Amount of passes in RenderPasses: {}", renderPasses.size());
	}

	void ForwardRenderer::BeginFrame()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	void ForwardRenderer::RenderScene(float deltaTime, Scene* scene, CameraComponent* cam)
	{
		LightData mainLight;
		bool found = false;
		for (auto* obj : scene->GetGameObjects()) {
			if (auto* lightComp = obj->GetComponent<LightComponent>()) {
				mainLight = lightComp->GetLightData();
				found = true;
				break;
			}
		}

		if (!found) return;

		m_ShadowPass->Begin();
		m_ShadowPass->Execute(scene, mainLight);
		m_ShadowPass->End();

		m_ForwardPass->Begin();
		m_ForwardPass->SetShadowData(m_ShadowPass->GetShadowMapTexture(), m_ShadowPass->GetLightSpaceMatrix());

		m_ForwardPass->Execute(deltaTime, scene, cam, {mainLight});
		m_ForwardPass->End();
	}

	void ForwardRenderer::EndFrame()
	{
		for (auto& pass : renderPasses) pass->End();
	}

	void ForwardRenderer::SetTarget(Framebuffer* fb) {
		if (m_ForwardPass) {
			m_ForwardPass->SetTarget(fb);
		}
	}
	ShadowMapPass* ForwardRenderer::GetShadowPass() const
	{
		return m_ShadowPass;
	}
}