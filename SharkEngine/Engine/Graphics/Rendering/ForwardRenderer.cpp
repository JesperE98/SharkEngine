#include "ForwardRenderer.h"
#include "Passes/ForwardRenderPass.h"
#include "Passes/ShadowMapPass.h"
#include "Passes/PointShadowPass.h"
#include "Passes/SkyPass.h"
#include "Core/Engine/EngineContext.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Components/Rendering/LightComponent.h"
#include "Components/Logic/CameraComponent.h"
#include "Scene/Scene.h"

namespace Shark::Graphics {

	using Shark::Scene;
	using Shark::Components::CameraComponent;
	using Shark::Components::LightComponent;
	using Shark::Components::LightData;
	using Shark::Graphics::ShadowBuffer;
	using Shark::Graphics::SkyPass;

	ForwardRenderer::ForwardRenderer()
	{
		Init();
	}

	ForwardRenderer::~ForwardRenderer()
	{
		if (m_SceneFb)			delete m_SceneFb;
		if (m_ForwardPass)		delete m_ForwardPass;
		if (m_ShadowPass)		delete m_ShadowPass;
		if (m_SkyPass)			delete m_SkyPass;
		if (m_PointShadowPass)	delete m_PointShadowPass;

		m_SceneFb			= nullptr;
		m_ShadowFb			= nullptr;
		m_ForwardPass		= nullptr;
		m_ShadowPass		= nullptr;
		m_SkyPass			= nullptr;
		m_PointShadowPass	= nullptr;
	}

	void ForwardRenderer::Init()
	{
		SE_LOG(Rendering, "Initializing ForwardRenderer.");

		m_SceneFb = new Framebuffer(Shark::Core::WINDOW_WIDTH, Shark::Core::WINDOW_HEIGHT);

		
		m_ShadowPass = new ShadowMapPass(2048); // Creating Shadow pass (High Resolution for crisp shadows)
		m_PointShadowPass = new PointShadowPass(1024);
		m_SkyPass = new SkyPass(Shark::Core::WINDOW_WIDTH, Shark::Core::WINDOW_HEIGHT);
		m_ForwardPass = new ForwardRenderPass(m_SceneFb); // Creating Forward Pass and adding it to list


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
		std::vector<LightData> sceneLights;
		for (auto* obj : scene->GetGameObjects()) {
			if (auto* lightComp = obj->GetComponent<LightComponent>()) {
				sceneLights.push_back(lightComp->GetLightData());
			}
		}

		if (sceneLights.empty()) return;

		m_ShadowPass->Begin();
		m_ShadowPass->Execute(scene, sceneLights);
		m_ShadowPass->End();

		m_PointShadowPass->Begin();
		m_PointShadowPass->Execute(scene, sceneLights);
		m_PointShadowPass->End();

		glm::mat4 view			= cam->GetViewMatrix();
		glm::mat4 projection	= cam->GetProjectionMatrix();
		m_SkyPass->SetCameraData(glm::inverse(view), glm::inverse(projection));
		m_SkyPass->Begin();
		m_SkyPass->Execute(scene, sceneLights);
		m_SkyPass->End();
		
		m_ForwardPass->Begin();

		// --- DIRECTIONAL SHADOW MAPS
		for (int i = 0; i < 4; ++i) {
			m_ForwardPass->SetShadowDataAtIndex(
				i, 
				m_ShadowPass->GetShadowMapTexture(i), 
				m_ShadowPass->GetLightSpaceMatrix(i));
		}
		
		// --- POINT SHADOW CUBEMAP
		for (int i = 0; i < MAX_SHADOW_POINT_LIGHTS; ++i) {
			m_ForwardPass->SetPointShadowDataAtIndex(
				i,
				m_PointShadowPass->GetCubemap(i),
				m_PointShadowPass->GetFarPlane()
			);
		}

		m_ForwardPass->Execute(deltaTime, scene, cam, { sceneLights });
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