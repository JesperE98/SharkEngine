#include "ShadowBuffer.h"

namespace Shark::Graphics {
	ShadowBuffer::ShadowBuffer(int w, int h) : RenderTarget(w, h)
	{
		CreateShadowBuffer(w, h);
	}

	ShadowBuffer::~ShadowBuffer()
	{
		if (m_DepthTexture) glDeleteTextures(1, &m_DepthTexture);
	}

	unsigned int ShadowBuffer::GetDepthTexture() const
	{
		return m_DepthTexture;
	}

	void ShadowBuffer::CreateShadowBuffer(int width, int height)
	{
		BindBuffer();

		// Creates the Depth Texture
		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); // Use GL_GEPTH_COMPONENT as the format

		// Essential filters for Shadows
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

		// Tell OpenGL we aren't rendering colors
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		//// 6. Check for success
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			SE_FAT(Rendering, "ShadowBuffer is not complete!");
		}

		//m_DepthTexture = m_FBO;
		UnbindBuffer();
	}
}

