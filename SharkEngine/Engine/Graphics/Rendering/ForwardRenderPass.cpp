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
        if (shader) {
            delete shader;
            shader = nullptr;
        }
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

        if (!cam) return;

		glm::mat4 viewMatrix = cam->GetViewMatrix();
		glm::mat4 projectionMatrix = cam->GetProjectionMatrix();

        // Loop trough all rendereables in scene
        for (auto* obj : scene->GetGameObjects()) {
            auto* meshRenderer = obj->GetComponent<MeshRendererComponent>();
            if (!meshRenderer) continue;

            Material* mat = meshRenderer->GetMaterial();
            if (!mat) continue;

            Shader* shader = mat->GetShader();
            if (!shader) continue;

            shader->Use();
            shader->SetMatrix4("view", viewMatrix);
            shader->SetMatrix4("projection", projectionMatrix);

            // Per-object
            shader->SetMatrix4("model", obj->GetTransform().GetModelMatrix());

            mat->Bind(); // Binds textures, sets material uniforms
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

