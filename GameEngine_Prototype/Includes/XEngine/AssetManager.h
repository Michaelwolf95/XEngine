#pragma once
#include "LibraryExport.h"
#include "Singleton.h"
#include "TextureLibrary.h"
#include "ModelLibrary.h"
#include "MaterialLibrary.h"
#include "MeshLibrary.h"
#include "ShaderLibrary.h"

class ENGINE_API AssetManager : public Singleton<AssetManager>
{
	friend class Singleton<AssetManager>;
public:
	TextureLibrary textureLib;
	ModelLibrary modelLib;
	MaterialLibrary materialLib;
	MeshLibrary meshLib;
	ShaderLibrary shaderLib;
	AssetManager();
	~AssetManager();
	static AssetManager* CreateManager();
	void Init();

	static void LoadTextureAsset(const char * textureFilePath, unsigned int * textureID, unsigned int loadMode = 3);
	// Loads texture into memory using file path, assigns texture ID
	static void LoadTexture(const char * textureFilePath, unsigned int * textureID, unsigned int loadMode = 3);

	std::string getProjectFilePath();
	void setProjectFilePath(std::string path);
private:
	// This can be redefined by the user.
	std::string projectFilePath = "../";
};

#define PROJECT_FILE_PATH							\
AssetManager::getInstance().getProjectFilePath()	\
/**/

#define ASSET_FILE_PATH								\
(PROJECT_FILE_PATH + std::string("Assets/"))						\
/**/