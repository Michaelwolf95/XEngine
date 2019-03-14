#pragma once
#include "Singleton.h"
#include "TextureLibrary.h"
#include "ModelLibrary.h"
#include "MaterialLibrary.h"
#include "MeshLibrary.h"
//#include <stb/stb_image.h>

static const char* ASSET_FILE_PATH = "../Assets/";

class AssetManager : public Singleton<AssetManager>
{
	friend class Singleton<AssetManager>;
public:
	TextureLibrary textureLib;
	ModelLibrary modelLib;
	MaterialLibrary materialLib;
	MeshLibrary meshLib;
	AssetManager();
	~AssetManager();
	static AssetManager* CreateManager();
	void Init();

	static void LoadTextureAsset(const char * textureFilePath, unsigned int * textureID, unsigned int loadMode = 3);
	static void LoadTexture(const char * textureFilePath, unsigned int * textureID, unsigned int loadMode = 3);
};

