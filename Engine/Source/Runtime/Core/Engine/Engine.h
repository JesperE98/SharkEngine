#ifndef ENGINE_H
#define ENGINE_H

#include "Core/Utilities/Debug.h"

struct GLFWwindow;
namespace Shark::Core{

	class Application;

	static constexpr unsigned int WINDOW_WIDTH = 1280;
	static constexpr unsigned int WINDOW_HEIGHT = 840;
	const std::string WINDOW_TITLE = "SharkEngine Window";

	/**
	 * @brief Manages window and OpenGL context initialization, runs the primary application loop, and handles shutdown/cleanup.
	 */
	class Engine
	{
	public:
		Engine();
		~Engine();

		/**
		 * @brief Initializes window + GL context. Call before Run.
		 */
		void Initialize();

		/**
		 * @brief Executes the function's primary run operation.
		 */
		void Run(Application* app);

		/**
		 * @brief Initiates a shutdown procedure, performing cleanup and stopping services or activities as appropriate.
		 */
		void Shutdown();

		GLFWwindow* GetWindow() const { return m_Window; }

	private:
		GLFWwindow* m_Window	= nullptr;
		bool m_Initialized		= false;
	};
}


#endif