#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#include <vector>
#include "RenderableObject.h"

/* The RenderManager is responsible for:
- 
*/

const char* DEFAULT_VertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* DEFAULT_FragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

class RenderManager
{
public:
	static RenderManager* instance;
	static int defaultShaderProgram;
	bool isInitialized = false;

	// Create static instance
	// ToDo: Setup the singleton manager pattern as a base class.
	static RenderManager* CreateManager()
	{
		if (instance != nullptr)
		{
			printf("DUPLICATE SINGLETON DETECTED");
			return NULL;
		}
		instance = new RenderManager();
		instance->Init();
		return instance;
	}

	// Init instance
	int Init()
	{
		defaultShaderProgram = CompileShaders(DEFAULT_VertexShaderSource, DEFAULT_FragmentShaderSource);

		return 0;
	}

	int CompileShaders(const char *vertexShaderSource, const char *fragmentShaderSource)
	{
		// build and compile our shader program
		// ------------------------------------
		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	void Render()
	{
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//ToDo: Loop through all renderables
	}
	void RenderObject(RenderableObject* renderable)
	{
		// draw our first triangle
		glUseProgram(*(renderable->shaderProgram));
		glBindVertexArray(renderable->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time
	}


	void FreeAllResources()
	{
		//ToDo: Loop through all renderables
	}

	void FreeObjectResources(RenderableObject* renderable)
	{
		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &(renderable->VAO));
		glDeleteBuffers(1, &(renderable->VBO));
		glDeleteBuffers(1, &(renderable->EBO));
	}
};