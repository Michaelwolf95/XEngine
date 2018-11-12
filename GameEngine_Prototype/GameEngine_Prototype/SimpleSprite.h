#pragma once
#include "RenderableObject.h"
#include <glm/glm.hpp>
class SimpleSprite : public RenderableObject
{
public:
	glm::vec4 Color;
	SimpleSprite(float* verts, unsigned int numV, unsigned int* ind, unsigned int numInd, Shader* _shader = nullptr);
	void Draw() override;
};

