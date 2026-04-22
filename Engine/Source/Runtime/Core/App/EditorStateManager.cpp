#include "EditorStateManager.h"

namespace Shark::Core {
	EditorStateManager& EditorStateManager::Get()
	{
		static EditorStateManager instance;
		return instance;
	}

	EditorStateManager::~EditorStateManager()
	{
		m_State = EditorState::Edit;
	}

	void EditorStateManager::OnSetState(EditorState e)
	{
		m_State = e;
	}


}

