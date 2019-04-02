#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Material.h"

class RenderableObject
{	
public:
	bool render_enabled = false;
	virtual void Draw() = 0;
	virtual void Setup() = 0;
	//float* vertices;
	//unsigned int numVerts;
	//unsigned int vertexDataSize;
	//unsigned int* indices;
	//unsigned int numIndices;
	//unsigned int VBO;
	//unsigned int VAO;
	//unsigned int EBO;
	//Material* material = nullptr;
	//RenderableObject();
	//RenderableObject(Material* _material);
	//RenderableObject(
	//	float* verts, unsigned int numV, unsigned int vertDataSize,
	//	unsigned int* ind, unsigned int numInd, Material* _material = nullptr);
	//~RenderableObject();
};