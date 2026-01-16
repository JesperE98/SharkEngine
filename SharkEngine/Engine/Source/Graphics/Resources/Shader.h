#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:

	// Program ID
	unsigned int ID;

	/*
	* Constructor that builds the shader program from 2 different shaders.
	* @param vertexPath - Path to the vertex shader source code.
	* @param fragmentPath - Path to the fragment shader source code.
	*/
	Shader(const char* vertexPath, const char* fragmentPath);

	~Shader();

	// Use/Activate the shader
	void Use() const;

	// Utility uniform functions

	/*
	* Sets a boolean uniform variable in the shader program.
	* @param name - Name of the uniform variable in the shader.
	* @param value - Value to set the uniform variable to.
	*/
	void SetBool(const std::string& name, bool value) const;

	/*
	* Sets an integer uniform variable in the shader program.
	* @param name - Name of the uniform variable in the shader.
	* @param value - Value to set the uniform variable to.
	*/
	void SetInt(const std::string& name, int value) const;

	/*
	* Sets a float uniform variable in the shader program.
	* @param name - Name of the uniform variable in the shader.
	* @param value - Value to set the uniform variable to.
	*/
	void SetFloat(const std::string& name, float value) const;

	void SetVector2(const std::string& name, const glm::vec2& value) const;

	void SetVector2(const std::string& name, float x, float y) const;

	void SetVector3(const std::string& name, const glm::vec3& value) const;

	void SetVector3(const std::string& name, float x, float y, float z) const;

	void SetVector4(const std::string& name, glm::vec4& value) const;

	void SetVector4(const std::string& name, float x, float y, float z, float w) const;

	void SetMatrix2(const std::string& name, const glm::mat2 mat) const;

	void SetMatrix3(const std::string& name, const glm::mat3 mat) const;

	void SetMatrix4(const std::string& name, const glm::mat4 mat) const;

private:

	/*
	* Utility function to check for shader compilation/linking errors.
	* @param shader - The shader or program ID to check.
	* @param type - Type of shader ("VERTEX", "FRAGMENT", "PROGRAM").
	*/
	void CheckCompileErrors(const unsigned int& shader, const std::string& type);
};


#endif // SHADERS_H