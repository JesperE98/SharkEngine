#include "Shader.h"
#include "Math/MathUtils.h"
#include "Core/Utilities/Debug.h"
#include "Managers/PathManager.h"

#include <glad/glad.h>
#include <fstream>
#include <filesystem>

namespace Shark::Graphics {

	using Shark::Managers::PathManager;

	Shader::Shader(const char* vertexPath, const char* fragmentPath) {

		SHARK_LOG(Material, "Shader::Shader() - Creating Shader Program.");

		std::string vPath = PathManager::GetInstance().GetContentPath(vertexPath);
		std::string fPath = PathManager::GetInstance().GetContentPath(fragmentPath);

		SHARK_LOG(Material, "Shader::Shader() - Vertex Shader Path: {}", vPath);

		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// Ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vShaderFile.open(vPath.c_str()); // Open the file
			fShaderFile.open(fPath.c_str()); // Open the file

			std::stringstream vShaderStream, fShaderStream; // Create string streams

			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf(); // Read the file
			fShaderStream << fShaderFile.rdbuf(); // Read the file

			// close file handlers
			vShaderFile.close();
			fShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			SHARK_ERR(Material, "Shader::Shader() - Failed to read shader files at paths: {} and {}", vPath, fPath);
			SHARK_ERR(Material, "Shader::Shader() - ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
			SHARK_ERR(Material, "Exception: {}", e.what());
		}

		const char* vShaderCode = vertexCode.c_str(); // Convert to c-string
		const char* fShaderCode = fragmentCode.c_str(); // Convert to c-string

		// 2. Compile shaders
		unsigned int vertex, fragment;

		// vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, "VERTEX");

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, "FRAGMENT");

		// Shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		CheckCompileErrors(ID, "PROGRAM");

		// Delete shaders as they are linked into our program now and no longer are necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader() {
		ID = 0;
	}

	void Shader::Use() const {
		glUseProgram(ID);
	}

	void Shader::SetBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void Shader::SetInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::SetFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::SetVector2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void Shader::SetVector2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void Shader::SetVector3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void Shader::SetVector3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void Shader::SetVector4(const std::string& name, glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void Shader::SetVector4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void Shader::SetMatrix2(const std::string& name, const glm::mat2 mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::SetMatrix3(const std::string& name, const glm::mat3 mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::SetMatrix4(const std::string& name, const glm::mat4 mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::CheckCompileErrors(const unsigned int& shader, const std::string& type) {
		int success;
		char infoLog[1024];

		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				SHARK_ERR(Material, "Shader::CheckCompileErrors() - ERROR::SHADER_COMPILATION_ERROR of type:{}\n{}\n", type, infoLog);
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				SHARK_ERR(Material, "Shader::CheckCompileErrors() - ERROR::PROGRAM_LINKING_ERROR of type:{}\n{}\n", type, infoLog);
			}
		}
	}
} // namespace Shark::Graphics