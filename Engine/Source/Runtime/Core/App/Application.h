#ifndef APPLICATION_H
#define APPLICATION_H

namespace Shark::Core {

	/**
	 * @brief Abstract base class that defines the lifecycle interface for an application or system. Derived classes must implement initialization, per-frame update, rendering, and shutdown logic.
	 */
	class Application
	{
	public:

		virtual ~Application() = default;

		/**
		 * @brief Pure virtual method called to perform initialization; must be overridden by derived classes.
		 */
		virtual void OnInitialize() = 0;

		/**
		 * @brief Pure virtual method invoked to update the object's state, typically called once per frame; must be overridden by derived classes.
		 * @param deltaTime Elapsed time since the last update, in seconds.
		 */
		virtual void OnUpdate(float deltaTime) = 0;

		/**
		 * @brief Pure virtual method invoked to perform rendering. Derived classes must implement this to render content.
		 */
		virtual void OnRender() = 0;

		/**
		 * @brief Invoked when the component or system is shutting down. This is a pure virtual method that derived classes must implement to perform cleanup or shutdown tasks.
		 */
		virtual void OnShutdown() = 0;
	};
}

#endif // APPLICATION_H