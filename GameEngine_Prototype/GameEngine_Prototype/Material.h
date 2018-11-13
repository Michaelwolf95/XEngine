#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Material
{
public:
	Shader* shader;
	const char* textureFilePath;
	unsigned int textureID = 0;
	glm::vec4 Color;
	Material(Shader* _shader = nullptr);
	~Material();
	void Load(); // Load for openGL to draw.

	void LoadTexture(const char* _textureFilePath);
};

