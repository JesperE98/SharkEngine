#ifndef FORWARD_RENDER_PASS_H
#define FORWARD_RENDER_PASS_H

#include "RenderPass.h"

namespace Shark::Graphics
{
	class ForwardRenderPass : virtual public RenderPass
	{
	public:
		explicit ForwardRenderPass(Framebuffer* target);
		~ForwardRenderPass() override final;
		void Begin() override final;
		void Execute(float deltaTime, Shark::Scene* scene, Shark::Components::CameraComponent* cam) override final;
		void End() override final;

		void SetTarget(Framebuffer* fb);
	};
}


#endif