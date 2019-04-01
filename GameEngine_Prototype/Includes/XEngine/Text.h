#pragma once

// Std. Includes
#include <iostream>
#include <map>
#include <string>
// GLAD
#include <glad/glad.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
//Other
#include "Shader.h"
#include "RenderManager.h"
#include "RenderableObject.h"
#include "Component.h"
#include "Material.h"

class Text : public RenderableObject, public Component
{
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

public:
	Text(const char*, FT_UInt, Material* m = nullptr);
	~Text();
	void Setup() override;
	void Draw() override;
	void Start() override;
	void Update() override;
	void setText(std::string);
	void setColor(glm::vec3);
	void setPosandScale(GLfloat, GLfloat, GLfloat);
	void RenderText(std::string, GLfloat, GLfloat, GLfloat, glm::vec3);

private:
	FT_Library ft;
	FT_Face face;
	FT_UInt fontSize;
	std::map<GLchar, Character> Characters;
	std::string fontPath;
	GLuint VAO;
	GLuint VBO;
	std::string text;
	GLfloat x, y, scale;
	glm::vec3 color;
};