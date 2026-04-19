#include "SceneManager.h"
#include "Scene/Scene.h"
#include "PhysicsSystem.h"

namespace Shark::Managers {

	using Shark::Scene;
	using Shark::Physics::PhysicsSystem;

	SceneManager& SceneManager::Get()
	{
		// TODO: insert return statement here
		static SceneManager instance;
		return instance;
	}

	void SceneManager::SetActiveScene(Scene* scene)
	{
		if (m_ActiveScene) {
			delete m_ActiveScene;
		}
		m_ActiveScene = scene;
	}

	void SceneManager::Update(float deltaTime)
	{
		if (m_ActiveScene) {
			m_ActiveScene->Update(deltaTime);
			PhysicsSystem::Get().Update(deltaTime, m_ActiveScene);
		}
	}

	Scene* SceneManager::GetActiveScene() const
	{
		return m_ActiveScene;
	}

	SceneManager::~SceneManager()
	{
		if (m_ActiveScene) {
			delete m_ActiveScene;
			m_ActiveScene = nullptr;
		}
	}

}
