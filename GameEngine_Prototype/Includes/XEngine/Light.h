#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
class Light
{
public:
	enum LightType { PointLight = 1, GlobalLight, SpotLight } TYPE;
	std::string UNIFORM_NAME;
	Light();
	virtual ~Light();
	virtual glm::vec3 getLightPos() = 0;
	virtual glm::vec3 getDirection() = 0;
	virtual int getTypeID() = 0;
	virtual void Draw(Shader* shader, int &counter) = 0;
	virtual const std::string* getUniformName() = 0;
};

struct PointLight {
	glm::vec3 color;
	float intensity;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	// shares attributes with PointLight
	PointLight source;

	// unique attributes
	glm::vec3 direction;
	float cutOff;
};

struct GlobalLight { // AKA DirLight
	glm::vec3 color;
	float intensity;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
