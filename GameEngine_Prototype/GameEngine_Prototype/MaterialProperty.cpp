#include "MaterialProperty.h"
#include "Serialization.h"
#include <glm/glm.hpp>
#include <string>
//#include "GLM_Serialize.h"
#include "AssetManager.h"

template<class Archive>
void FloatProperty::serialize(Archive &ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
	ar & BOOST_SERIALIZATION_NVP(value);
}
template<class Archive>
void IntProperty::serialize(Archive &ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
	ar & BOOST_SERIALIZATION_NVP(value);
}
template<class Archive>
void Vec2Property::serialize(Archive &ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
	ar & BOOST_SERIALIZATION_NVP(value);
}
template<class Archive>
void Vec3Property::serialize(Archive &ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
	ar & BOOST_SERIALIZATION_NVP(value);
}
template<class Archive>
void Vec4Property::serialize(Archive &ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
	ar & BOOST_SERIALIZATION_NVP(value);
}

template<class Archive>
void TextureProperty::save(Archive &ar, const unsigned int version) const
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
	ar & boost::serialization::make_nvp<std::string>("filePath", value->path);
	if (version > 1)
	{
		ar & boost::serialization::make_nvp<std::string>("textureType", value->type);
	}
}
template<class Archive>
void TextureProperty::load(Archive &ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProperty);
	std::string filePath;
	ar & boost::serialization::make_nvp<std::string>("filePath", filePath);
	LoadTextureFromPath(filePath);

	//value = AssetManager::getInstance().textureLib.GetAsset(filePath);
	if (version > 1)
	{
		//std::string textureType = "";
		ar & boost::serialization::make_nvp<std::string>("textureType", value->type);
		//std::cout << "Texture Type: " << value->type << std::endl;
	}
}


BOOST_CLASS_EXPORT_GUID(FloatProperty, "FloatProperty")
BOOST_CLASS_EXPORT_GUID(IntProperty, "IntProperty")
BOOST_CLASS_EXPORT_GUID(Vec2Property, "Vec2Property")
BOOST_CLASS_EXPORT_GUID(Vec3Property, "Vec3Property")
BOOST_CLASS_EXPORT_GUID(Vec4Property, "Vec4Property")
BOOST_CLASS_EXPORT_GUID(TextureProperty, "TextureProperty")



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
	glBindTexture(GL_TEXTURE_2D, value->id);
	
}

void TextureProperty::Unbind(Shader* shader, unsigned int texIndex)
{
	//glActiveTexture(GL_TEXTURE0 + texIndex);
	//auto loc = glGetUniformLocation(shader->ID, (this->value->type).c_str());
	//glUniform1i(texIndex, texIndex);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
}