#ifndef RENDERABLE_H
#define RENDERABLE_H

namespace Shark::Graphics {

	class IRenderable
	{
	public:
		virtual ~IRenderable() = default;
		virtual void Draw() const = 0;
	};

}

#endif