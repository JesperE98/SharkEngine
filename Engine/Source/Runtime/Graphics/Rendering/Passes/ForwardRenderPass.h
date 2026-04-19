#ifndef FORWARD_RENDER_PASS_H
#define FORWARD_RENDER_PASS_H

#include "RenderPass.h"


namespace Shark::Graphics
{
	class ForwardRenderPass : public RenderPass
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

		void UpdateCameraTransform(
			Shark::Graphics::Shader* shader,
			Shark::Components::CameraComponent* cam,
			glm::mat4& view,
			glm::mat4& projection) override final;

		void SetShadowDataAtIndex(int index, unsigned int texID, const glm::mat4& lightSpaceMatrix);
		void UpdateLights(Shark::Graphics::Shader* shader, std::vector<Shark::Components::LightData>& lights) override final;

		void SetPointShadowDataAtIndex(int index, unsigned int cubemapID, float farPlane);
		void End() override final;

		void SetTarget(Framebuffer* fb);

	private:
		unsigned int m_ShadowMapIDs[MAX_DIR_SHADOWS] = {};
		glm::mat4 m_LightSpaceMatrices[MAX_DIR_SHADOWS];

		unsigned int m_PointShadowMapIDs[MAX_SHADOW_POINT_LIGHTS] = {};
		float m_PointShadowFarPlane = 100.0f;


		void Execute(Shark::Scene* scene, const std::vector<Shark::Components::LightData>& mainLight) override final {}
	};
}


#endif