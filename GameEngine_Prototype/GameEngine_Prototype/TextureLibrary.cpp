#include "TextureLibrary.h"
#include "AssetManager.h"

TextureLibrary::TextureLibrary() {}

TextureLibrary::~TextureLibrary() {}

unsigned int & TextureLibrary::LoadAsset(std::string filePath)
{
	library.insert({ filePath, 0u });
	std::string fullPath = ASSET_FILE_PATH + std::string(filePath);
	AssetManager::LoadTextureAsset(fullPath.c_str(), &library[filePath]);
	return library[filePath];
}
