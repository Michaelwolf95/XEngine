#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "RenderManager.h"
#include "AssetManager.h"
class GizmoSpriteDrawer
{
public:
	GizmoSpriteDrawer(const char* _texturePath);
	~GizmoSpriteDrawer();
	void Setup();
	void Draw(glm::vec3 position);

	// The VBO containing the 4 vertices of the quad.
	static const GLfloat spriteQuadVerices[];

	std::string gizmoTexturePath;
	glm::vec4 color;
	// Render state
	GLuint VAO;
	GLuint VBO;
	unsigned int textureID;
};

