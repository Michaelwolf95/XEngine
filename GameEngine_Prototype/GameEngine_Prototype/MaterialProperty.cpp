#include "MaterialProperty.h"
#include "Serialization.h"

//template<typename T>
//MaterialProperty<T>::MaterialProperty(std::string name, T val) {
//	propertyName = name;
//	value = val;
//}

BOOST_CLASS_EXPORT_GUID(FloatProperty, "FloatProperty")
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

int IntProperty::getValue()
{
	return value;
}

void IntProperty::setValue(int val)
{
	value = val;
}

glm::vec2 Vec2Property::getValue()
{
	return value;
}

void Vec2Property::setValue(glm::vec2 val)
{
	value = val;
}

glm::vec3 Vec3Property::getValue()
{
	return value;
}

void Vec3Property::setValue(glm::vec3 val)
{
	value = val;
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
