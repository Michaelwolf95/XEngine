#pragma once
#include "RenderableObject.h"
#include "Component.h"
class SimpleModel :
	public RenderableObject
{
public:
	// taken from renderableobject
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
	SimpleModel(float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int* ind, unsigned int numInd, Material* _shader = nullptr);
	~SimpleModel();

	void Setup() override;

	void Draw() override;

};

