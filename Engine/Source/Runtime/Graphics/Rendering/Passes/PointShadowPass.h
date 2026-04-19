#ifndef POINT_SHADOW_PASS_H
#define POINT_SHADOW_PASS_H

#include "RenderPass.h"
#include <array>

namespace Shark::Graphics {

	class PointShadowBuffer;

	class PointShadowPass : public RenderPass {
	public:
		PointShadowPass(int resolution = 1024);
		~PointShadowPass();

		void Begin() override final;
		void End() override final;
		void Execute(Shark::Scene* scene, std::vector<Shark::Components::LightData>& lights);

		unsigned int GetCubemap(int index) const;
		float GetFarPlane() const { return m_FarPlane; }

	private:
		int m_Resolution;
		float m_FarPlane = 25.0f;

		std::array<PointShadowBuffer*, MAX_SHADOW_POINT_LIGHTS>  m_Buffers;

		void RenderPointLight(Shark::Scene* scene, const Shark::Components::LightData& light, int index);
		
		void Execute(
			float deltaTime,
			Shark::Scene* scene,
			Shark::Components::CameraComponent* cam,
			std::vector<Shark::Components::LightData> lights) override final {};

		void UpdateCameraTransform(
			Shark::Graphics::Shader* shader,
			Shark::Components::CameraComponent* cam,
			glm::mat4& view,
			glm::mat4& projection) override final {};

		void UpdateLights(
			Shark::Graphics::Shader* shader,
			std::vector<Shark::Components::LightData>& lights) override final {};
		void Execute(Shark::Scene* scene, const std::vector<Shark::Components::LightData>& mainLight) override final {}

	};
}


#endif