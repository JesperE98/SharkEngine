#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#pragma region Includes

#include "Core/Messaging/MessageSystem.h"

#include <string>
#include <unordered_map>
#include <functional>

#pragma endregion

#pragma region Namespaces

namespace Shark {
	class Scene;
}

#pragma endregion

namespace Shark::Core {

	using SceneBuilder = std::function<void(Shark::Scene*)>;

	class SceneManager
	{
	public:
		MessageSystem inbox;

		static SceneManager& Get();

		/**
		 * @brief Registers a scene builder under the specified name for later use.
		 * @param name The name or identifier for the scene, used to look up the registered builder.
		 * @param builder A SceneBuilder callable or object that constructs or initializes the scene; passed by value.
		 */
		void RegisterScene(const std::string& name, SceneBuilder builder);

		/**
		 * @brief Loads a scene by its name.
		 * @param name The name or identifier of the scene to load.
		 * @return A pointer to the loaded Shark::Scene, or nullptr if the scene could not be found or failed to load.
		 */
		Shark::Scene* LoadScene(const std::string& name);

		/**
		 * @brief Loads a scene from a JSON file on disk.
		 * @param path File path to the scene JSON.
		 * @return Pointer to the loaded scene, or nullptr on failure.
		 */
		Shark::Scene* LoadSceneFromFile(const std::string& path);

		void UnloadActiveScene();
		void SetActiveScene(Shark::Scene* scene);

		Shark::Scene* GetActiveScene() const;
		void Update(float deltaTime);


		// Disable copying
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
	private:
		Shark::Scene*	m_ActiveScene = nullptr;
		std::string		m_ActiveSceneName;
		std::unordered_map<std::string, SceneBuilder> m_SceneBuilders;

		SceneManager() = default;
		~SceneManager();

	};
}


#endif // SCENE_MANAGER_H