#ifndef RENDERABLE_H
#define RENDERABLE_H

namespace Shark::Interfaces {

	class IRenderable
	{
	public:
		virtual ~IRenderable() = default;
		virtual void Draw() const = 0;
	};

}

#endif