#include "TextureLibrary.h"
#include "AssetManager.h"
#include "Texture.h"
#include "stb/stb_image.h"

TextureLibrary::TextureLibrary() {}

TextureLibrary::~TextureLibrary() {}

Texture *& TextureLibrary::GetAsset(std::string filePath)
{
	return GetAsset(filePath, 3);
}

Texture *& TextureLibrary::GetAsset(std::string filePath, unsigned int loadMode)
{
	auto search = this->library.find(filePath);
	if (search == this->library.end())
	{
		// Not found.
		//std::cout << "\tAsset not found!" << std::endl;
		//std::cout << "\t\tkey: " << key << std::endl;
		return LoadAsset(filePath, loadMode);
	}
	else
	{
		// Found
		//std::cout << "\tAsset found!" << std::endl;
		return this->library[filePath];
	}
}


// Checks texture library for existing texture, if exists, no new texture is created, if not, stores a new texture.
// Calls LoadTextureAsset to load texture into memory and assign texture ID.
Texture* & TextureLibrary::LoadAsset(std::string filePath)
{
	return LoadAsset(filePath, 3);
}

Texture *& TextureLibrary::LoadAsset(std::string filePath, unsigned int loadMode)
{
	std::cout << "Loading Texture:" << filePath << std::endl;
	Texture* text = new Texture();
	text->path = filePath;
	text->loadMode = loadMode;

	library.insert({ filePath, text });
	LoadTexture(filePath.c_str(), &library[filePath]->id, loadMode);
	return library[filePath];
}

Texture* & TextureLibrary::Reload(Texture* texture)
{
	auto search = this->library.find(texture->path);
	if (search != this->library.end())
	{
		// Found.
		// Just reload the texture with values.
		LoadTexture(texture->path.c_str(), &library[texture->path]->id, texture->loadMode);
		return library[texture->path];
	}
	else
	{
		// Not Found.
		// Need to create a new library entry.
		// TODO: Remove old entry if possible.
		std::cout << "RELOADING TEXTURE IN LIBRARY! ====================\n";
		return LoadAsset(texture->path, texture->loadMode);
	}

}

void TextureLibrary::LoadTexture(const char * textureFilePath, unsigned int * textureID, unsigned int loadMode)
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
		auto formatByte = (loadMode == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, formatByte, width, height, 0, formatByte, GL_UNSIGNED_BYTE, data);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << textureFilePath << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture*& TextureLibrary::GetCubeMap(std::vector<std::string> faces)
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
Texture* & TextureLibrary::LoadCubeMapTexture(std::vector<std::string> faces)
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	library.insert({ text->path, text });
	return library[text->path];
}