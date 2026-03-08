#include "ForwardRenderPass.h"
#include "Scene/Scene.h"
#include "Core/GameObject.h"
#include "Components/Rendering/MeshRendererComponent.h"
#include "Components/Logic/CameraComponent.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Graphics/Resources/Shader.h"

namespace Shark::Graphics {

    using Shark::Scene;
    using Shark::Components::CameraComponent;
    using Shark::Components::MeshRendererComponent;

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

    void ForwardRenderPass::Execute(float deltaTime, Scene* scene, CameraComponent* cam) {

		glm::mat4 viewMatrix = cam->GetViewMatrix();
		glm::mat4 projectionMatrix = cam->GetProjectionMatrix();

        // Loop trough all rendereables in scene
        for (auto* obj : scene->GetGameObjects()) {
            MeshRendererComponent* meshRenderer = obj->GetComponent<MeshRendererComponent>();
            if (!meshRenderer || !meshRenderer->GetMaterial()) continue;

			Shader* shader = meshRenderer->GetMaterial()->GetShader();
            if (!shader) continue;

            shader->Use();

            shader->SetMatrix4("u_View", viewMatrix);
            shader->SetMatrix4("u_Projection", projectionMatrix);
			shader->SetVector3("u_ViewPos", cam->GetOwner()->GetTransform().position); // Passes Camera Position (for specular highlights)

			// Pass Light Data (Ideally these would be passed from a LightComponent, but for now we'll hardcode a single directional light)
			shader->SetVector3("u_LightDir", glm::vec3(-0.2f, -1.0f, -0.3f)); // Directional light direction);
			shader->SetVector3("u_LightColor", glm::vec3(1.0f)); // White light
             
            meshRenderer->Render(); // Draws Mesh
        }
    }

    void ForwardRenderPass::End()
    {
        if (m_Target) {
            m_Target->Unbind();
        }
    }
}

