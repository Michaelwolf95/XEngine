#pragma once
#include "AssetLibrary.h"
#include "Material.h" //Texture struct definition

//TODO: Rework this so that textures are structs, not just openGL textureIDs

class TextureLibrary : public AssetLibrary<std::string, Texture>
{
public:
	struct TextureQuery
	{
		std::string filePath;
		std::string type;

	};
	TextureLibrary();
	~TextureLibrary();
	Texture& LoadAsset(std::string filePath) override;
};

