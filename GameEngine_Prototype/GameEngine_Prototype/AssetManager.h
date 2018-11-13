#pragma once
#include "Singleton.h"

static const char* ASSET_FILE_PATH = "../Assets/";

class AssetManager : public Singleton<AssetManager>
{
	friend class Singleton<AssetManager>;
public:
	AssetManager();

	static void LoadTexture(const char * textureFilePath, unsigned int * textureID);
	
};

