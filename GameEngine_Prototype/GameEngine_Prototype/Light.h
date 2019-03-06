#pragma once
#include <glm/glm.hpp>
class Light
{
public:
	Light();
	virtual ~Light();
	virtual glm::vec3 getLightColor() = 0;
	virtual glm::vec3 getLightPos() = 0;
	virtual int getTypeID() = 0;
};

