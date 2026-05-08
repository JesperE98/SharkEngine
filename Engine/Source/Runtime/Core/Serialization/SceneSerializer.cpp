#include "SceneSerializer.h"

#include <Scene/Scene.h>
#include <Core/GameObject.h>
#include <Core/Utilities/Debug.h>
#include <IO/PathManager.h>

#include <Components/Component.h>
#include <Components/ComponentRegistry.h>
#include <Components/Logic/CameraComponent.h>
#include <Components/Logic/CameraController.h>
#include <Components/Rendering/MeshRendererComponent.h>
#include <Components/Rendering/LightComponent.h>
#include <Components/PlayerController.h>
#include <Components/Physics/AABBComponent.h>
#include <Components/Physics/RigidbodyComponent.h>
#include <Components/GoalTrigger.h>
#include <Components/UI/MainMenuComponent.h>
#include <Components/UI/LevelTimer.h>
#include <Components/Rendering/TerrainComponent.h>
#include <Components/AIController.h>

#include <Graphics/Resources/Material.h>
#include <Graphics/Resources/MeshManager.h>
#include <Graphics/Resources/PrimitiveMesh.h>

#include <Math/MathUtils.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace Shark::Serialization {

	using json = nlohmann::json;
	using namespace Shark::Core;
	using namespace Shark::Components;
	using namespace Shark::Graphics;
	using namespace Shark::Math;
	using namespace Shark::Resources;


	SceneSerializer::SceneSerializer(Scene* scene) : m_Scene(scene){
	}

	bool SceneSerializer::SaveToFile(const std::string& path) {
		if (!m_Scene) {
			SE_ERR(Engine, "ScenesSerializer has no scene to save.");
			return false;
		}

		std::string fullPath;

		if (path.find(":") != std::string::npos || path.starts_with("/")) {
			fullPath = path;
		} else {
			fullPath = IO::PathManager::Get().GetPath(IO::PathCategory::Content, path);
		}

		// Ensure the directory exists
		std::filesystem::path filepath(fullPath);
		std::filesystem::create_directories(filepath.parent_path());

		// Build JSON
		json sceneJson = SerializeScene();

		// Write to disk
		std::ofstream file(fullPath);
		if (!file.is_open()) {
			SE_ERR(Engine, "Failed to open file for writing: {}", fullPath);
			return false;
		}

		file << sceneJson.dump(4); // 4-space indent
		file.close();

		SE_SUCC(Engine, "Scene saved: {}", fullPath);
		return true;
	}

	bool SceneSerializer::LoadFromFile(const std::string& path) {
		if (!m_Scene) {
			SE_ERR(Engine, "SceneSerializer has no scene target for loading.");
			return false;
		}

		std::string fullPath;

		if (path.find(":") != std::string::npos || path.starts_with("/")) {
			fullPath = path;
		} else {
			fullPath = IO::PathManager::Get().GetPath(IO::PathCategory::Content, path);
		}

		if (!std::filesystem::exists(fullPath)) {
			SE_ERR(Engine, "Scene file does not exists. {}", path);
			return false;
		}

		std::ifstream file(fullPath);
		if (!file.is_open()) {
			SE_ERR(Engine, "Failed to open scene file: {}", fullPath);
			return false;
		}

		json sceneJson;
		try {
			file >> sceneJson;
		}
		catch(const json::parse_error& e) {
			SE_ERR(Engine, "JSON parse error in {}: {}", fullPath, e.what());
			return false;
		}

		DeserializeScene(sceneJson);

		SE_SUCC(Engine, "Scene loaded: {}", fullPath);
		return true;
	}

	json SceneSerializer::SerializeScene() {
		json j;
		j["version"] = 1;
		j["name"] = m_Scene->GetName();

		json gameObject = json::array();
		for (const GameObject* obj : m_Scene->GetGameObjects()) {
			if (!obj) continue;
			gameObject.push_back(SerializeGameObject(obj));
		}
		j["gameObjects"] = gameObject;

		return j;
	}

	json SceneSerializer::SerializeGameObject(const GameObject* obj) {
		json j;
		j["name"] = obj->GetName();
		j["transform"] = SerializeTransform(obj->GetTransform());

		json comps = json::array();
		for (const Component* comp : obj->GetComponents()) {
			if (!comp) continue;
			json compJson = SerializeComponent(comp);
			if (!compJson.is_null()) {
				comps.push_back(compJson);
			}
		}
		j["components"] = comps;

		return j;
	}

	json SceneSerializer::SerializeComponent(const Component* comp) {
		// Try each concreate type. Returns first match
		if (auto* c = dynamic_cast<const CameraComponent*>( comp ))			return SerializeCamera(c);
		if (auto* c = dynamic_cast<const CameraController*>( comp ))		return SerializeCameraController(c);
		if (auto* c = dynamic_cast<const MeshRendererComponent*>( comp ))	return SerializeMeshRenderer(c);
		if (auto* c = dynamic_cast<const AABBComponent*>( comp ))			return SerializeAABB(c);
		if (auto* c = dynamic_cast<const RigidbodyComponent*>( comp ))		return SerializeRigidBody(c);
		if (auto* c = dynamic_cast<const LightComponent*>( comp ))			return SerializeLight(c);
		if (auto* c = dynamic_cast<const PlayerController*>( comp ))		return SerializePlayerController(c);
		if (auto* c = dynamic_cast<const GoalTrigger*>( comp ))				return SerializeGoalTrigger(c);
		if (auto* c = dynamic_cast<const MainMenuComponent*>( comp ))		return SerializeMainMenu(c);
		if (auto* c = dynamic_cast<const LevelTimer*>( comp ))				return SerializeLevelTimer(c);
		if (auto* c = dynamic_cast<const TerrainComponent*>( comp ))		return SerializeTerrainComponent(c);
		if (auto* c = dynamic_cast<const AIController*>( comp ))			return SerializeAIController(c);

		SE_WARN(Engine, "SceneSerializer: Unknow component type, skipping...");
		return json(nullptr);
	}

	json SceneSerializer::SerializeCamera(const CameraComponent* c) {
		return {
			{"type",		"CameraComponent"},
			{"enabled",		c->bEnabled},
			{"fov",			c->FOV},
			{"nearClip",	c->NearClip},
			{"farClip",		c->FarClip},
			{"aspectRatio", c->AspectRatio},
		};
	}

	json SceneSerializer::SerializeCameraController(const Components::CameraController* c) {
		return {
			{"type",				"CameraController"},
			{"enabled",				c->bEnabled},
			{"moveSpeed",			c->GetMoveSpeed()},
			{"mouseSensitivity",	c->GetMouseSensitivity()},
		};
	}

	json SceneSerializer::SerializeMeshRenderer(const MeshRendererComponent* c) {
		json j;
		j["type"] = "MeshRendererComponent";
		j["enabled"] =	c->bEnabled;

		std::string path = c->GetMeshPath();

		if (!path.empty() && std::all_of(path.begin(), path.end(), ::isdigit)) {
			j["meshPath"] = "Primitive:" + path;
		} else {
			j["meshPath"] = path;
		}

		if (c->GetMaterial()) {
			j["material"] = SerializeMaterial(c->GetMaterial());
		}

		return j;
	}

	json SceneSerializer::SerializeAABB(const AABBComponent* c) {
		return {
			{"type",		"AABBComponent"},
			{"enabled",		c->bEnabled},
			{"offset",		Vec3ToJson(c->m_Offset)},
			{"extents",		Vec3ToJson(c->m_Extents)},
			{"isStatic",	c->bIsStatic},
		};
	}

	json SceneSerializer::SerializeRigidBody(const RigidbodyComponent* c) {
		return {
			{"type",			"RigidbodyComponent"},
			{"enabled",			c->bEnabled},
			{"mass",			c->mass},
			{"gravity",			c->gravity},
			{"drag",			c->drag},
			{"bounceFactor",	c->bounceFactor},
			{"useGravity",		c->bUseGravity},
		};
	}

	json SceneSerializer::SerializeLight(const LightComponent* c) {
		return {
			{"type",		"LightComponent"},
			{"enabled",		c->bEnabled},
			{"lightType",	static_cast<int>(c->Type)},
			{"color",		Vec3ToJson(c->Color)},
			{"intensity",	c->Intensity},
			{"range",		c->Range},
		};
	}

	json SceneSerializer::SerializePlayerController(const PlayerController* c) {
		return {
			{"type",			"PlayerController"},
			{"enabled",			c->bEnabled},
			{"moveSpeed",		c->moveSpeed},
			{"jumpForce",		c->jumpForce},
			{"dashForce",		c->dashForce},
			{"dashCooldown",	c->dashCooldown},
		};
	}

	nlohmann::json SceneSerializer::SerializeGoalTrigger(const Components::GoalTrigger* c) {
		return {
			{"type", "GoalTrigger"},
			{"enabled", c->bEnabled},
			{"nextLevel", c->nextLevel},
		};
	}

	nlohmann::json SceneSerializer::SerializeMainMenu(const Components::MainMenuComponent* c) {
		return {
			{"type", "MainMenuComponent"},
			{"enabled", c->bEnabled},
		};
	}

	nlohmann::json SceneSerializer::SerializeLevelTimer(const Components::LevelTimer* c) {
		return { 
			{"type", "LevelTimer"}, 
			{"enabled", c->bEnabled} 
		};
	}

	nlohmann::json SceneSerializer::SerializeTerrainComponent(const Components::TerrainComponent* c) {
		return {
			{"type",			"TerrainComponent"},
			{"enabled",			c->bEnabled},
			{"heightmapPath",	c->HeightmapPath},
			{"heightScale",		c->HeightScale},
			{"xzScale",			c->xzScale},
		};
	}

	nlohmann::json SceneSerializer::SerializeAIController(const Components::AIController* c) {
		return {
			{"type",			"AIController"},
			{"enabled",			c->bEnabled},
			{"patrolSpeed",		c->PatrolSpeed},
			{"patrolDistance",	c->PatrolDistance},
		};
	}

	json SceneSerializer::SerializeMaterial(const Material* mat) {
		return {
			{"diffusePath",		mat->GetTexturePath()},
			{"specularPath",	mat->GetSpecularTexturePath()},
			{"shininess",		mat->GetShininess()},
			{"tiling",			Vec2ToJson(mat->GetTiling())},
			{"useMipMaps",		mat->GetUseMipMaps()},
		};
	}

	json SceneSerializer::SerializeTransform(const Transform& t) {
		return {
			{"position",	Vec3ToJson(t.position)},
			{"rotation",	QuatToEulerJson(t.rotation)},
			{"scale",		Vec3ToJson(t.scale)},
		};
	}

	json SceneSerializer::Vec2ToJson(const Vector2& v) {
		return json::array({ v.x, v.y });
	}

	json SceneSerializer::Vec3ToJson(const Vector3& v) {
		return json::array({ v.x, v.y, v.z });
	}

	json SceneSerializer::QuatToEulerJson(const Quaternion& q) {
		Vector3 euler = ToEulerDegrees(q);
		return json::array({ euler.x, euler.y, euler.z });
	}

	void SceneSerializer::DeserializeScene(const json& j) {
		// Clear existing scene contents (optional)

		if (j.contains("name")) {
			m_Scene->SetName(j.at("name").get<std::string>());
		}

		if (!j.contains("gameObjects") || !j.at("gameObjects").is_array()) {
			SE_WARN(Engine, "Scene file has no gameObjects array");
			return;
		}

		for (const auto& objJson : j.at("gameObjects")) {
			GameObject* obj = DeserializeGameObject(objJson);

			if (obj) {
				m_Scene->AddGameObject(obj);
			}
		}
	}

	GameObject* SceneSerializer::DeserializeGameObject(const json& j) {
		std::string name = j.value("name", "Unnamed");
		GameObject* obj = new GameObject(name);

		if (j.contains("transform")) {
			DeserializeTransform(j.at("transform"), obj->GetTransform());
		}

		if (j.contains("components") && j.at("components").is_array()) {
			for (const auto& compJson : j.at("components")) {
				DeserializeComponent(compJson, obj);
			}
		}

		return obj;
	}

	Component* SceneSerializer::DeserializeComponent(const json& j, GameObject* owner) {
		if (!j.contains("type")) {
			SE_WARN(Engine, "Component JSON missing 'type' field, skipping.");
			return nullptr;
		}

		std::string type = j.at("type").get<std::string>();
		Component* result = ComponentRegistry::Get().Create(type, owner);

		if (!result) {
			SE_WARN(Engine, "Unknown component type: {}", type);
			return nullptr;
		}

		if (auto* c = dynamic_cast<CameraComponent*>( result ))				DeserializeCamera(j, c);
		else if (auto* c = dynamic_cast<CameraController*>( result ))		DeserializeCameraController(j, c);
		else if (auto* c = dynamic_cast<MeshRendererComponent*>( result ))	DeserializeMeshRenderer(j, c);
		else if (auto* c = dynamic_cast<AABBComponent*>( result ))			DeserializeAABB(j, c);
		else if (auto* c = dynamic_cast<RigidbodyComponent*>( result ))		DeserializeRigidbody(j, c);
		else if (auto* c = dynamic_cast<LightComponent*>( result ))			DeserializeLight(j, c);
		else if (auto* c = dynamic_cast<PlayerController*>( result ))		DeserializePlayerController(j, c);
		else if (auto* c = dynamic_cast<GoalTrigger*>( result ))			DeserializeGoalTrigger(j, c);
		else if (auto* c = dynamic_cast<MainMenuComponent*>( result ))		DeserializeMainMenu(j, c);
		else if (auto* c = dynamic_cast<LevelTimer*>( result ))				DeserializeLevelTimer(j, c);
		else if (auto* c = dynamic_cast<TerrainComponent*>( result ))		DeserializeTerrainComponent(j, c);
		else if (auto* c = dynamic_cast<AIController*>( result ))			DeserializeAIController(j, c);


		result->bEnabled = j.value("enabled", true);

		return result;
	}

	void SceneSerializer::DeserializeCamera(const json& j, CameraComponent* c) {
		c->FOV			= j.value("fov", 45.0f);
		c->NearClip		= j.value("nearClip", 0.1f);
		c->FarClip		= j.value("farClip", 1000.0f);
		c->AspectRatio	= j.value("aspectRatio", 1.77f);
		c->UpdateProjectionMatrix();
	}

	void SceneSerializer::DeserializeCameraController(const json& j, CameraController* c) {
		c->SetMoveSpeed(j.value("moveSpeed", 5.0f));
		c->SetMouseSensitivity(j.value("mouseSensitivity", 0.1f));
	}

	void SceneSerializer::DeserializeMeshRenderer(const json& j, MeshRendererComponent* c) {
		std::string meshPath = j.value("meshPath", "");
		c->SetMeshPath(meshPath);

		// Synchronous loading
		if (!meshPath.empty()) {
			if (meshPath.starts_with("Primitive:")) {
				// handle primitives separately
				std::string typeStr = meshPath.substr(10);
				int primTypeInt = std::stoi(typeStr);
				PrimitiveType type = static_cast<PrimitiveType>(primTypeInt);

				Mesh* mesh = MeshManager::Get().LoadMesh(type);
				c->SetMesh(mesh);
			} else {
				Mesh* mesh = MeshManager::Get().LoadMeshSync(meshPath);
				c->SetMesh(mesh);
			}
		}

		// Material
		if (j.contains("material")) {
			Material* mat = new Material();
			DeserializeMaterial(j.at("material"), mat);
			c->SetMaterial(mat);
		} else {
			c->SetMaterial(new Material());
		}
	}

	void SceneSerializer::DeserializeAABB(const json& j, AABBComponent* c) {
		if (j.contains("offset"))	c->m_Offset		= JsonToVec3(j.at("offset"));
		if (j.contains("extents"))	c->m_Extents	= JsonToVec3(j.at("extents"));
		c->bIsStatic = j.value("isStatic", false);
	}
	void SceneSerializer::DeserializeRigidbody(const json& j, RigidbodyComponent* c) {
		c->mass			= j.value("mass", 1.0f);
		c->gravity		= j.value("gravity", -20.0f);
		c->drag			= j.value("drag", 0.85f);
		c->bounceFactor = j.value("bounceFactor", 0.0f);
		c->bUseGravity	= j.value("useGravity", true);
	}

	void SceneSerializer::DeserializeLight(const json& j, LightComponent* c) {
		c->Type			= static_cast<LightType>( j.value("lightType", 0) );
		if (j.contains("color")) c->Color = JsonToVec3(j.at("color"));
		c->Intensity	= j.value("intensity", 1.0f);
		c->Range		= j.value("range", 10.0f);
	}

	void SceneSerializer::DeserializePlayerController(const json& j, PlayerController* c) {
		c->moveSpeed	= j.value("moveSpeed", 8.0f);
		c->jumpForce	= j.value("jumpForce", 10.0f);
		c->dashForce	= j.value("dashForce", 20.0f);
		c->dashCooldown = j.value("dashCooldown", 1.0f);
	}

	void SceneSerializer::DeserializeGoalTrigger(const nlohmann::json& j, Components::GoalTrigger* c) {
		c->nextLevel = j.value("nextLevel", "");
	}

	void SceneSerializer::DeserializeMainMenu(const nlohmann::json& j, Components::MainMenuComponent* c) {
		// Empty for now. No tunable fields
	}

	void SceneSerializer::DeserializeLevelTimer(const nlohmann::json& j, Components::LevelTimer* c) {
		// No fields to populate for now...
	}

	void SceneSerializer::DeserializeTerrainComponent(const nlohmann::json& j, Components::TerrainComponent* c) {
		std::string path	= j.value("heightmapPath", "");
		float hScale		= j.value("heightScale", 25.0f);
		float xzSc			= j.value("xzScale", 1.0f);

		if (!path.empty()) {
			c->LoadTerrain(path, hScale, xzSc);
		}
	}

	void SceneSerializer::DeserializeAIController(const nlohmann::json& j, Components::AIController* c) {
		c->PatrolSpeed		= j.value("patrolSpeed", 3.0f);
		c->PatrolDistance	= j.value("patrolDistance", 10.0f);
	}

	void SceneSerializer::DeserializeMaterial(const json& j, Material* mat) {
		if (j.contains("diffusePath")) {
			std::string path = j.at("diffusePath").get<std::string>();
			if (!path.empty()) mat->SetTexture(path);
		}
		if (j.contains("specularPath")) {
			std::string path = j.at("specularPath").get<std::string>();
			if (!path.empty()) mat->SetSpecularTexture(path);
		}
		if (j.contains("shininess")) {
			mat->SetShininess(j.at("shininess").get<float>());
		}
		if (j.contains("tiling")) {
			mat->SetTiling(JsonToVec2(j.at("tiling")));
		}
		if (j.contains("useMipMaps")) {
			mat->SetUseMipMaps(j.at("useMipMaps").get<bool>());
		}
	}

	void SceneSerializer::DeserializeTransform(const json& j, Transform& t) {
		t.position	= JsonToVec3(j.at("position"));
		t.rotation	= JsonToQuatFromEuler(j.at("rotation"));
		t.scale		= JsonToVec3(j.at("scale"));
	}

	Vector2 SceneSerializer::JsonToVec2(const json& j) {
		return Vector2(
			j.at(0).get<float>(),
			j.at(1).get<float>()
		);
	}

	Vector3 SceneSerializer::JsonToVec3(const json& j) {
		return Vector3(
			j.at(0).get<float>(),
			j.at(1).get<float>(),
			j.at(2).get<float>()
		);
	}

	Quaternion SceneSerializer::JsonToQuatFromEuler(const json& j) {
		Vector3 euler = JsonToVec3(j);
		return FromEulerDegrees(euler);
	}
}