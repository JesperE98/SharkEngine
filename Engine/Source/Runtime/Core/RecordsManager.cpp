#include "RecordsManager.h"
#include "Core/Utilities/Debug.h"
#include "IO/PathManager.h"

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
		std::string fullPath = IO::PathManager::Get().GetPath(IO::PathCategory::Content, "records.json");

		if (!std::filesystem::exists(fullPath)) {
			SE_WARN(IO, "Records file not found at: {}. This is normal on first run.", fullPath);
			return;
		}

		m_FilePath = fullPath;

		std::ifstream file(fullPath);
		if (!file.is_open()) {
			SE_ERR(Engine, "File isn't open. File Path:{}", fullPath);
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
		std::string fullPath = IO::PathManager::Get().GetPath(IO::PathCategory::Content, "records.json");

		m_FilePath = fullPath;
		std::filesystem::create_directories(std::filesystem::path(fullPath).parent_path());

		json j;
		for (const auto& [name, time] : m_Records) {
			j[name] = time;
		}

		std::ofstream file(fullPath);

		if (file.is_open()) {
			file << j.dump(4);
		}
	}
}