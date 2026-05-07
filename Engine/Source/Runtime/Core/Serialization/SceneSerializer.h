#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include <nlohmann/json.hpp>
#include <string>

namespace Shark {
	class Scene;
}

namespace Shark::Core {
	class GameObject;
}

namespace Shark::Components {
	class Component;
	class CameraComponent;
	class CameraController;
	class MeshRendererComponent;
	class AABBComponent;
	class RigidbodyComponent;
	class LightComponent;
	class PlayerController;
	class GoalTrigger;
	class MainMenuComponent;
	class LevelTimer;
}

namespace Shark::Graphics {
	class Material;
}

namespace Shark::Math {
	struct Transform;
	struct Vector2;
	struct Vector3;
	struct Quaternion;
}

namespace Shark::Serialization {

	
	class SceneSerializer {
	public:
		explicit SceneSerializer(Shark::Scene* scene);

		/**
		 * @brief Saves data to the specified file path.
		 * @param path Path to the file where data will be saved.
		 * @return true if the file was saved successfully; otherwise false.
		 */
		bool SaveToFile(const std::string& path);

		bool LoadFromFile(const std::string& path);
	private:
#pragma region SCENE SERIALIZATION
		nlohmann::json SerializeScene();
		nlohmann::json SerializeGameObject(const Core::GameObject* obj);

		nlohmann::json SerializeComponent(const Components::Component* comp);

		nlohmann::json SerializeCamera(const Components::CameraComponent* c);
		nlohmann::json SerializeCameraController(const Components::CameraController* c);
		nlohmann::json SerializeMeshRenderer(const Components::MeshRendererComponent* c);
		nlohmann::json SerializeAABB(const Components::AABBComponent* c);
		nlohmann::json SerializeRigidBody(const Components::RigidbodyComponent* c);
		nlohmann::json SerializeLight(const Components::LightComponent* c);
		nlohmann::json SerializePlayerController(const Components::PlayerController* c);
		nlohmann::json SerializeGoalTrigger(const Components::GoalTrigger* c);
		nlohmann::json SerializeMainMenu(const Components::MainMenuComponent* c);
		nlohmann::json SerializeLevelTimer(const Components::LevelTimer* c);

		nlohmann::json SerializeMaterial(const Graphics::Material* mat);
		nlohmann::json SerializeTransform(const Math::Transform& t);

		static nlohmann::json Vec2ToJson(const Math::Vector2& v);
		static nlohmann::json Vec3ToJson(const Math::Vector3& v);
		static nlohmann::json QuatToEulerJson(const Math::Quaternion& q);

#pragma endregion

#pragma region SCENE DESERIALIZATION
		void DeserializeScene(const nlohmann::json& j);
		Core::GameObject* DeserializeGameObject(const nlohmann::json& j);
		
		Components::Component* DeserializeComponent(const nlohmann::json& j, Core::GameObject* owner);

		void DeserializeCamera(const nlohmann::json& j, Components::CameraComponent* c);
		void DeserializeCameraController(const nlohmann::json& j, Components::CameraController* c);
		void DeserializeMeshRenderer(const nlohmann::json& j, Components::MeshRendererComponent* c);
		void DeserializeAABB(const nlohmann::json& j, Components::AABBComponent* c);
		void DeserializeRigidbody(const nlohmann::json& j, Components::RigidbodyComponent* c);
		void DeserializeLight(const nlohmann::json& j, Components::LightComponent* c);
		void DeserializePlayerController(const nlohmann::json& j, Components::PlayerController* c);
		void DeserializeGoalTrigger(const nlohmann::json& j, Components::GoalTrigger* c);
		void DeserializeMainMenu(const nlohmann::json& j, Components::MainMenuComponent* c);
		void DeserializeLevelTimer(const nlohmann::json& j, Components::LevelTimer* c);

		void DeserializeMaterial(const nlohmann::json& j, Graphics::Material* mat);
		void DeserializeTransform(const nlohmann::json& j, Math::Transform& t);

		static Math::Vector2 JsonToVec2(const nlohmann::json& j);
		static Math::Vector3 JsonToVec3(const nlohmann::json& j);
		static Math::Quaternion JsonToQuatFromEuler(const nlohmann::json& j);

#pragma endregion

		Shark::Scene* m_Scene = nullptr;
	};
}
#endif // SCENE_SERIALIZER_H