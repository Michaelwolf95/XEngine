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
	Texture*& GetAsset(std::string filePath) override;
	Texture*& GetAsset(std::string filePath, unsigned int loadMode);
	Texture*& LoadAsset(std::string filePath) override;
	Texture*& LoadAsset(std::string filePath, unsigned int loadMode);

	Texture* & Reload(Texture* texture);


	// Loads texture into memory using file path, assigns texture ID
	void LoadTexture(const char * textureFilePath, unsigned int * textureID, unsigned int loadMode = 3);

	Texture*& GetCubeMap(std::vector<std::string> faces);
	Texture*& LoadCubeMapTexture(std::vector<std::string> faces);
};

