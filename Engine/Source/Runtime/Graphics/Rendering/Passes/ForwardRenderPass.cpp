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
    using Components::CameraComponent;
    using Components::MeshRendererComponent;
    using Components::LightData;

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
            glViewport(0, 0, Core::WINDOW_WIDTH, Core::WINDOW_HEIGHT);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void ForwardRenderPass::Execute(float deltaTime, Scene* scene, CameraComponent* cam, std::vector<LightData> lights) {
		glm::mat4 viewMatrix = cam->GetViewMatrix();
		glm::mat4 projectionMatrix = cam->GetProjectionMatrix();

        // Loop trough all rendereables in scene
        for (auto* obj : scene->GetGameObjects()) {
            MeshRendererComponent* meshRenderer = obj->GetComponent<MeshRendererComponent>();
            if (!meshRenderer || !meshRenderer->GetMaterial()) continue;

			Shader* shader = meshRenderer->GetMaterial()->GetShader();
            if (!shader) continue;

            shader->Use();
           

            // --- DIRECTIONAL SHADOWS (Slot 2-5) ---
            for (int i = 0; i < 4; i++) {
                glActiveTexture(GL_TEXTURE2 + i);

                if (i == 0 && m_ShadowMapIDs[i] != 0)
                    glBindTexture(GL_TEXTURE_2D, m_ShadowMapIDs[i]);
                else
                    glBindTexture(GL_TEXTURE_2D, 0);

                shader->SetInt("uShadowMaps[" + std::to_string(i) + "]", 2 + i);
                shader->SetMatrix4("uLightSpaceMatrices[" + std::to_string(i) + "]", m_LightSpaceMatrices[i]);
            }

            //// --- POINT SHADOWS (Slot 6-7) ---
            for (int i = 0; i < MAX_SHADOW_POINT_LIGHTS; i++) {
                glActiveTexture(GL_TEXTURE6 + i);

                if (m_PointShadowMapIDs[i] != 0)
                    glBindTexture(GL_TEXTURE_CUBE_MAP, m_PointShadowMapIDs[i]);
                else
                    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

                shader->SetInt("uPointShadowMaps[" + std::to_string(i) + "]", 6 + i);
            }
            shader->SetFloat("uPointShadowFarPlane", m_PointShadowFarPlane);

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

    void ForwardRenderPass::SetShadowDataAtIndex(int index, unsigned int texID, const glm::mat4& lightSpaceMatrix)
    {
        if (index < 4) {
            m_ShadowMapIDs[index] = texID;
            m_LightSpaceMatrices[index] = lightSpaceMatrix;
        }
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

    void ForwardRenderPass::SetPointShadowDataAtIndex(int index, unsigned int cubemapID, float farPlane)
    {
        if (index < 4) {
            m_PointShadowMapIDs[index] = cubemapID;
            m_PointShadowFarPlane = farPlane;
        }

    }

    void ForwardRenderPass::End()
    {
        if (m_Target) {
            m_Target->UnbindBuffer();
        }
    }
}

