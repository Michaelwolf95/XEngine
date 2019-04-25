#include "Text.h"

const std::string FILE_PATH = "../Assets/Fonts/";

REGISTER_COMPONENT(Text, "Text")

//Text::Text() {}

Text::Text(const char *filePath, FT_UInt size)
{
	this->fontPath = filePath;
	this->fontSize = size;

	Setup();
}

Text::~Text()
{
	RenderManager::getInstance().RemoveRenderable((RenderableObject*)this);
}

void Text::setFontSize(FT_UInt size)
{
	this->fontSize = size;
}

void Text::setText(std::string newText)
{
	this->text = newText;
}

void Text::setColor(glm::vec3 colors)
{
	this->color = colors;
}

void Text::setPos(GLfloat xPos, GLfloat yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
}

void Text::setScale(GLfloat scaleSz)
{
	this->scale = scaleSz;
}

FT_UInt Text::getFontSize()
{
	return this->fontSize;
}

std::string Text::getText()
{
	return this->text;
}

glm::vec3 Text::getColor()
{
	return this->color;
}

glm::vec2 Text::getPos()
{
	glm::vec2 position = glm::vec2(this->xPos, this->yPos);

	return position;
}

GLfloat Text::getScale()
{
	return this->scale;
}

void Text::Start() {}
void Text::Update() {}

void Text::Setup()
{
	setText("Text");
	setColor(glm::vec3(255.0, 255.0f, 255.0f));
	setPos(20.0f, 20.0f);
	setScale(1.0f);

	render_enabled = true;
	RenderManager::getInstance().AddRenderable((RenderableObject*)this);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->shader = RenderManager::defaultTextShader;

	this->shader->use();
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800.0f), static_cast<GLfloat>(600.0f), 0.0f);
	this->shader->setMat4("projection", projection);
	this->shader->setInt("text", 0);
	
	this->Load();
}

void Text::Draw()
{
	GLfloat x = this->xPos;
	GLfloat y = this->yPos;

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate corresponding render state	
	this->shader->use();
	this->shader->setVec3("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = this->text.begin(); c != this->text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * this->scale;
		GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * this->scale;

		GLfloat w = ch.Size.x * this->scale;
		GLfloat h = ch.Size.y * this->scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph
		x += (ch.Advance >> 6) * this->scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::DrawInspector()
{
	glm::vec2 pos = this->getPos();
	ImGui::InputFloat2("Position", (float*)&pos);
	if (pos != this->getPos())
	{
		this->setPos(pos.x, pos.y);
	}
	GLfloat guiScale = this->getScale();
	ImGui::InputFloat("Scale", (float*)&guiScale);
	if (guiScale != this->getScale())
	{
		this->setScale(guiScale);
	}
	glm::vec3 guiColor = this->getColor();
	ImGui::ColorEdit3("Color", (float*)&guiColor);
	if (guiColor != this->getColor())
	{
		this->setColor(guiColor);
	}
	const char* arr = this->text.c_str();
	ImGui::InputText("Text", (char*)arr, 256);
	std::string newText(arr);
	if (newText != this->getText())
	{
		this->setText(newText);
	}
	FT_UInt size = this->getFontSize();
	ImGui::InputInt("Font Size", (int*)&size);
	if (size != this->getFontSize())
	{
		this->setFontSize(size);
		this->Load();
	}
}

void Text::Load()
{
	std::string fullPath = FILE_PATH + this->fontPath;

	// First clear the previously loaded Characters
	this->Characters.clear();
	// Then initialize and load the FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) // All functions return a value different than 0 whenever an error occurred
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, fullPath.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, this->fontSize);
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Then for the first 128 ASCII characters, pre-load/compile their characters and store them
	for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}