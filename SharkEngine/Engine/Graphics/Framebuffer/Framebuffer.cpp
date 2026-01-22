#include "Framebuffer.h"
#include "Core/Utilities/Debug.h"

#include <stdexcept>

namespace Shark::Graphics {
	Framebuffer::Framebuffer(int width, int height)
		: m_Width(width), m_Height(height) {

		// Create FBO
		glGenFramebuffers(1, &m_FBO);
		Bind();

		// Color texture
		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

		// Depth buffer
		glGenRenderbuffers(1, &m_DepthRbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRbo);

		// Check completeness
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			SHARK_FATAL(Rendering, "Framebuffer::Framebuffer() - not complete!");
		}

		Unbind();
	}

	Framebuffer::~Framebuffer() {
		if (m_ColorTexture) glDeleteTextures(1, &m_ColorTexture);
		if (m_DepthRbo) glDeleteRenderbuffers(1, &m_DepthRbo);
		if (m_FBO) glDeleteFramebuffers(1, &m_FBO);
	}

	void Framebuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_Width, m_Height);
	}

	void Framebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Clear(float r, float g, float b, float a) const {
		Bind();
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Framebuffer::Resize(int width, int height)
	{
		if (width <= 0 || height <= 0) return; // Skips invalid sizes
		if (width == m_Width && height == m_Height) return;

		m_Width = width;
		m_Height = height;

		Bind();

		// Reallocate color texture
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);

		// Reallocate depth/stencil renderbuffer
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		// Check completeness again
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			SHARK_FATAL(Rendering, "Framebuffer::Resize() - Resize failed!");
		}

		Unbind();
	}
}

