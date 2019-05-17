#pragma once
#include <glm/glm.hpp>
//#include <stb/stb_image.h>
#include "RenderableObject.h"
class SimpleSprite : public RenderableObject
{
public:
	//taken from renderableobject
	float* vertices;
	unsigned int numVerts;
	unsigned int vertexDataSize;
	unsigned int* indices;
	unsigned int numIndices;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	Material* material = nullptr;
	//
	//unsigned int textureID = 0;
	//glm::vec4 Color;
	SimpleSprite(float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int* ind, unsigned int numInd, Material* _shader = nullptr);
	
	//void LoadTexture(const char* textureFilePath);
	void Draw() override;

	void Setup() override;
};

