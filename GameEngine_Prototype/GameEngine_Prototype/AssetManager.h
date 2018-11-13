#pragma once
static const char* ASSET_FILE_PATH = "../Assets/";
class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	static void LoadTexture(const char * textureFilePath, unsigned int * textureID);
	
};

