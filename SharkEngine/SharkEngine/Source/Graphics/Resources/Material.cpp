#include "Material.h"
#include "Core/Utilities/Time.h"

#include <iostream>
#include <glad/glad.h>

Material::Material(Shader* shaderProgram, Texture* texture) 
	: m_Shader(shaderProgram), m_Texture(texture)
{
	std::cout << Time::CreateTimeStamp() << ": " << "Creating Material. " << std::endl;
	if(shaderProgram) {
	}
	else {
		CreateDefaultShader();
	}

	if (texture) {
		m_Texture = texture;
	}
	else {
		CreateDefaultTexture();
	}

}

Material::~Material()
{
	if(m_Shader) {
		delete m_Shader;
		m_Shader = nullptr;
	}
}

void Material::Bind() const {

	if (m_Texture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture->GetID());
		m_Shader->SetInt("u_Texture", 0);
	}
}

void Material::CreateDefaultShader() {
	
	const char* vertexPath = "Assets/Shaders/SE_BasicLit.vert.glsl";
	const char* fragmentPath = "Assets/Shaders/SE_BasicLit.frag.glsl";

	m_Shader = new Shader(vertexPath, fragmentPath);
}

void Material::CreateDefaultTexture()
{
	const char* path = "Assets/Textures/box-texture-pbr-01.jpg";

	m_Texture = new Texture(path);
}

