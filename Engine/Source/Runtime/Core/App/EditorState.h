#ifndef EDITOR_STATES_H
#define EDITOR_STATES_H

#include <cstdint>

enum class EditorState : uint8_t {
	Edit,
	Play,
	Pause
};

#endif // EDITOR_STATES_H