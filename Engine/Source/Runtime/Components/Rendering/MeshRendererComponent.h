#ifndef ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H
#define ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H

#include "Components/Component.h"
#include "Graphics/Resources/Material.h"
#include "Graphics/Resources/Mesh.h"

namespace Shark::Components {

	class MeshRendererComponent : virtual public Component
	{
	public:
		explicit MeshRendererComponent(Graphics::Mesh* mesh, Graphics::Material* mat)
			: m_Mesh(mesh), m_Material(mat) { 
			tickMode = TickMode::Always;
		}

		~MeshRendererComponent() override;

		void Update(float deltaTime) override {}

		void Render();

#pragma region ASSET SETTERS
		void SetMesh(Graphics::Mesh* mesh);
		void SetMaterial(Graphics::Material* mat);
#pragma endregion

#pragma region ASSET GETTERS
		Graphics::Mesh* GetMesh() const;
		Graphics::Material* GetMaterial() const;
#pragma endregion

	private:
		Graphics::Mesh* m_Mesh{ nullptr };
		Graphics::Material* m_Material{ nullptr };
	};
}

#endif // ENGINE_COMPONENTS_MESHRENDERERCOMPONENT_H