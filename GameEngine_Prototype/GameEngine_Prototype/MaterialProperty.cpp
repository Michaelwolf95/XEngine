#include "MaterialProperty.h"

float FloatProperty::getValue()
{
	return value;
}

void FloatProperty::setValue(float val)
{
	value = val;
}

Texture& TextureProperty::getValue()
{
	return Texture();
}

void TextureProperty::setValue(Texture& val)
{
}
