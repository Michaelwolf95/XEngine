#pragma once
#include "RenderableObject.h"
class SimpleModel :
	public RenderableObject
{
public:
	SimpleModel(float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int* ind, unsigned int numInd, Material* _shader = nullptr);
	~SimpleModel();

	void Setup() override;

	void Draw() override;
};

