#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>
#include <glm/glm.hpp>

namespace Shark { class Scene; }
namespace Shark::Components { class CameraComponent; struct LightData; }

namespace Shark::Graphics {
	class Shader;
	class Framebuffer;
	class ShadowBuffer;

	class RenderPass
	{
	public:
		explicit RenderPass(Framebuffer* target = nullptr) : m_Target(target) {}
		virtual ~RenderPass() = default;

		// Called before drawing (bind FBO, clear, set state)
		virtual void Begin() = 0;

		// Called to render scene
		virtual void Execute(
			float deltaTime, 
			Shark::Scene* scene, 
			Shark::Components::CameraComponent* cam, 
			std::vector<Shark::Components::LightData> lights) = 0;
		virtual void Execute(Scene* scene, const Shark::Components::LightData& mainLight) = 0;

		virtual void UpdateCameraTransform(
			Shark::Graphics::Shader* shader,
			Shark::Components::CameraComponent* cam,
			glm::mat4& view,
			glm::mat4& projection) = 0;

		virtual void UpdateLights(
			Shark::Graphics::Shader* shader, 
			std::vector<Shark::Components::LightData>& lights) = 0;

		// Called after drawing (unbind FBO, reset state)
		virtual void End() = 0;
	protected:
		Shader* shader{ nullptr };
		Framebuffer* m_Target{ nullptr };
	};
}

#endif