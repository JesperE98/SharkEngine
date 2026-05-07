#ifndef RECORDS_MANAGER_H
#define RECORDS_MANAGER_H

#include <string>
#include <unordered_map>

namespace Shark::Core {

	class RecordsManager {
	public:
		static RecordsManager& Get() {
			static RecordsManager instance;
			return instance;
		}

		void Initialize();
		// Returns true if this is a new best time
		bool SubmitTime(const std::string& levelName, float timeSeconds);

		bool HasRecord(const std::string& levelName) const;
		float GetBestTime(const std::string& levelName) const;

		void LoadFromFile();
		void SaveToFile();


	private:
		std::unordered_map<std::string, float> m_Records;
		std::string m_FilePath = "Content/records.json";

		RecordsManager() = default;

		~RecordsManager() = default;

		RecordsManager(const RecordsManager&) = delete;
		RecordsManager& operator=(const RecordsManager&) = delete;
	};
}

#endif // RECORDS_MANAGER_H