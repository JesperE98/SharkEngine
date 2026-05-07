#include "RecordsManager.h"
#include "Core/Utilities/Debug.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <cfloat>

namespace Shark::Core {

	using json = nlohmann::json;

	void RecordsManager::Initialize() {
		LoadFromFile();
	}

	bool RecordsManager::SubmitTime(const std::string& levelName, float timeSeconds) {
		if (levelName.empty()) {
			SE_ERR(Engine, "Level Name was empty");
			return false;
		}

		auto it = m_Records.find(levelName);
		bool isNewBest = ( it == m_Records.end() ) || ( timeSeconds < it->second );

		if (isNewBest) {
			m_Records[levelName] = timeSeconds;
			SaveToFile();
			SE_LOG(Engine, "New best time on '{}': {:.2f}s", levelName, timeSeconds);
		}

		return isNewBest;
	}

	bool RecordsManager::HasRecord(const std::string& levelName) const {

		if (levelName.empty()) {
			SE_ERR(Engine, "Level Name was empty");
			return false;
		}

		return m_Records.find(levelName) != m_Records.end();
	}

	float RecordsManager::GetBestTime(const std::string& levelName) const {
		if (levelName.empty()) {
			SE_ERR(Engine, "Level Name was empty");
			return false;
		}

		auto it = m_Records.find(levelName);

		return ( it != m_Records.end() ) ? it->second : FLT_MAX;
	}

	void RecordsManager::LoadFromFile() {
		bool filePathExists = std::filesystem::exists(m_FilePath);

		if (!filePathExists) {
			SE_ERR(Engine, "File path doesn't exists.");
			return;
		}

		std::ifstream file(m_FilePath);
		if (!file.is_open()) {
			SE_ERR(Engine, "File isn't open. File Path:{}", m_FilePath);
			return;
		}

		try {
			json j;
			file >> j;

			for (auto& [key, value] : j.items()) {
				m_Records[key] = value.get<float>();
			}
		} catch (const json::parse_error& e) {
			SE_ERR(Engine, "Failed to parse records file: {}", e.what());
		}
	}

	void RecordsManager::SaveToFile() {
		std::filesystem::create_directories(std::filesystem::path(m_FilePath).parent_path());

		json j;
		for (const auto& [name, time] : m_Records) {
			j[name] = time;
		}

		std::ofstream file(m_FilePath);

		if (file.is_open()) {
			file << j.dump(4);
		}
	}
}