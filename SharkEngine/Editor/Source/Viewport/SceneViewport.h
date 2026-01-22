#ifndef SCENE_VIEWPORT_H
#define SCENE_VIEWPORT_H

#include <Source/Interfaces/IViewport.h>
#include <memory>

namespace Shark::Entities	{ class Camera; }
namespace Shark::Graphics	{ class Framebuffer; }

namespace Shark::Editor {
	class SceneViewport : public Shark::Interfaces::IViewport
	{
	public:
		explicit SceneViewport(std::string name);
		~SceneViewport() override;

#pragma region IViewport Contract
		void OnRender(Shark::Scene& scene, Shark::Graphics::Renderer& rend) override;
		unsigned int GetColorAttachment() const override;
		const std::string& GetName() const override;
#pragma endregion

#pragma region Editor Specific
		void SetSize(int width, int height);
		void SetActiveCamera(Shark::Entities::Camera* cam);
		Shark::Entities::Camera* GetActiveCamera() const;
#pragma endregion

	private:
		std::string m_Name;
		std::shared_ptr<Shark::Graphics::Framebuffer> m_Framebuffer;
		Shark::Entities::Camera* m_ActiveCamera{ nullptr };

		int m_Width = 1280;
		int m_Height = 720;
	};
}


#endif // SCENE_VIEWPORT_H