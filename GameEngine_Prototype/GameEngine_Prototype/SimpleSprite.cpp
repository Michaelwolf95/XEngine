#include "SimpleSprite.h"

SimpleSprite::SimpleSprite(float* verts, unsigned int numV, unsigned int* ind, unsigned int numInd, Shader* _shader)
	: RenderableObject(verts, numV, ind, numInd, _shader)
{
	Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void SimpleSprite::Draw()
{
	shader->setColor("MainColor", Color.x, Color.y, Color.z, Color.w);
	RenderableObject::Draw();
}