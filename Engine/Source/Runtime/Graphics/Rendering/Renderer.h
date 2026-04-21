#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

struct GLFWwindow;

namespace Shark				{ class Scene; }
namespace Shark::Core		{ class EngineContext; }
namespace Shark::Components { class CameraComponent; }

namespace Shark::Graphics {

	class RenderPass;

	class Renderer
	{
	public:

		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

		virtual ~Renderer() = default;
		virtual void Init() = 0;
		virtual void BeginFrame() = 0;
		virtual void RenderScene(float deltaTime, Shark::Scene* scene, Components::CameraComponent* cam) = 0;
		virtual void EndFrame() = 0;

	protected:
		Core::EngineContext* context{ nullptr };
		std::vector<std::unique_ptr<RenderPass>> renderPasses;
	};
}

#endif