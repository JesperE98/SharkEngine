#include "SceneViewport.h"

#pragma region Engine Includes
#include <Scene/Scene.h>
#include <Graphics/Rendering/Renderer.h>
#include <Graphics/Framebuffer/Framebuffer.h>
#include <Graphics/Rendering/ForwardRenderer.h>
#include <Components/Logic/CameraComponent.h>
#pragma endregion

#include <ImGui/imgui.h>

namespace Shark::Editor {

    using Components::CameraComponent;
    using Graphics::Renderer;
    using Graphics::ForwardRenderer;
    using Graphics::Framebuffer;
    using Core::GameObject;
    using Core::Time;
    using Shark::Scene;

    SceneViewport::SceneViewport(std::string name)
        : m_Name(std::move(name))
    {
    }

    void SceneViewport::OnInitialize()
    {
        m_Framebuffer = std::make_shared<Framebuffer>(m_Width, m_Height);
    }

    void SceneViewport::OnRender(Scene* scene, Renderer* rend)
    {
        ForwardRenderer* fr = dynamic_cast<ForwardRenderer*>(rend);
        if (fr) {
            fr->SetTarget(m_Framebuffer.get());
        }

        CameraComponent* cam = scene->GetCamera();

        if (!cam) {
            ImGui::TextDisabled("No active camera in scene.");
            return;
        }

        m_ActiveCamera = cam;

        rend->OnBeginFrame();
        rend->OnRenderScene(Time::GetDeltaTime(), scene, m_ActiveCamera);
        rend->OnEndFrame();
    }

    unsigned int SceneViewport::GetColorAttachment() const
    {
        return m_Framebuffer->GetColorTexture();
    }

    const std::string& SceneViewport::GetName() const
    {
        return m_Name;
    }

    void SceneViewport::SetSize(int width, int height)
    {
        if (width == m_Width && height == m_Height) return;
        m_Width = width;
        m_Height = height;
        m_Framebuffer->Resize(width, height);

        // Update active camera projection
        if (m_ActiveCamera) {
            float aspect = static_cast<float>(width) / static_cast<float>(height);
            m_ActiveCamera->SetAspectRatio(aspect);
            m_ActiveCamera->UpdateProjectionMatrix();
        }
    }

    void SceneViewport::SetActiveCamera(CameraComponent* cam)
    {
        m_ActiveCamera = cam;
    }

    CameraComponent* SceneViewport::GetActiveCamera() const
    {
        return m_ActiveCamera;
    }

}