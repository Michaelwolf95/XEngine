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
#include "serialization.h"

class DLLExport Text : public RenderableObject, public Component
{
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

public:
	static Registrar<Text> registrar;
	//Text();
	Text();
	~Text();
	void Start() override;
	void Update() override;
	void Setup() override;
	void OnEnable() override;
	void Draw() override;
	void DrawInspector() override;
	void Load();
	void setFontSize(FT_UInt);
	void setText(std::string);
	void setColor(glm::vec3);
	void setPos(GLfloat, GLfloat);
	void setScale(GLfloat);
	FT_UInt getFontSize();
	std::string getText();
	glm::vec3 getColor();
	glm::vec2 getPos();
	GLfloat getScale();
	std::string fontPath;

private:
	std::map<GLchar, Character> Characters;
	std::string text;
	FT_UInt fontSize;
	glm::vec3 color;
	GLfloat xPos, yPos, scale;
	GLuint VAO, VBO;
	Shader* shader;

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(fontPath);
		ar & BOOST_SERIALIZATION_NVP(text);
		ar & BOOST_SERIALIZATION_NVP(fontSize);
		ar & BOOST_SERIALIZATION_NVP(color);
		ar & BOOST_SERIALIZATION_NVP(xPos);
		ar & BOOST_SERIALIZATION_NVP(yPos);
		ar & BOOST_SERIALIZATION_NVP(scale);
	}
};