#include "SceneManager.h"
#include "Scene.h"

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
	}
}

Scene* SceneManager::GetActiveScene() const
{
	return m_ActiveScene;
}

SceneManager::~SceneManager()
{
	if(m_ActiveScene) {
		delete m_ActiveScene;
		m_ActiveScene = nullptr;
	}
}
