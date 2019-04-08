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

////////

void TextureProperty::LoadTextureFromPath(std::string filePath)
{
	value = &AssetManager::getInstance().textureLib.GetAsset(filePath);
}
