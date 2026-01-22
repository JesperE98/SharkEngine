#ifndef FORWARD_RENDERER_H
#define FORWARD_RENDERER_H

#include "Renderer.h"

namespace Shark::Graphics
{
	class Framebuffer;

	class ForwardRenderer : virtual public Renderer
	{
	public:
		explicit ForwardRenderer();
		void Init() override final;
		void BeginFrame() override final;
		void RenderScene(float deltaTime, Shark::Scene* scene, Shark::Entities::Camera* cam) override final;
		void EndFrame() override final;
		void SetTarget(Framebuffer* fb);
	private:
		std::unique_ptr<Framebuffer> sceneFb; // Main scene target
		std::unique_ptr<Framebuffer> shadowFb; // Shadow map target (optional)
	};
}

#endif