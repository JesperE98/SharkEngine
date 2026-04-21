#pragma once

#include "RenderTarget.h"

namespace Shark::Graphics {
	class Framebuffer : public RenderTarget
	{
	public:
		Framebuffer(int width, int height);
		~Framebuffer() override;

		void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) const;
		void Resize(int width, int height);

		unsigned int GetColorTexture() const { return m_ColorTexture; }

	private:
		void Invalidate(int width, int height);

		GLuint m_ColorTexture{ 0 };
		GLuint m_DepthRbo{ 0 };
		unsigned int m_ID{ 0 };				// The "Container" ID
	};
}