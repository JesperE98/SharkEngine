#ifndef EDITOR_WINDOWMANAGER_H
#define EDITOR_WINDOWMANAGER_H

#include <Core/Messaging/MessageSystem.h>
#include <vector>

#include "EditorWindows/EditorWindow.h"

namespace Shark::Editor {

	class WindowManager
	{
	public:

		Shark::Core::MessageSystem inbox;

		static WindowManager& Get() {
			static WindowManager instance;
			return instance;
		}


		void Initialize();
		void Update(float deltaTime);
		void RenderWindows(float deltaTime);

		/**
		@brief Adds a new window to the manager. The manager takes ownership of the window pointer and will handle its lifecycle.
		@param window A pointer to the EditorWindow to add. Must be allocated on the heap (using new).
		*/
		void AddWindow(EditorWindow* window);

		/**
		@brief Renders the SceneViewport window. This should be called after all other windows have been rendered, as it typically contains the main 3D view of the editor.
		*/
		void RenderSceneViewport();
		void Shutdown();

		const std::vector<EditorWindow*>& GetWindows() const { return m_Windows; }
		bool IsSceneViewportFocused() const;
		bool IsSceneViewportHovered() const;

		/**
		Utility function to get a specific window type. Returns nullptr if not found or if the cast fails.
		@param T The type of window to retrieve (must derive from EditorWindow)
		*/
		template<typename T>
		T* GetWindow();

	private:
		std::vector<EditorWindow*> m_Windows;		
	
		class SceneViewport* m_SceneViewport{ nullptr };

		WindowManager() = default;
		~WindowManager() {
			Shutdown();
		}

		// Delete copy constructor and assignment operator
		WindowManager(const WindowManager&) = delete;
		WindowManager& operator=(const WindowManager&) = delete;

		/**
		@brief Renders the Engine Statistics window, which displays real-time information about memory usage and other performance metrics. This window is typically used for debugging and optimization purposes.
		*/
		void RenderStatsWindow();

		/*
		@brief Handles incoming messages from other systems. This function processes messages related to window management, such as opening and closing windows, and updates the state of the windows accordingly.
		@param msg The message to process, containing the event type and any relevant payload data.
		*/
		void RecieveMessages(const Shark::Core::Message& msg);

		template<typename T, typename... Args>
		T* CreateEditorWindow(Args&&... args) {
			// Create a new window of type T with the provided arguments
			T* window = new T(std::forward<Args>(args)...);
			m_Windows.push_back(static_cast<EditorWindow*>(window)); // Store the window in the manager

			return window; // Return the raw pointer to the created window
		}
	};
}

#endif // EDITOR_WINDOWMANAGER_H