#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Shark::Graphics {
	class Shader {
	public:

		// Program ID
		unsigned int ID;
		/*
		* Constructor that builds the shader program from 2 different shaders.
		* @param vertPath - Path to the vertex shader source code.
		* @param fragPath - Path to the fragment shader source code.
		*/
		explicit Shader(const char* vertPath, const char* fragPath);

		/*
		* Constructor that builds the shader program from 2 different shaders.
		* @param vertPath - Path to the vertex shader source code.
		* @param fragPath - Path to the fragment shader source code.
		* @param geomPath - Path to the geometry shader source code.
		*/
		Shader(const char* vertPath, const char* fragPath, const char* geomPath);

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


		void CreateShaderProgram(const char* vertPath, const char* fragPath);
		void CreateShaderProgram(const char* vertPath, const char* fragPath, const char* geomPath);
	};
}
#endif // SHADERS_H