#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"

// Wrap this at compile time for releases.
// Maybe store path in app config file?
static const char* SHADER_FILE_PATH = "../Shaders/";

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	//std::cout << "Shader constructor called with arguments:\n";
	//std::cout << "\tvertexPath: "	<< vertexPath << std::endl;
	//std::cout << "\tfragmentPath: " << fragmentPath << std::endl;
	//std::cout << "\tgeometryPath: " << (geometryPath == nullptr) ? "" : geometryPath;
	//std::cout << std::endl;

	std::string vShaderPath = SHADER_FILE_PATH + std::string(vertexPath);
	std::string fShaderPath = SHADER_FILE_PATH + std::string(fragmentPath);
	std::string gShaderPath = SHADER_FILE_PATH + std::string((geometryPath==nullptr)? "" : geometryPath);
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vShaderPath);
		fShaderFile.open(fShaderPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "\tvShaderPath: " << vShaderPath << std::endl;
		std::cout << "\tfShaderPath: " << fShaderPath << std::endl;
		std::cout << "\tgShaderPath: " << gShaderPath << std::endl;
	}

	// Must call method without geometry code if none exists.	
	geometryPath ? 
		compileShaders(vertexCode.c_str(), fragmentCode.c_str(), geometryCode.c_str()) :
		compileShaders(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::Shader(const std::string shaderFilePath)
{
	bool result = parseShaderFile(SHADER_FILE_PATH + shaderFilePath);
	//std::cout << "Single File Shader constructor with argument:\n";
	//std::cout << "\tshaderFilePath: " << shaderFilePath << std::endl;
	//std::cout << "Shader file parsing " << result ? "successful!\n" : "failed...\n";
}

// activate the shader
// ------------------------------------------------------------------------
void Shader::use()
{
	glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setColor(const std::string & name, float r, float g, float b, float a) const
{
	int vertexColorLocation = glGetUniformLocation(ID, name.c_str());
	glUniform4f(vertexColorLocation, r, g, b, a);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

std::string Shader::to_string()
{
	std::string str = "";

	str += "\nPrint contents of Shader: ";
	str += "\nID: " + std::to_string(this->ID);

	return str;
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

int Shader::parseShaderFile(const std::string & path)
{
	//std::cout << "Shader::parseShaderFile with argument\n";
	//std::cout << "\tpath: " << path << std::endl;
	enum SUCCESS { failure = -1, success };
	std::string vertexCode = "";
	std::string fragmentCode = "";

	std::ifstream shaderFile;
	// ensure ifstream can throw exception
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		std::string str;
		shaderFile.open(path, std::ifstream::out);
		bool writeToVertex = false;
		bool writeToFragment = false;
		while (!shaderFile.eof())
		{
			getline(shaderFile, str, '\n');
			if (str == "#VERTEX_SHADER") // use vertexCode
			{
				writeToVertex = true;
				writeToFragment = false;
				getline(shaderFile, str, '\n');
			}
			if (str == "#FRAGMENT_SHADER") // use fragmentCode
			{
				writeToVertex = false;
				writeToFragment = true;
				getline(shaderFile, str, '\n');
			}

			if (writeToVertex) vertexCode += str + '\n';
			else if (writeToFragment) fragmentCode += str + '\n';
		}
		shaderFile.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::parseShaderFile::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return SUCCESS::failure;
	}
	catch (std::exception e) 
	{
		std::cout << "ERROR: Problem in Shader::parseShaderFile\n";
	}
	compileShaders(vertexCode.c_str(), fragmentCode.c_str());
	return SUCCESS::success;
}

void Shader::compileShaders(const char * vertexCode, const char * fragmentCode, const char * geometryCode)
{
	std::cout << "Compiling Shader: " << this->vFilePath << std::endl;

	// 2. compile shaders
	unsigned int vertex, fragment, geometry;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	//unsigned int geometry;
	if (geometryCode != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometryCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryCode != nullptr)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryCode != nullptr)
		glDeleteShader(geometry);
}
