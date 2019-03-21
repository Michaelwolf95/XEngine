#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
class Light
{
public:
	enum LightType { PointLight = 1, GlobalLight, SpotLight };
	std::string UNIFORM_NAME;
	Light();
	virtual ~Light();
	virtual glm::vec3 getLightColor() = 0;
	virtual glm::vec3 getLightPos() = 0;
	virtual float getIntensity() = 0;
	//virtual glm::vec3 getAmbient() = 0;
	//virtual glm::vec3 getDiffuse() = 0;
	//virtual glm::vec3 getSpecular() = 0;
	virtual float getConstant() = 0;
	virtual float getLinear() = 0;
	virtual float getQuadratic() = 0;
	virtual glm::vec3 getDirection() = 0;
	virtual int getTypeID() = 0;
	//virtual int getCount() = 0;
	virtual void draw(Shader* shader, int &counter) = 0;
	virtual const std::string* getUniformName() = 0;
};

