#include "Renderer.h"
#include "glad/glad.h"

namespace Shark::Graphics {
    void Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
}