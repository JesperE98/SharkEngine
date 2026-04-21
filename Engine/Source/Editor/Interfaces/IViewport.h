#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <string>

namespace Shark { class Scene; }
namespace Shark::Graphics { class Renderer; }

namespace Shark::Interfaces {

	class IViewport {
	public:
		virtual ~IViewport() = default;
		/* Initialize any resources needed for this viewport (e.g., framebuffer, textures). */
		virtual void OnInitialize() = 0;
		/* Render the scene into this viewport using a renderer. */
		virtual void OnRender(Shark::Scene& scene, Graphics::Renderer& rend) = 0;
		/* Return the raw GPU texture handle (OpenGL GLuint). */
		virtual unsigned int GetColorAttachment() const = 0;
		/* Name for identification (Scene, Game, etc...). */
		virtual const std::string& GetName() const = 0;
	};
}


#endif // VIEWPORT_H