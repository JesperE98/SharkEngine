#include "ForwardRenderPass.h"
#include "Core/Engine/Scene.h"
#include "Core/Entities/GameObject.h"
#include "Core/Components/MeshRendererComponent.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Graphics/Resources/Shader.h"

ForwardRenderPass::ForwardRenderPass(Framebuffer* target) 
    : RenderPass(target) {}

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

void ForwardRenderPass::Execute(float deltaTime, Scene* scene, Camera* cam) {
    
    if (!cam) return;

	// Loop trough all rendereables in scene
    for (auto* obj : scene->GetGameObjects()) {
        auto* meshRenderer = obj->GetComponent<MeshRendererComponent>();
        if (!meshRenderer) continue;

        Material* mat = meshRenderer->GetMaterial();
        if (!mat) continue;

        Shader* shader = mat->GetShader();
        if (!shader) continue;

        shader->Use();
        shader->SetMatrix4("view", cam->GetViewMatrix());
        shader->SetMatrix4("projection", cam->GetProjectionMatrix());

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
