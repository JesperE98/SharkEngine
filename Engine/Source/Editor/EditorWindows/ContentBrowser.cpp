#include "ContentBrowser.h"

#include <Core/Utilities/Debug.h>
#include <IO/PathManager.h>
#include <Graphics/Resources/TextureManager.h>

#include <ImGui/imgui.h>
#include <algorithm>

namespace Shark::Editor {

	using Core::Debug;
	using Resources::TextureManager;
	using IO::PathManager;

	ContentBrowser::ContentBrowser() : EditorWindow("Content", true) {
		m_RootPath = PathManager::Get().GetRootPath() + "Content";
	}

	void ContentBrowser::OnInitialize() {
		RefreshFiles();
	}

	void ContentBrowser::OnUpdateWindow(float deltaTime) {
		DrawToolbar();
		ImGui::Separator();
		DrawGrid();
	}

	void ContentBrowser::OnShutdown() {
		m_Assets.clear();
		m_ThumbnailCache.clear();
	}

	void ContentBrowser::DrawToolbar() {
		if (ImGui::Button("Refresh")) {
			RefreshFiles();
		}
		ImGui::SameLine();
		ImGui::Text("Root: %s", m_RootPath.string().c_str());
		ImGui::SameLine();
		ImGui::Text("|	%zu assets", m_Assets.size());
	}

	void ContentBrowser::DrawGrid() {
		const float panelWidth	= ImGui::GetContentRegionAvail().x;
		const float cellWidth	= m_CellSize + m_CellPadding;
		int columns = std::max(1, static_cast<int>( panelWidth / cellWidth ));

		if (ImGui::BeginTable("##contentGrid", columns,
			ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_SizingFixedFit)) {
			for (const auto& entry : m_Assets) {
				ImGui::TableNextColumn();
				DrawAssetCell(entry);
			}
			ImGui::EndTable();
		}
	}

	void ContentBrowser::DrawAssetCell(const AssetEntry& entry) {
		ImGui::PushID(entry.path.string().c_str());

		// Thumbnail (or placeholder button)
		if (entry.thumbnailTexId != 0) {
			ImGui::Image(
				(ImTextureID)(intptr_t)entry.thumbnailTexId,
				ImVec2(m_CellSize, m_CellSize),
				ImVec2(0, 1), ImVec2(1, 0) // flip V for GL
			);
		} else {
			// Placeholder: a button with an initial letter based on type
			const char* label = "?";
			switch (entry.type) {
				case AssetType::Texture:	label = "T"; break;
				case AssetType::Model:		label = "M"; break;
				case AssetType::Shader:		label = "S"; break;
				default: break;
			}
			ImGui::Button(label, ImVec2(m_CellSize, m_CellSize));
		}

		// Drag source - any cell can be dragged
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			const char* payloadType = GetDragPayloadType(entry.type);

			if (payloadType) {
				// payload data = null-terminated path string
				std::filesystem::path relPath = std::filesystem::relative(entry.path, m_RootPath);
				std::string payload = relPath.generic_string();

				ImGui::SetDragDropPayload(payloadType, payload.c_str(), payload.size() + 1);
				ImGui::Text("%s", entry.displayName.c_str());
			}
			ImGui::EndDragDropSource();
		}

		// Tooltip on hover
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("%s", entry.path.string().c_str());
		}

		// Filename label, truncated to cell width
		ImGui::TextWrapped("%s", entry.displayName.c_str());

		ImGui::PopID();
	}

	void ContentBrowser::RefreshFiles() {
		m_Assets.clear();

		if (!std::filesystem::exists(m_RootPath)) {
			SE_WARN(Editor, "ContentBrowser root path does not exists: {}", m_RootPath.string());
			return;
		}

		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(m_RootPath)) {
			if (!dirEntry.is_regular_file()) continue;

			AssetEntry asset;
			asset.path			= dirEntry.path();
			asset.displayName	= dirEntry.path().filename().string();
			asset.type			= ClassifyFile(dirEntry.path());

			// Load thumbnail for textures only (cheap win)
			if (asset.type == AssetType::Texture) {
				std::filesystem::path relPath = std::filesystem::relative(asset.path, m_RootPath);
				std::string relKey = relPath.generic_string();

				auto it = m_ThumbnailCache.find(relKey);
				if (it != m_ThumbnailCache.end()) {
					asset.thumbnailTexId = it->second;
				} else {
					// Ask the TextureManager for the GL texture id &&
					// adapt the API name.
					unsigned int id = TextureManager::Get().GetTextureID(relKey);

					if (id != 0) {
						m_ThumbnailCache[relKey]	= id;
						asset.thumbnailTexId		= id;
					}
				}
			}

			m_Assets.push_back(std::move(asset));
		}

		// Sort: folders-first would be nice later, but for now just by name
		std::sort(m_Assets.begin(), m_Assets.end(), [](const AssetEntry& a, const AssetEntry& b) {
			return a.displayName < b.displayName;
		});

		SE_LOG(Editor, "ContentBrowser scanned {} assets in {}", m_Assets.size(), m_RootPath.string());
	}

	ContentBrowser::AssetType ContentBrowser::ClassifyFile(const std::filesystem::path& path) const {
		std::string ext = path.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" ||
			ext == ".bmp" || ext == ".tga") {
			return AssetType::Texture;
		}

		if (ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".glb") {
			return AssetType::Model;
		}

		if (ext == ".glsl" || ext == ".vert" || ext == ".frag" || ext == ".geom") {
			return AssetType::Shader;
		}
		return AssetType::Unknown;
	}

	const char* ContentBrowser::GetDragPayloadType(AssetType type) const {
		switch (type) {
			case AssetType::Texture: return "ASSET_TEXTURE";
			case AssetType::Model:	return "ASSET_MODEL";
			case AssetType::Shader: return "ASSET_SHADER";
			default:	return nullptr;
		}
	}
}