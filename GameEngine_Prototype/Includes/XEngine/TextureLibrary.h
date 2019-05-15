#pragma once
#include "AssetLibrary.h"
//#include "Material.h" //Texture struct definition
#include "Texture.h"
#include <vector>

//TODO: Rework this so that textures are structs, not just openGL textureIDs

class TextureLibrary : public AssetLibrary<std::string, Texture*>
{
public:
	struct TextureQuery
	{
		std::string filePath;
		std::string type;

	};
	TextureLibrary();
	~TextureLibrary();
	Texture*& LoadAsset(std::string filePath) override;

	Texture*& GetCubeMap(std::vector<std::string> faces);
	Texture*& LoadCubeMapTexture(std::vector<std::string> faces);
};

