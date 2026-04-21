#ifndef SKY_PASS_H
#define SKY_PASS_H

#include "RenderPass.h"

namespace Shark::Graphics {
	class SkyPass : public RenderPass {
	public:
		SkyPass(int width, int height);
		~SkyPass();

		void Begin() override final;
		void End() override final;

		void Execute(Scene* scene, const std::vector<Components::LightData>& light) override final;
		void SetCameraData(const glm::mat4& invView, const glm::mat4& invProjection);

		void SetSunColor(const glm::vec3& color) { m_SunColor = color; }
		void SetSunSize(float size) { m_SunSize = size; }

	private:
		QuadBuffer* m_QuadBuffer{ nullptr };

		glm::mat4 m_InvView			= glm::mat4(1.0f);
		glm::mat4 m_InvProjection	= glm::mat4(1.0f);
		glm::vec3 m_SunColor		= glm::vec3(1.0f, 0.95f, 0.8f);
		float m_SunSize				= 0.9995f;

		unsigned int m_QuadVAO = 0;
		unsigned int m_QuadVBO = 0;


		// Called to render scene
		void Execute(
			float deltaTime,
			Shark::Scene* scene,
			Components::CameraComponent* cam,
			std::vector<Components::LightData> lights) override final {};

		void UpdateCameraTransform(
			Graphics::Shader* shader,
			Components::CameraComponent* cam,
			glm::mat4& view,
			glm::mat4& projection) override final {};

		void UpdateLights(
			Graphics::Shader* shader,
			std::vector<Components::LightData>& lights) override final {};
	};
}
#endif // SKY_PASS_H