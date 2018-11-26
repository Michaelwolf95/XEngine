#pragma once
#include "RenderableObject.h"
#include "Component.h"
class SimpleModelComponent :
	public RenderableObject, public Component
{
public:
	SimpleModelComponent(float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int* ind, unsigned int numInd, Material* _shader = nullptr);
	~SimpleModelComponent();

	void Setup() override;

	void Draw() override;
	void Start() override;
	void Update() override;
	void OnDrawGizmos() override;
};

