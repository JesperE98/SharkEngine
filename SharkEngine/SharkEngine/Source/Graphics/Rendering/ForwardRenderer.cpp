#include "ForwardRenderer.h"
#include "ForwardRenderPass.h"
#include "Core/Engine/EngineContext.h"
#include "Core/Engine/Scene.h"
#include "Core/Utilities/Time.h"

ForwardRenderer::ForwardRenderer()
{
	std::cout << Time::CreateTimeStamp() << ": " << "Created ForwardRenderer.\n";
}

void ForwardRenderer::Init()
{
	std::cout << Time::CreateTimeStamp() << ": " << "Initializing ForwardRenderer.\n";

	sceneFb = std::make_unique<Framebuffer>(WINDOW_WIDTH, WINDOW_HEIGHT);

	std::unique_ptr<ForwardRenderPass> pass = std::make_unique<ForwardRenderPass>(sceneFb.get());
	std::cout << Time::CreateTimeStamp() << ": " << "ForwardRenderer - Pushing " << pass << " to the RenderPasses vector.\n";
	renderPasses.push_back(std::move(pass));

	std::cout << Time::CreateTimeStamp() << ": " << "ForwardRenderer - Amount of passes in RenderPasses: " << renderPasses.size() << ".\n";
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

void ForwardRenderer::RenderScene(float deltaTime, Scene* scene, Camera* cam)
{
	for (auto& pass : renderPasses) {
		pass->Execute(deltaTime, scene, cam);
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
