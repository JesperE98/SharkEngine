#ifndef ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H
#define ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H

#include "Components/Component.h"
#include "Graphics/Resources/Material.h"
#include "Graphics/Resources/Mesh.h"

namespace Shark::Components {

	class MeshRendererComponent : virtual public Component
	{
	public:
		explicit MeshRendererComponent(Shark::Graphics::Mesh* mesh, Shark::Graphics::Material* mat)
			: m_Mesh(mesh), m_Material(mat) { }

		~MeshRendererComponent() override;

		void Update(float deltaTime) override {}

		void Render();

		// --- Asset Setters ---
		void SetMesh(Shark::Graphics::Mesh* mesh);
		void SetMaterial(Shark::Graphics::Material* mat);

		// --- Asset Getters ---
		Shark::Graphics::Mesh* GetMesh() const;
		Shark::Graphics::Material* GetMaterial() const;

	private:
		Shark::Graphics::Mesh* m_Mesh{ nullptr };
		Shark::Graphics::Material* m_Material{ nullptr };
	};
}

#endif // ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H