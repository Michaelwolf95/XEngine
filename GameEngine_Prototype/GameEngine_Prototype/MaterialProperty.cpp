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


FloatProperty::FloatProperty()
{
	value = 0.0f;
}


float FloatProperty::getValue()
{
	return value;
}

void FloatProperty::setValue(float val)
{
	value = val;
}

IntProperty::IntProperty()
{
	value = 0;
}

int IntProperty::getValue()
{
	return value;
}

void IntProperty::setValue(int val)
{
	value = val;
}

Vec2Property::Vec2Property()
{
	value = glm::vec2();
}

glm::vec2 Vec2Property::getValue()
{
	return value;
}

void Vec2Property::setValue(glm::vec2 val)
{
	value = val;
}

Vec3Property::Vec3Property()
{
	value = glm::vec3();
}

glm::vec3 Vec3Property::getValue()
{
	return value;
}

void Vec3Property::setValue(glm::vec3 val)
{
	value = val;
}

Vec4Property::Vec4Property()
{
	value = glm::vec4();
}

glm::vec4 Vec4Property::getValue()
{
	return value;
}

void Vec4Property::setValue(glm::vec4 val)
{
	value = val;
}

TextureProperty::TextureProperty()
{
	value = nullptr;
}

Texture* TextureProperty::getValue()
{
	return value;
}

void TextureProperty::setValue(Texture* val)
{
	value = val;
}

// TODO: Given that a texture has an enum type this method will return a property name that will connect with texture uniforms in shaders
//std::string TextureProperty::getPropertyName()
//{
//	switch (value->type)
//	{
//		case 
//	}
//}

void TextureProperty::LoadTextureFromPath(std::string filePath)
{
	value = &AssetManager::getInstance().textureLib.GetAsset(filePath);
}
