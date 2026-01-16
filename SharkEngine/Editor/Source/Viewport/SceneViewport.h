#ifndef SCENE_VIEWPORT_H
#define SCENE_VIEWPORT_H

#include <Source/Interfaces/IViewport.h>
#include <string>
#include <memory>

class Scene;
class Camera;
class Renderer;
class Framebuffer;

class SceneViewport : public IViewport
{
public:
	explicit SceneViewport(std::string name);
	~SceneViewport() override;

#pragma region IViewport Contract
	void OnRender(Scene& scene, Renderer& rend) override;
	unsigned int GetColorAttachment() const override;
	const std::string& GetName() const override;
#pragma endregion

#pragma region Editor Specific
	void SetSize(int width, int height);
	void SetActiveCamera(Camera* cam);
	Camera* GetActiveCamera() const;
#pragma endregion

private:
	std::string m_Name;
	std::shared_ptr<Framebuffer> m_Framebuffer;
	Camera* m_ActiveCamera{ nullptr };

	int m_Width = 1280;
	int m_Height = 720;
};

#endif // SCENE_VIEWPORT_H