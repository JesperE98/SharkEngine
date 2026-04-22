#ifndef FORWARD_RENDERER_H
#define FORWARD_RENDERER_H

#include "Renderer.h"

namespace Shark::Graphics
{
	class Framebuffer;
	class ShadowMapPass;
	class ShadowBuffer;
	class ForwardRenderPass;
	class SkyPass;
	class PointShadowPass;

	class ForwardRenderer : public Renderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer() override;
		void OnInitialize() override final;
		void OnBeginFrame() override final;
		void OnRenderScene(float deltaTime, Shark::Scene* scene, Components::CameraComponent* cam) override final;
		void OnEndFrame() override final;
		void SetTarget(Framebuffer* fb);
		ShadowMapPass* GetShadowPass() const;
	private:
		Framebuffer* m_SceneFb				{ nullptr }; // Main scene target
		ShadowBuffer* m_ShadowFb			{ nullptr }; // Shadow map target (optional)
		ForwardRenderPass* m_ForwardPass	{ nullptr };
		ShadowMapPass* m_ShadowPass			{ nullptr };
		SkyPass* m_SkyPass					{ nullptr };
		PointShadowPass* m_PointShadowPass	{ nullptr };
	};
}

#endif