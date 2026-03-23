#ifndef FORWARD_RENDER_PASS_H
#define FORWARD_RENDER_PASS_H

#include "RenderPass.h"

namespace Shark::Graphics
{
	class ForwardRenderPass : virtual public Shark::Graphics::RenderPass
	{
	public:
		explicit ForwardRenderPass(Framebuffer* target);
		~ForwardRenderPass() override final;

		void Begin() override final;
		void Execute(
			float deltaTime, 
			Shark::Scene* scene, 
			Shark::Components::CameraComponent* cam,
			std::vector<Shark::Components::LightData> lights) override final;
		void Execute(Shark::Scene* scene, const Shark::Components::LightData& mainLight) {}

		void UpdateCameraTransform(
			Shark::Graphics::Shader* shader,
			Shark::Components::CameraComponent* cam,
			glm::mat4& view,
			glm::mat4& projection) override final;

		void SetShadowData(unsigned int textureID, const glm::mat4& lightSpaceMatrix);
		void UpdateLights(Shark::Graphics::Shader* shader, std::vector<Shark::Components::LightData>& lights) override final;

		void End() override final;

		void SetTarget(Framebuffer* fb);

	private:
		unsigned int m_ShadowMapID{ 0 };
		glm::mat4 m_LightSpaceMatrix = glm::mat4(1.0f);
	};
}


#endif