#ifndef SHARK_EVENTS_H
#define SHARK_EVENTS_H

#include "Core/Utilities/Delegate.h"
#include "Core/Utilities/Debug.h"

namespace Shark::Core {

	SE_DECLARE_MULTICAST_DELEGATE_OneParam(FOnLogAdded, const LogEntry&);

	class SharkEvents {
	public:

		static FOnLogAdded& OnLogAdded() {
			static FOnLogAdded instance;
			return instance;
		}

	};
}

#endif // SHARK_EVENTS_H