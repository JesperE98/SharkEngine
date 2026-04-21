#ifndef POINT_SHADOW_BUFFER_H
#define POINT_SHADOW_BUFFER_H

#include "RenderTarget.h"

namespace Shark::Graphics {
	class PointShadowBuffer : RenderTarget {
	public:
		PointShadowBuffer(int resolution);
		~PointShadowBuffer() override;

		unsigned int GetCubemap() const { return m_CubemapTexture; }

	private:
		unsigned int m_CubemapTexture{ 0 };
		void CreateCubemapBuffer(int resolution);
	};
}


#endif