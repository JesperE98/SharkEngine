#ifndef FORWARD_RENDERER_H
#define FORWARD_RENDERER_H

#include "Renderer.h"
#include "Source/Graphics/Framebuffer/Framebuffer.h"

class ForwardRenderer : virtual public Renderer
{
public:
	explicit ForwardRenderer();
	void Init() override final;
	void BeginFrame() override final;
	void RenderScene(float deltaTime, Scene* scene, Camera* cam) override final;
	void EndFrame() override final;
	void SetTarget(Framebuffer* fb);
private:
	std::unique_ptr<Framebuffer> sceneFb; // Main scene target
	std::unique_ptr<Framebuffer> shadowFb; // Shadow map target (optional)
};

#endif