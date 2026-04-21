#ifndef SHADOW_BUFFER_H
#define SHADOW_BUFFER_H

#include "RenderTarget.h"

namespace Shark::Graphics {
	class ShadowBuffer : virtual public RenderTarget {
	public:
		ShadowBuffer(int w, int h);
		~ShadowBuffer() override;

		unsigned int GetDepthTexture() const;

	private:
		unsigned int m_DepthTexture{ 0 };

		/*
		@brief Creates a shadowbuffer.
		*/
		void CreateShadowBuffer(int width, int height);
	};
}
#endif
