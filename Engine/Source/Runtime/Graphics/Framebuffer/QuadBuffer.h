#ifndef QUAD_BUFFER_H
#define QUAD_BUFFER_H

#include "RenderTarget.h"

namespace Shark::Graphics {
	class QuadBuffer : RenderTarget {
	public:
		QuadBuffer(int w, int h);
		~QuadBuffer() override;

		void Draw() const;

	private:
		unsigned int m_VAO{ 0 };
		unsigned int m_VBO{ 0 };

		void CreateQuad();
	};
}

#endif // QUAD_BUFFER_H
