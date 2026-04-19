#include "PointShadowBuffer.h"

namespace Shark::Graphics {
	PointShadowBuffer::PointShadowBuffer(int resolution) : IRenderTarget(resolution, resolution)
	{
		CreateCubemapBuffer(resolution);
	}

	PointShadowBuffer::~PointShadowBuffer()
	{
		if (m_CubemapTexture) glDeleteBuffers(1, &m_CubemapTexture);
	}

	void PointShadowBuffer::CreateCubemapBuffer(int resolution)
	{
		BindBuffer();

		glGenTextures(1, &m_CubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);


		// Creates all 6 faces
		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				GL_DEPTH_COMPONENT, resolution, resolution,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Attachg cubemap FBO
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_CubemapTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			SE_FAT(Rendering, "PointShadowBuffer is not complete!");
		}

		UnbindBuffer();
	}
}
