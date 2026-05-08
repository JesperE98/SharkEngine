#include "ForwardRenderer.h"
#include "Core/Engine/Engine.h"
#include "Passes/ForwardRenderPass.h"
#include "Passes/ShadowMapPass.h"
#include "Passes/PointShadowPass.h"
#include "Passes/SkyPass.h"
#include "Core/GameObject.h"
#include "Core/Utilities/DebugRenderer.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Components/Rendering/LightComponent.h"
#include "Components/Logic/CameraComponent.h"
#include "Components/Physics/AABBComponent.h"
#include "Scene/Scene.h"
#include "Math/Frustum.h"


namespace Shark::Graphics {

	using Shark::Scene;
	using Components::CameraComponent;
	using Components::LightComponent;
	using Components::LightData;
	using Components::AABB;
	using Components::AABBComponent;
	using Graphics::ShadowBuffer;
	using Graphics::SkyPass;
	using Core::GameObject;
	using Math::Vector3;
	using Core::DebugRenderer;

	using namespace Shark::Spatial;

	inline Vector3 GetOctreeDepthColor(int depth) {
		switch (depth) {
			case 0:		return { 1.0f, 0.0f, 0.0f }; // red (root)
			case 1:		return { 1.0f, 0.5f, 0.0f }; // orange
			case 2:		return { 1.0f, 1.0f, 0.0f }; // yellow
			case 3:		return { 0.0f, 1.0f, 0.0f }; // green
			case 4:		return { 0.0f, 1.0f, 1.0f }; // cyan
			default:	return { 0.5f, 0.5f, 1.0f }; // light blue
		}
	}

	ForwardRenderer::ForwardRenderer()
	{
		OnInitialize();
	}

	ForwardRenderer::~ForwardRenderer()
	{
		if (m_SceneFb)			delete m_SceneFb;
		if (m_ForwardPass)		delete m_ForwardPass;
		if (m_ShadowPass)		delete m_ShadowPass;
		if (m_SkyPass)			delete m_SkyPass;
		if (m_PointShadowPass)	delete m_PointShadowPass;

		m_SceneFb			= nullptr;
		m_ShadowFb			= nullptr;
		m_ForwardPass		= nullptr;
		m_ShadowPass		= nullptr;
		m_SkyPass			= nullptr;
		m_PointShadowPass	= nullptr;
	}

	void ForwardRenderer::OnInitialize()
	{
		SE_PROC(Rendering, "Initializing ForwardRenderer...");

		m_SceneFb = new Framebuffer(Core::WINDOW_WIDTH, Core::WINDOW_HEIGHT);

		
		m_ShadowPass = new ShadowMapPass(2048); // Creating Shadow pass (High Resolution for crisp shadows)
		m_PointShadowPass = new PointShadowPass(1024);
		m_SkyPass = new SkyPass(Core::WINDOW_WIDTH, Core::WINDOW_HEIGHT);
		m_ForwardPass = new ForwardRenderPass(m_SceneFb); // Creating Forward Pass and adding it to list

		DebugRenderer::Get().Init();
		SE_LOG(Rendering, "ForwardRenderer initialized successfully.");
		//SE_LOG(Rendering, "Amount of passes in RenderPasses: {}", renderPasses.size());
	}

	void ForwardRenderer::OnBeginFrame()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	void ForwardRenderer::OnRenderScene(float deltaTime, Scene* scene, CameraComponent* cam)
	{
		std::vector<LightData> sceneLights;
		for (auto* obj : scene->GetGameObjects()) {
			if (!obj || obj->bMarkedForDeletion) continue;

			if (auto* lightComp = obj->GetComponent<LightComponent>()) {
				sceneLights.push_back(lightComp->GetLightData());
			}
		}

		if (!sceneLights.empty()) {
			m_ShadowPass->Begin();
			m_ShadowPass->Execute(scene, sceneLights);
			m_ShadowPass->End();

			m_PointShadowPass->Begin();
			m_PointShadowPass->Execute(scene, sceneLights);
			m_PointShadowPass->End();
		}

		// Sky should always render
		glm::mat4 view			= cam->GetViewMatrix();
		glm::mat4 projection	= cam->GetProjectionMatrix();
		m_SkyPass->SetCameraData(glm::inverse(view), glm::inverse(projection));
		m_SkyPass->Begin();
		m_SkyPass->Execute(scene, sceneLights);
		m_SkyPass->End();
	

		// Forward pass always renders
		m_ForwardPass->Begin();

		// --- DIRECTIONAL SHADOW MAPS
		for (int i = 0; i < 4; ++i) {
			m_ForwardPass->SetShadowDataAtIndex(
				i, 
				m_ShadowPass->GetShadowMapTexture(i), 
				m_ShadowPass->GetLightSpaceMatrix(i));
		}
		
		// --- POINT SHADOW CUBEMAP
		for (int i = 0; i < MAX_SHADOW_POINT_LIGHTS; ++i) {
			m_ForwardPass->SetPointShadowDataAtIndex(
				i,
				m_PointShadowPass->GetCubemap(i),
				m_PointShadowPass->GetFarPlane()
			);
		}

		// ====== FRUSTU CULLING =======
		// Build frustum from camera
		Math::Frustum frustum;
		glm::mat4 vp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
		frustum.ExtractFromViewProjection(vp);

		// Query octree for visible objects
		auto visibleObjects = OctreeSystem::Get().QueryFrustum(frustum);
		//std::vector<Core::GameObject*> visibleObjects;

		// Include objects not in the octree (no AABB = always visible)
		for (auto* obj : scene->GetGameObjects()) {
			if (!obj->GetComponent<AABBComponent>() && !obj->bMarkedForDeletion) {
				visibleObjects.push_back(obj);
			}
		}

		// Track stats for debug display
		m_DrawnCount = static_cast<int>( visibleObjects.size() );
		m_CulledCount = static_cast<int>( scene->GetGameObjects().size() ) - m_DrawnCount;

		m_ForwardPass->Execute(deltaTime, scene, cam, { sceneLights }, &visibleObjects);

		// TEMPORARY: Build Octree from scene AABBs and draw wireframe
		if (OctreeSystem::Get().IsDebugDrawEnabled()) {
			OctreeSystem::Get().ForEachNodeBoundsWithDepth([](const AABB& b, int depth) {
				Vector3 color = GetOctreeDepthColor(depth);
				DebugRenderer::Get().AddAABB(b.min, b.max, color);
														   });
		}
		// === END TEMPORARY ===

		// Debug AABBs
		for (GameObject* obj : scene->GetGameObjects()) {
			AABBComponent* aabb = obj->GetComponent<AABBComponent>();
			if (!aabb) continue;

			const AABB& worldAABB = aabb->GetWorldAABB();
			Vector3 color = aabb->bIsStatic ? Vector3(0, 1, 0) : Vector3(1, 1, 0);

			DebugRenderer::Get().AddAABB(worldAABB.min, worldAABB.max, color);
		}

		DebugRenderer::Get().Render(cam->GetViewMatrix(), cam->GetProjectionMatrix());
		DebugRenderer::Get().Clear();

		m_ForwardPass->End();
	}

	void ForwardRenderer::OnEndFrame()
	{
		for (auto& pass : renderPasses) pass->End();
	}

	void ForwardRenderer::SetTarget(Framebuffer* fb) {
		if (m_ForwardPass) {
			m_ForwardPass->SetTarget(fb);
		}
	}
	ShadowMapPass* ForwardRenderer::GetShadowPass() const
	{
		return m_ShadowPass;
	}
}