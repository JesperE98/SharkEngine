#include "RenderTarget.h"
#include <glad/glad.h>

namespace Shark::Graphics {

	RenderTarget::RenderTarget(int w, int h) : m_Width(w), m_Height(h) {
		glGenFramebuffers(1, &m_FBO);
		SE_LOG(OpenGL, "Created FBO with ID: {}", m_FBO);
	}

	RenderTarget::~RenderTarget() {
		if (m_FBO) glDeleteFramebuffers(1, &m_FBO);
	}

	void RenderTarget::BindBuffer() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_Width, m_Height);
	}

	void RenderTarget::UnbindBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }


}