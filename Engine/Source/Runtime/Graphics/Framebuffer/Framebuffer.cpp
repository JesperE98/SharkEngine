#include "Framebuffer.h"

namespace Shark::Graphics {

	Framebuffer::Framebuffer(int width, int height) : RenderTarget(width, height) {
		Invalidate(width, height);
	}

	Framebuffer::~Framebuffer() {
		if (m_ColorTexture) glDeleteTextures(1, &m_ColorTexture);
		if (m_DepthRbo) glDeleteRenderbuffers(1, &m_DepthRbo);
	}

	void Framebuffer::Invalidate(int width, int height)
	{
		if (m_ColorTexture) {
			glDeleteTextures(1, &m_ColorTexture);
			glDeleteRenderbuffers(1, &m_DepthRbo);
		}

		BindBuffer();

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
			SE_FAT(Rendering, "Framebuffer::Framebuffer() - not complete!");
		}

		UnbindBuffer();
	}

	void Framebuffer::Clear(float r, float g, float b, float a) const {
		BindBuffer();
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Framebuffer::Resize(int width, int height)
	{
		if (width <= 0 || height <= 0 || width == m_Width && height == m_Height) return; // Skips invalid sizes

		m_Width = width;
		m_Height = height;

		Invalidate(width, height);
	}

}

