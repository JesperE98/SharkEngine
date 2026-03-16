#include "ForwardRenderPass.h"
#include "Scene/Scene.h"
#include "Core/GameObject.h"
#include "Components/Rendering/MeshRendererComponent.h"
#include "Components/Rendering/LightComponent.h"
#include "Components/Logic/CameraComponent.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Graphics/Resources/Shader.h"

namespace Shark::Graphics {

    using Shark::Scene;
    using Shark::Components::CameraComponent;
    using Shark::Components::MeshRendererComponent;
    using Shark::Components::LightData;

    ForwardRenderPass::ForwardRenderPass(Framebuffer* target)
        : RenderPass(target) {
    }

    ForwardRenderPass::~ForwardRenderPass()
    {
    }

    void ForwardRenderPass::SetTarget(Framebuffer* fb) { m_Target = fb; }

    void ForwardRenderPass::Begin() {
        // Clear screen and depth buffers
        if (m_Target) {
            m_Target->Bind();
            m_Target->Clear(0.1f, 0.1f, 0.1f, 1.0f);
        }
    }

    void ForwardRenderPass::Execute(float deltaTime, Scene* scene, CameraComponent* cam, std::vector<Shark::Components::LightData> lights) {

		glm::mat4 viewMatrix = cam->GetViewMatrix();
		glm::mat4 projectionMatrix = cam->GetProjectionMatrix();

        // Loop trough all rendereables in scene
        for (auto* obj : scene->GetGameObjects()) {
            MeshRendererComponent* meshRenderer = obj->GetComponent<MeshRendererComponent>();
            if (!meshRenderer || !meshRenderer->GetMaterial()) continue;

			Shader* shader = meshRenderer->GetMaterial()->GetShader();
            if (!shader) continue;

            shader->Use();

            UpdateCameraTransform(shader, cam, viewMatrix, projectionMatrix);
            UpdateLights(shader, lights);

            meshRenderer->Render(); // Draws Mesh
        }
    }

    void ForwardRenderPass::UpdateCameraTransform(Shark::Graphics::Shader* shader,Shark::Components::CameraComponent* cam,
        glm::mat4& view, glm::mat4& projection)
    {
        shader->SetMatrix4("uView", view);
        shader->SetMatrix4("uProjection", projection);
        shader->SetVector3("uViewPos", cam->GetOwner()->GetTransform().position); // Passes Camera Position (for specular highlights)
    }

    void ForwardRenderPass::UpdateLights(Shader* shader, std::vector<Shark::Components::LightData>& lights)
    {
        shader->SetInt("uLightCount", static_cast<int>(lights.size()));

        for (int i = 0; i < lights.size(); i++) {
            std::string base = "uLights[" + std::to_string(i) + "].";
            shader->SetInt(base +       "type",         lights[i].type);
            shader->SetVector3(base +   "position",     lights[i].position);
            shader->SetVector3(base +   "direction",    lights[i].direction);
            shader->SetVector3(base +   "color",        lights[i].color);
            shader->SetFloat(base +     "intensity",    lights[i].intensity);
            shader->SetFloat(base +     "range",        lights[i].range);
        }
    }

    void ForwardRenderPass::End()
    {
        if (m_Target) {
            m_Target->Unbind();
        }
    }
}

