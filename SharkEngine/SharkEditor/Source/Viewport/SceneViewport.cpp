#include "SceneViewport.h"
#include <Core/Engine/Scene.h>
#include <Core/Entities/Camera.h>
#include <Core/Utilities/Time.h>
#include <Source/Graphics/Rendering/Renderer.h>
#include <Source/Graphics/Framebuffer/Framebuffer.h>
#include <Source/Graphics/Rendering/ForwardRenderer.h>



SceneViewport::SceneViewport(std::string name)
    : m_Name(std::move(name))
{
    m_Framebuffer = std::make_shared<Framebuffer>(m_Width, m_Height);
}

SceneViewport::~SceneViewport() = default;

void SceneViewport::OnRender(Scene& scene, Renderer& rend)
{   
    ForwardRenderer* fr = dynamic_cast<ForwardRenderer*>(&rend);
    if (fr) {
        fr->SetTarget(m_Framebuffer.get());
    }

    Camera* cam = m_ActiveCamera ? m_ActiveCamera : scene.GetCamera();
    if (cam) {
        m_ActiveCamera = cam;

        rend.BeginFrame();
        rend.RenderScene(static_cast<float>(Time::GetDeltaTime()), &scene, cam);
        rend.EndFrame();
    }

   
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

void SceneViewport::SetActiveCamera(Camera* cam)
{
    m_ActiveCamera = cam;
}

Camera* SceneViewport::GetActiveCamera() const
{
    return m_ActiveCamera;
}
