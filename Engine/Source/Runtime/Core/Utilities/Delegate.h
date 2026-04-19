#ifndef DELEGATE_H
#define DELEGATE_H

#include <vector>
#include <functional>
#include <mutex>

namespace Shark::Core {

	// Multicast class - allows multiple listeners
	template<typename... Args>
	class TMultiCastDelegate {
	public:
		using Callback = std::function<void(Args...)>;

		template<typename T>
		void AddListener(T* InObject, void(T::*InMethod)(Args...)) {
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Listeners.push_back([InObject, InMethod](Args... args) {
				(InObject->*InMethod)(args...);
			});
		}

		void Broadcast(Args... args) {
			std::lock_guard<std::mutex> lock(m_Mutex);
			
			for (const auto& listener : m_Listeners) {
				if (listener) {
					listener(args...);
				}
			}
		}

		void Clear() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Listeners.clear();
		}

	private:

		std::vector<Callback> m_Listeners;
		std::mutex m_Mutex;
	};
}

// --- SHARK ENGINE MACROS ---

#define SE_DECLARE_MULTICAST_DELEGATE(DelegateName)\
	using DelegateName = Shark::Core::TMultiCastDelegate<>;

#define SE_DECLARE_MULTICAST_DELEGATE_OneParam(DelegateName, ParamType)\
	using DelegateName = Shark::Core::TMultiCastDelegate<ParamType>;

#define SE_DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateName, Param1, Param2)\
	using DelegateName = Shark::Core::TMultiCastDelegate<Param1, Param2>;
#endif // DELEGATE_H
