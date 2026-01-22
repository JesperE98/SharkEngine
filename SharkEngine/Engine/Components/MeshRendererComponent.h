#ifndef ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H
#define ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H

#include "Component.h"
#include "Graphics/Resources/Material.h"
#include "Graphics/Resources/Mesh.h"

namespace Shark::Components {

	class MeshRendererComponent : public Shark::Components::Component
	{
	public:
		MeshRendererComponent(Shark::Entities::GameObject* owner, Shark::Graphics::Mesh* mesh, Shark::Graphics::Material* mat)
			: Component(owner, "MeshRenderer"), m_GameObject(owner), m_Mesh(mesh), m_Material(mat) {
		}

		~MeshRendererComponent() override = default;

		void Update(float deltaTime) override {}

		void Render();

		// --- Asset Getters ---
		Shark::Graphics::Mesh* GetMesh() const;
		Shark::Graphics::Material* GetMaterial() const;

		// --- Asset Setters ---
		void SetMesh(Shark::Graphics::Mesh* mesh);
		void SetMaterial(Shark::Graphics::Material* mat);

	private:
		Shark::Graphics::Mesh* m_Mesh{ nullptr };
		Shark::Graphics::Material* m_Material{ nullptr };
		Shark::Entities::GameObject* m_GameObject{ nullptr };
	};
}

#endif // ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H