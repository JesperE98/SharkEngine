#ifndef EDITOR_STATE_MANAGER_H
#define EDITOR_STATE_MANAGER_H

#include "EditorState.h"

namespace Shark::Core {
	class EditorStateManager {
	public:
		static EditorStateManager& Get();
		EditorState GetState() const { return m_State; }
		void OnSetState(EditorState e);
		bool IsPlaying() const { return m_State == EditorState::Play; }
		bool IsPaused() const { return m_State == EditorState::Pause; }

		// Disable copying
		EditorStateManager(const EditorStateManager&)				= delete;
		EditorStateManager& operator=(const EditorStateManager&)	= delete;
	private:
		EditorState m_State = EditorState::Edit;

		EditorStateManager() = default;
		~EditorStateManager();
	};
}

#endif // EDITOR_STATE_MANAGER_H