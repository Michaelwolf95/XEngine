#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
	int* shaderProgram;
	RenderableObject(float* verts, unsigned int numV, unsigned int* ind, unsigned int numInd);
	void Setup();
};