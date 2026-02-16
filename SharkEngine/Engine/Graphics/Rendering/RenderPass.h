#ifndef RENDERPASS_H
#define RENDERPASS_H

namespace Shark { class Scene; }
namespace Shark::Components { class CameraComponent; }

namespace Shark::Graphics {
	class Shader;
	class Framebuffer;

	class RenderPass
	{
	public:
		explicit RenderPass(Framebuffer* target = nullptr) : m_Target(target) {}
		virtual ~RenderPass() = default;

		// Called before drawing (bind FBO, clear, set state)
		virtual void Begin() = 0;

		// Called to render scene
		virtual void Execute(float deltaTime, Shark::Scene* scene, Shark::Components::CameraComponent* cam) = 0;

		// Called after drawing (unbind FBO, reset state)
		virtual void End() = 0;
	protected:
		Shader* shader{ nullptr };
		Framebuffer* m_Target{ nullptr };
	};
}

#endif