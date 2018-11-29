#pragma once
#include "Component.h"

class GameObjectAnalytic :
	public Component
{
public:
	GameObjectAnalytic();
	~GameObjectAnalytic();
	void Start() override;
	void Update() override;
};

