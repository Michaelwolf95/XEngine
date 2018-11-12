#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

class RenderableObject
{
public:
	float* vertices;
	unsigned int* indices;
	unsigned int numVerts;
	unsigned int numIndices;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	Shader* shader;
	bool enabled = false;
	RenderableObject(float* verts, unsigned int numV, unsigned int* ind, unsigned int numInd, Shader* _shader = nullptr);
	~RenderableObject();
	virtual void Setup();
	virtual void Draw();
};