#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

#include <glad.h>
#include "Core/Utilities/Debug.h"

namespace Shark::Graphics {
	class IRenderTarget {
	public:
		virtual ~IRenderTarget() {
			if (m_FBO) glDeleteFramebuffers(1, &m_FBO);
		}

		void BindBuffer() const {
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glViewport(0, 0, m_Width, m_Height);
		}

		static void UnbindBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

		unsigned int GetFBO() const { return m_FBO; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	protected:

		IRenderTarget(int w, int h) : m_Width(w), m_Height(h) {
			glGenFramebuffers(1, &m_FBO);
			SE_LOG(OpenGL, "Created FBO with ID: {}", m_FBO);
		}

		GLuint m_FBO{ 0 };
		int m_Width;
		int m_Height;
	};
}

#endif