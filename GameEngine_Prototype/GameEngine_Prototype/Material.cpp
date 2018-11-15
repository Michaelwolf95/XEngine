#include "Material.h"
#include "RenderManager.h"
#include "Shader.h"
#include "AssetManager.h"

Material::Material(Shader* _shader, bool _useLight)
{
	if (_shader == nullptr)
	{
		shader = RenderManager::defaultShader;
	}
	else
	{
		shader = _shader;
	}
	useLight = _useLight;
	Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Material::~Material()
{
}

void Material::Load()
{

	shader->setColor("MainColor", Color.x, Color.y, Color.z, Color.w);

	if (textureID > 0)
	{
		// bind Texture
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	else
		glBindTexture(GL_TEXTURE_2D, 0);
}

void Material::LoadTexture(const char * _textureFilePath)
{
	textureFilePath = _textureFilePath;
	AssetManager::LoadTexture(textureFilePath, &textureID);
}
