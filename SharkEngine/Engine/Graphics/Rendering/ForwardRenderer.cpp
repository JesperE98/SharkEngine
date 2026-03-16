#include "ForwardRenderer.h"
#include "ForwardRenderPass.h"
#include "Core/Engine/EngineContext.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Components/Rendering/LightComponent.h"
#include "Scene/Scene.h"

namespace Shark::Graphics {

	using Shark::Scene;
	using Shark::Components::CameraComponent;
	using Shark::Components::LightComponent;
	using Shark::Components::LightData;

	ForwardRenderer::ForwardRenderer()
	{
		SE_LOG(Rendering, "ForwardRenderer::ForwardRenderer() - Created ForwardRenderer.");
	}

	void ForwardRenderer::Init()
	{
		SE_LOG(Rendering, "ForwardRenderer::Init() - Initializing ForwardRenderer.");

		sceneFb = std::make_unique<Framebuffer>(Shark::Core::WINDOW_WIDTH, Shark::Core::WINDOW_HEIGHT);

		std::unique_ptr<ForwardRenderPass> pass = std::make_unique<ForwardRenderPass>(sceneFb.get());
		//SE_LOG(Rendering, "ForwardRenderer::Init() - Adding {} to RenderPasses vector.", pass);
		renderPasses.push_back(std::move(pass));

		SE_LOG(Rendering, "ForwardRenderer::Init() - ForwardRenderer initialized successfully.");
		SE_LOG(Rendering, "ForwardRenderer::Init() - Amount of passes in RenderPasses: {}", renderPasses.size());
	}

	void ForwardRenderer::BeginFrame()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		for (auto& pass : renderPasses) {
			pass->Begin();
		}
	}

	void ForwardRenderer::RenderScene(float deltaTime, Scene* scene, CameraComponent* cam)
	{
		// Gather all lights in the scene
		std::vector<LightData> sceneLights;
		for (auto* obj : scene->GetGameObjects()) {
			if (auto* light = obj->GetComponent<LightComponent>()) {
				sceneLights.push_back(light->GetLightData());
			}
		}

		for (auto& pass : renderPasses) {
			pass->Execute(deltaTime, scene, cam, sceneLights);
		}
	}

	void ForwardRenderer::EndFrame()
	{
		for (auto& pass : renderPasses) {
			pass->End();
		}
	}

	void ForwardRenderer::SetTarget(Framebuffer* fb) {
		for (auto& pass : renderPasses) {
			auto* forwardPass = dynamic_cast<ForwardRenderPass*>(pass.get());
			if (forwardPass) {
				forwardPass->SetTarget(fb);
			}
		}
	}
}