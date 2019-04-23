#pragma once
#include "Component.h"
//#include "PrimitiveModels.h"

class GameObjectAnalytic :
	public Component
{
public:
	GameObjectAnalytic();
	~GameObjectAnalytic();
	void Start() override;
	void Update() override;
	GameObjectAnalytic * DeepCopy() override;
};

