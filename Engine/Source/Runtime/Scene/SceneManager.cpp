#include "SceneManager.h"
#include "Scene.h"

#include "IO/PathManager.h"

#include "Core/Utilities/Debug.h"
#include <Core/Spatial/OctreeSystem.h>
#include "Core/Serialization/SceneSerializer.h"

namespace Shark::Core {

	using Shark::Scene;
	using Spatial::OctreeSystem;
	using Serialization::SceneSerializer;

	SceneManager& SceneManager::Get()
	{
		static SceneManager instance;
		return instance;
	}

	SceneManager::~SceneManager()
	{
		if (m_ActiveScene) {
			delete m_ActiveScene;
			m_ActiveScene = nullptr;
		}

		m_ActiveSceneName.clear();
	}

	void SceneManager::RegisterScene(const std::string& name, SceneBuilder builder)
	{
		m_SceneBuilders[name] = builder;
		SE_LOG(Engine, "Registered scene: {}", name);
	}

	Shark::Scene* SceneManager::LoadScene(const std::string& name)
	{
		auto it = m_SceneBuilders.find(name);
		if (it == m_SceneBuilders.end()) {
			SE_WARN(Engine, "No scene registered with name {}", name);
			return nullptr;
		}

		// Tear down old scene
		UnloadActiveScene();

		// Create fresh scene
		Scene* newScene = new Scene();
		it->second(newScene);

		m_ActiveScene = newScene;
		m_ActiveSceneName = name;

		OctreeSystem::Get().RebuildFromScene(newScene);

		// Broadcast using a helper function
		SendTo(*this, EventType::SceneLoaded, name, newScene);

		SE_LOG(Engine, "Loaded scene: {}", name);
		return newScene;
	}

	Scene* SceneManager::LoadSceneFromFile(const std::string& path) {
		UnloadActiveScene();

		Scene* newScene = new Scene();
		SceneSerializer serializer(newScene);

		if (!serializer.LoadFromFile(path)) {
			delete newScene;
			SE_ERR(Engine, "Failed to load scene from file: {}", path);
			return nullptr;
		}

		m_ActiveScene = newScene;
		m_ActiveSceneName = path;

		OctreeSystem::Get().RebuildFromScene(newScene);

		SendTo(*this, EventType::SceneLoaded, path, newScene);

		SE_LOG(Engine, "Loaded scene from file: {}", path);
		return newScene;
	}

	void SceneManager::UnloadActiveScene()
	{
		if (!m_ActiveScene) return;

		SE_PROC(Engine, "Unloading scene: {}", m_ActiveSceneName);

		SendTo(*this, EventType::SceneUnloaded, m_ActiveSceneName, m_ActiveScene);

		delete m_ActiveScene;
		m_ActiveScene = nullptr;
		m_ActiveSceneName.clear();
	}

	void SceneManager::SetActiveScene(Scene* scene)
	{
		UnloadActiveScene();
		m_ActiveScene = scene;
		OctreeSystem::Get().RebuildFromScene(scene);
	}

	void SceneManager::Update(float deltaTime)
	{
		Message msg;
		while (inbox.Pop(msg)) {
			switch (msg.type) {

				case EventType::LoadScene:
					if (msg.payload.ends_with(".json")) {
						LoadSceneFromFile(msg.payload);
					} else {
						LoadScene(msg.payload);
					}
				
					break;

				case EventType::UnloadScene:	
					UnloadActiveScene();	
				
					break;
				default: break;
			}
		}
	}

	Scene* SceneManager::GetActiveScene() const
	{
		return m_ActiveScene;
	}
}
