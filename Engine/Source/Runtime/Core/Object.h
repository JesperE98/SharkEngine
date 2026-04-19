#ifndef OBJECT_H
#define OBJECT_H

#include "Core/Utilities/Debug.h"

namespace Shark {
	class Object
	{
	public:
		virtual ~Object() = default;

		// Utility functions
		const void SetName(const std::string& value) { name = value; }
		const std::string& GetName() const { return name; }
	protected:
		std::string name = "New Object";
	};
}

#endif