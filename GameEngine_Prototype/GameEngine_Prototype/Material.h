#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"

/* TODO: The information we want to store about each material
should be stored in "material modules" that can be attached to it.
For now, we can just use some flags to check if its lit or not.
*/
class Material
{
public:
	Shader* shader;
	const char* textureFilePath;
	unsigned int textureID = 0;
	bool useLight = false;
	float ambientStrength = 1.0f;
	float specularStrength = 0.5f;
	glm::vec4 Color;
	Material(Shader* _shader = nullptr, bool _useLight = false);
	~Material();
	void Load(); // Load for openGL to draw.

	void LoadTexture(const char* _textureFilePath);
};

