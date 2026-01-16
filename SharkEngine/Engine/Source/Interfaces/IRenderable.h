#ifndef RENDERABLE_H
#define RENDERABLE_H

class IRenderable
{
public:
	virtual ~IRenderable() = default;
	virtual void Draw() const = 0;
};

#endif