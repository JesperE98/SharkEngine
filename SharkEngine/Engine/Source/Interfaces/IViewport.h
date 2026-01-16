#ifndef VIEWPORT_H
#define VIEWPORT_H

class Scene;
class Renderer;

#include <string>

class IViewport {
public:
	virtual ~IViewport() = default;
	/* Render the scene into this viewport using a renderer. */
	virtual void OnRender(Scene& scene, Renderer& rend) = 0;
	/* Return the raw GPU texture handle (OpenGL GLuint). */
	virtual unsigned int GetColorAttachment() const = 0;
	/* Name for identification (Scene, Game, etc...). */
	virtual const std::string& GetName() const = 0;
};

#endif // VIEWPORT_H