#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stb/stb_image.h>
#include "AssetManager.h"


AssetManager::AssetManager() {}
AssetManager::~AssetManager() {}

AssetManager* AssetManager::CreateManager()
{
	AssetManager* instance = &AssetManager::getInstance();

	instance->Init();
	return instance;
}

void AssetManager::Init()
{
	stbi_set_flip_vertically_on_load(1);
}

std::string AssetManager::getProjectFilePath()
{
	return projectFilePath;
}

void AssetManager::setProjectFilePath(std::string path)
{
	projectFilePath = path;
}

bool AssetManager::FileExists(std::string path)
{
	std::ifstream ifs(path);
	return ifs.good();
}
