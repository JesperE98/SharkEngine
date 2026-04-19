#ifndef SHARKBEHAVIOUR_H
#define SHARKBEHAVIOUR_H

#include "Behavior.h"

namespace Shark::Components {
	class SharkBehavior : virtual public Behavior
	{
	public:
		virtual void BeginPlay() {};
		virtual void Update(float deltaTime) override {};

	};
}

#endif // SHARKBEHAVIOUR_H