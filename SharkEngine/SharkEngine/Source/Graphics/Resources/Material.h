#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"

class Material {
public:


	Material(Shader* shaderProgram = nullptr, Texture* m_Texture = nullptr);
	~Material();

	void Bind() const;

	Shader* GetShader() const { return m_Shader; }

private:

	Shader* m_Shader{ nullptr };
	Texture* m_Texture{ nullptr };

	void CreateDefaultShader();
	void CreateDefaultTexture();
};

#endif