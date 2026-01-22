#pragma once

#include <glad/glad.h>

namespace Shark::Graphics {
	class Framebuffer
	{
	public:
		Framebuffer(int width, int height);
		~Framebuffer();

		void Bind() const;
		static void Unbind();
		void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) const;
		void Resize(int width, int height);
		GLuint GetColorTexture() const { return m_ColorTexture; }

	private:

		GLuint m_FBO{ 0 };
		GLuint m_ColorTexture{ 0 };
		GLuint m_DepthRbo{ 0 };
		int m_Width, m_Height;
	};
}