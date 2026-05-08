#include "EditorStateManager.h"
#include <Components/SharkBehavior.h>

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

		Components::SharkBehavior sharkBehavior;
		m_State = e;

		switch (e) {
			case EditorState::Play:
				sharkBehavior.BeginPlay();
				break;
		}
	}


}

