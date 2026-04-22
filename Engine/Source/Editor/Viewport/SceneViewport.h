#ifndef SCENE_VIEWPORT_H
#define SCENE_VIEWPORT_H

#include <Interfaces/IViewport.h>
#include <memory>

namespace Shark::Components		{ class CameraComponent; }
namespace Shark::Graphics		{ class Framebuffer; }

namespace Shark::Editor {
	class SceneViewport : public Interfaces::IViewport
	{
	public:
		explicit SceneViewport(std::string name);
		~SceneViewport() override = default;

#pragma region IViewport Contract
		void OnInitialize() override;
		void OnRender(Shark::Scene* scene, Graphics::Renderer* rend) override;
		unsigned int GetColorAttachment() const override;
		const std::string& GetName() const override;
#pragma endregion

#pragma region Editor Specific
		void SetSize(int width, int height);
		void SetActiveCamera(Components::CameraComponent* cam);
		void IsHovered(bool value) { m_bIsHovered = value; }
		void IsFocused(bool value) { m_bIsFocused = value; }
		Components::CameraComponent* GetActiveCamera() const;
#pragma endregion

	private:
		bool m_bIsHovered = false;
		bool m_bIsFocused = false;
		std::string m_Name;
		std::shared_ptr<Graphics::Framebuffer> m_Framebuffer;
		Components::CameraComponent* m_ActiveCamera{ nullptr };

		int m_Width = 1280;
		int m_Height = 720;
	};
}


#endif // SCENE_VIEWPORT_H