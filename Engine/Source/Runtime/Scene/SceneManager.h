#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

namespace Shark {
	class Scene;
}

namespace Shark::Managers {

	class SceneManager
	{
	public:

		static SceneManager& Get();

		void SetActiveScene(Shark::Scene* scene);
		void Update(float deltaTime);

		Shark::Scene* GetActiveScene() const;

	private:
		SceneManager() = default;
		~SceneManager();

		// Disable copying
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		Shark::Scene* m_ActiveScene{ nullptr };
	};
}


#endif // SCENE_MANAGER_H