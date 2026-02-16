#ifndef RENDERER_H
#define RENDERER_H

#include "RenderPass.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <iostream>

namespace Shark				{ class Scene; }
namespace Shark::Core		{ class EngineContext; }
namespace Shark::Components	{ class CameraComponent; }

namespace Shark::Graphics {
	class RenderPass;

	class Renderer
	{
	public:

		static void FramebufferSizeCallback(GLFWwindow* m_Window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		virtual ~Renderer() = default;
		virtual void Init() = 0;
		virtual void BeginFrame() = 0;
		virtual void RenderScene(float deltaTime, Shark::Scene* scene, Shark::Components::CameraComponent* cam) = 0;
		virtual void EndFrame() = 0;

	protected:
		Shark::Core::EngineContext* context{ nullptr };
		std::vector<std::unique_ptr<RenderPass>> renderPasses;
	};
}

#endif