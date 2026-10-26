#include "Renderer.h"
#include "Passes/RenderPass.h"
#include "glad/glad.h"

namespace Shark::Graphics {
	Renderer::~Renderer() = default;

    void Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
}