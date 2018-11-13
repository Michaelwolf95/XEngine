#pragma once
#include <glm/glm.hpp>
//#include <stb/stb_image.h>
#include "RenderableObject.h"
class SimpleSprite : public RenderableObject
{
public:
	//unsigned int textureID = 0;
	//glm::vec4 Color;
	SimpleSprite(float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int* ind, unsigned int numInd, Material* _shader = nullptr);
	
	//void LoadTexture(const char* textureFilePath);
	void Draw() override;

	void Setup() override;
};

