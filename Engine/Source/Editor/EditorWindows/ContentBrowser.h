#ifndef CONTENT_BROWSER_H
#define CONTENT_BROWSER_H

#include "EditorWindow.h"
#include <vector>
#include <filesystem>
#include <unordered_map>

namespace Shark::Editor {
	class ContentBrowser : public EditorWindow {
	public:

		explicit ContentBrowser();
		~ContentBrowser() override = default;

		void OnInitialize() override final;
		void OnUpdateWindow(float deltaTime) override final;
		void OnShutdown() override final;

	private:
		enum class AssetType : uint8_t {
			Unknown,
			Texture,
			Model,
			Shader,
		};

		struct AssetEntry {
			std::filesystem::path path;
			std::string displayName;
			AssetType type = AssetType::Unknown;
			unsigned int thumbnailTexId = 0;
		};

		void RefreshFiles();
		void DrawToolbar();
		void DrawGrid();
		void DrawAssetCell(const AssetEntry& entry);

		AssetType ClassifyFile(const std::filesystem::path& path) const;
		const char* GetDragPayloadType(AssetType type) const;

		std::filesystem::path							m_RootPath;
		std::vector<AssetEntry>							m_Assets;
		std::unordered_map<std::string, unsigned int>	m_ThumbnailCache;

		float m_CellSize	= 96.0f;
		float m_CellPadding = 12.0f;
	};
}


#endif // CONTENT_BROWSER_H