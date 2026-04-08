#ifndef SHADOW_MAP_PASS_H
#define SHADOW_MAP_PASS_H

#include "RenderPass.h"
#include <memory>

namespace Shark::Graphics {

	class ShadowMapPass : public RenderPass
	{
	public:
		ShadowMapPass(int resolution = 2048);
		~ShadowMapPass();

		void Begin() override final;
		void End() override final;

		void Execute(Shark::Scene* scene, const std::vector<Shark::Components::LightData>& lights) override;

		void SetLightSpaceMatrix(int index, const glm::mat4& matrix);
		glm::mat4 GetLightSpaceMatrix(int index) const;

		void SetShadowMapTexture();
		unsigned int GetShadowMapTexture(int index) const;

	private:

		int m_Resolution = 2048;
		std::unique_ptr<ShadowBuffer> m_ShadowBuffers[4];
		glm::mat4 m_LightSpaceMatrices[4];
		int m_ActiveShadowCount{ 0 };

		void Execute(float dt, Shark::Scene* s, Shark::Components::CameraComponent* c, std::vector<Shark::Components::LightData> l) override final {}
		virtual void UpdateCameraTransform(Shark::Graphics::Shader* shader, Shark::Components::CameraComponent* cam, glm::mat4& view, glm::mat4& projection) override final { }
		virtual void UpdateLights(Shark::Graphics::Shader* shader, std::vector<Shark::Components::LightData>& lights) override final { }
	};
}


#endif