#include "ForwardRenderPass.h"
#include "Scene/Scene.h"
#include "Core/GameObject.h"
#include "Components/Rendering/MeshRendererComponent.h"
#include "Components/Rendering/LightComponent.h"
#include "Components/Logic/CameraComponent.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Graphics/Resources/Shader.h"
#include "Core/Engine/Engine.h"

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
            m_Target->BindBuffer(); // This calls glViewport(0, 0, m_Width, m_Height) internally!
            m_Target->Clear(0.1f, 0.1f, 0.1f, 1.0f);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Add this line to fix the "Tiny Scene" issue
            glViewport(0, 0, Shark::Core::WINDOW_WIDTH, Shark::Core::WINDOW_HEIGHT);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void ForwardRenderPass::Execute(float deltaTime, Scene* scene, CameraComponent* cam, std::vector<Shark::Components::LightData> lights) {
		glm::mat4 viewMatrix = cam->GetViewMatrix();
		glm::mat4 projectionMatrix = cam->GetProjectionMatrix();

        if(m_ShadowMapID != 0) {
            // If no shadow map, we still need to point the sampler somewhere safe 
            // or the shader will error out trying to read unit 0

        }

        // Loop trough all rendereables in scene
        for (auto* obj : scene->GetGameObjects()) {
            MeshRendererComponent* meshRenderer = obj->GetComponent<MeshRendererComponent>();
            if (!meshRenderer || !meshRenderer->GetMaterial()) continue;

			Shader* shader = meshRenderer->GetMaterial()->GetShader();
            if (!shader) continue;

            shader->Use();

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_ShadowMapID);
            shader->SetInt("uShadowMap", 2);
            shader->SetMatrix4("uLightSpaceMatrix", m_LightSpaceMatrix);

            
            UpdateCameraTransform(shader, cam, viewMatrix, projectionMatrix);
            UpdateLights(shader, lights);

            meshRenderer->Render(); // Draws Mesh
        }
    }

    void ForwardRenderPass::UpdateCameraTransform(Shader* shader, CameraComponent* cam,
        glm::mat4& view, glm::mat4& projection)
    {
        shader->SetMatrix4("uView", view);
        shader->SetMatrix4("uProjection", projection);
        shader->SetVector3("uViewPos", cam->GetOwner()->GetTransform().position); // Passes Camera Position (for specular highlights)
    }

    void ForwardRenderPass::SetShadowData(unsigned int textureID, const glm::mat4& lightSpaceMatrix)
    {
        m_ShadowMapID = textureID;
        m_LightSpaceMatrix = lightSpaceMatrix;
    }

    void ForwardRenderPass::UpdateLights(Shader* shader, std::vector<LightData>& lights)
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
            m_Target->UnbindBuffer();
        }
    }
}

