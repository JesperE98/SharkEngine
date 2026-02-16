#ifndef SHARKBEHAVIOUR_H
#define SHARKBEHAVIOUR_H

#include "Behaviour.h"

namespace Shark::Components {
	class SharkBehaviour : virtual public Behaviour
	{
	public:
		virtual void BeginPlay() {};
		virtual void Update(float deltaTime) override {};

	};
}

#endif // SHARKBEHAVIOUR_H