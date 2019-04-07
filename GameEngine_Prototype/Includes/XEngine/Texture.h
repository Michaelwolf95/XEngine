#pragma once
#include <string>

enum TEXTURE_TYPE { DIFFUSE, SPECULAR, NORMALS, HEIGHT };

struct Texture {
	// texture id
	unsigned int id;
	// texture type
	std::string type;
	// texture path
	std::string path;
};