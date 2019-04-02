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

// Loads texture into memory using file path, assigns texture ID by calling LoadTexture procedure
void AssetManager::LoadTextureAsset(const char* textureFilePath, unsigned int* textureID, unsigned int loadMode)
{
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	std::string path = ASSET_FILE_PATH + std::string(textureFilePath);
	AssetManager::LoadTexture(path.c_str(), textureID, loadMode);
}

// Loads texture into memory using file path, assigns texture ID
void AssetManager::LoadTexture(const char* textureFilePath, unsigned int* textureID, unsigned int loadMode)
{
	// load and create a texture 
	// -------------------------
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_load_

	unsigned char *data = stbi_load(textureFilePath, &width, &height, &nrChannels, loadMode);
	if (data)
	{
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		auto formatByte = (loadMode == 4)? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, formatByte, width, height, 0, formatByte, GL_UNSIGNED_BYTE, data);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}