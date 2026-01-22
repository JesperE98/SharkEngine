#include "Object.h"

namespace Shark {
	Object::Object(const std::string& name)
		: name(name)
	{
	}

	Object::~Object() {
		// Note: Mesh and Material are not deleted here to avoid double deletion
		// if they are shared among multiple objects. Memory management should be
		// handled externally.

		for (auto* sub : SubObjects) {
			delete sub;
			sub = nullptr;
		}
	}
}

