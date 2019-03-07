#pragma once
#include <glm/glm.hpp>
class Light
{
public:
	Light();
	virtual ~Light();
	virtual glm::vec3 getLightColor() = 0;
	virtual glm::vec3 getLightPos() = 0;
	virtual float getIntensity() = 0;
	virtual glm::vec3 getAmbient() = 0;
	virtual glm::vec3 getDiffuse() = 0;
	virtual glm::vec3 getSpecular() = 0;
	virtual float getConstant() = 0;
	virtual float getLinear() = 0;
	virtual float getQuadratic() = 0;
	virtual glm::vec3 getDirection() = 0;
	virtual int getTypeID() = 0;
};

