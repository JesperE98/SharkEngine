#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"

namespace Shark::Graphics {
	class Material {
	public:

		Material(Shader* shaderProgram = nullptr, Texture* m_Texture = nullptr);
		~Material();

		void Bind() const;

		Shader* GetShader() const { return m_Shader; }

		Texture* m_Texture{ nullptr };
	private:

		Shader* m_Shader{ nullptr };

		void CreateDefaultShader();
		void CreateDefaultTexture();
	};
}

#endif