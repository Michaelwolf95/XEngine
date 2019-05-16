#include "MaterialProperty.h"
#include "Serialization.h"
#include <glm/glm.hpp>
#include <string>
//#include "GLM_Serialize.h"
#include "AssetManager.h"

BOOST_CLASS_EXPORT_GUID(FloatProperty, "FloatProperty")
BOOST_CLASS_EXPORT_GUID(IntProperty, "IntProperty")
BOOST_CLASS_EXPORT_GUID(Vec2Property, "Vec2Property")
BOOST_CLASS_EXPORT_GUID(Vec3Property, "Vec3Property")
BOOST_CLASS_EXPORT_GUID(Vec4Property, "Vec4Property")
BOOST_CLASS_EXPORT_GUID(TextureProperty, "TextureProperty")

BOOST_CLASS_VERSION(TextureProperty, 2);

// Used by serialization.
void TextureProperty::LoadTextureFromPath(std::string filePath)
{
	value = AssetManager::getInstance().textureLib.GetAsset(filePath);
}

void TextureProperty::Reload()
{
	this->setValue(AssetManager::getInstance().textureLib.Reload(this->value));
}

void TextureProperty::Bind(Shader* shader, unsigned int texIndex)
{
	glActiveTexture(GL_TEXTURE0 + texIndex);
	auto loc = glGetUniformLocation(shader->ID, (this->value->type).c_str());
	glUniform1i(loc, texIndex);

	// bind texture
	glBindTexture(GL_TEXTURE_2D,value->id);
	
}

void TextureProperty::Unbind(Shader* shader, unsigned int texIndex)
{
	//glActiveTexture(GL_TEXTURE0 + texIndex);
	//auto loc = glGetUniformLocation(shader->ID, (this->value->type).c_str());
	//glUniform1i(texIndex, texIndex);

	// bind texture
	glBindTexture(GL_TEXTURE_2D, 0);

}