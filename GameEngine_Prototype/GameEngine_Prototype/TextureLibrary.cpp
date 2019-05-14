#include "TextureLibrary.h"
#include "AssetManager.h"
#include "Texture.h"
#include "stb/stb_image.h"

TextureLibrary::TextureLibrary() {}

TextureLibrary::~TextureLibrary() {}

// Checks texture library for existing texture, if exists, no new texture is created, if not, stores a new texture.
// Calls AssetManager::LoadTextureAsset to load texture into memory and assign texture ID.
Texture & TextureLibrary::LoadAsset(std::string filePath)
{
	std::cout << "TextureLibrary::LoadAsset with arguments\n";
	std::cout << "\tfilePath: " << filePath << std::endl;
	Texture* text = new Texture();
	text->path = filePath;

	library.insert({ filePath, *text });

	// if does not contain the directory 'Assets'
	std::string fullPath;
	//if (filePath.find("Assets") == std::string::npos)
	//	fullPath = ASSET_FILE_PATH + std::string(filePath);
	//else
		fullPath = std::string(filePath);

	AssetManager::LoadTextureAsset(fullPath.c_str(), &library[filePath].id); // <-- Maybe this should be in this class too, instead of just in asset manager.
	return library[filePath];
}

Texture& TextureLibrary::GetCubeMap(std::vector<std::string> faces)
{
	std::string key = "CUBEMAP";
	for (size_t i = 0; i < faces.size() && i < 6; i++)
	{
		key += "|" + faces[i];
	}
	auto search = this->library.find(key);
	if (search == this->library.end())
	{
		// Not found.
		return LoadCubeMapTexture(faces);
	}
	else
	{
		// Found
		return this->library[key];
	}
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
Texture & TextureLibrary::LoadCubeMapTexture(std::vector<std::string> faces)
{
	Texture* text = new Texture();
	text->path = "CUBEMAP";
	for (size_t i = 0; i < faces.size() && i < 6; i++)
	{
		text->path += "|" + faces[i];
	}

	glGenTextures(1, &text->id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, text->id);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	library.insert({ text->path, *text });

	return library[text->path];
}

/*
// TODO: Move this to TextureLibrary..?
unsigned int TextureLibrary::TextureFromFile(const char * path, const std::string &directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;

		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
*/