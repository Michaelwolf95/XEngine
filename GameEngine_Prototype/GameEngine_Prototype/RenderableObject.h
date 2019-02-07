#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Material.h"

class RenderableObject
{
	
public:
	float* vertices;
	unsigned int numVerts;
	unsigned int vertexDataSize;
	unsigned int* indices;
	unsigned int numIndices;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	Material* material;
	bool enabled = false;
	RenderableObject();
	RenderableObject(
		float* verts, unsigned int numV, unsigned int vertDataSize,
		unsigned int* ind, unsigned int numInd, Material* _material = nullptr);
	~RenderableObject();
	virtual void Draw();
	virtual void Setup();
};