#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Component.h"

namespace Shark::Components {
	class Behaviour : virtual public Component
	{
	public:
		virtual ~Behaviour() = default;

		void SetEnabled(bool value) {
			if (bEnabled != value) {
				bEnabled = value;
				if(bEnabled) {
					OnEnable();
				}
				else {
					OnDisable();
				}
			}
		}

		virtual void OnEnable() {}
		virtual void OnDisable() {}
	};
}

#endif // BEHAVIOUR_H