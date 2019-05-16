#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "RenderManager.h"
#include "AssetManager.h"
#include "Texture.h"
class GizmoSpriteDrawer
{
public:
	static float gizmoScale;

	GizmoSpriteDrawer(const char* _texturePath);
	~GizmoSpriteDrawer();
	void Setup();
	void Draw(glm::vec3 position);

	// The VBO containing the 4 vertices of the quad.
	static const GLfloat spriteQuadVerices[];

	std::string gizmoTexturePath;
	// Render state
	GLuint VAO;
	GLuint VBO;
	Texture* spriteTexture;
	glm::vec4 color = glm::vec4(1.0f);
};

