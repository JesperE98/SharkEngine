#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

#include "Core/Utilities/Debug.h"
#include <glad/glad.h>

namespace Shark::Graphics {
	class RenderTarget {
	public:
		virtual ~RenderTarget();

		void BindBuffer() const;
		static void UnbindBuffer();

		unsigned int GetFBO() const { return m_FBO; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	protected:

		RenderTarget(int w, int h);

		GLuint m_FBO{ 0 };
		int m_Width;
		int m_Height;
	};
}

#endif