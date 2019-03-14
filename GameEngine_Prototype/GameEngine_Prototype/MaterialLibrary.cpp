#include "MaterialLibrary.h"
#include "AssetManager.h"


MaterialLibrary::MaterialLibrary()
{
}


MaterialLibrary::~MaterialLibrary()
{
}

void MaterialLibrary::SaveAsset(std::string filePath, Material * material)
{
	library.insert({ filePath, material });
	std::cout << "Material Saved into Library" << std::endl;
}

Material *& MaterialLibrary::LoadAsset(std::string filePath)
{
	std::cout << "Material Loaded into Library" << std::endl;
	Material* material = library.at(filePath);
	return material;
}
