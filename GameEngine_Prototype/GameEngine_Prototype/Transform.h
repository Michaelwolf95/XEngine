#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Component.h"
//class Component;
class Transform : public Component
{
public:
	Transform* parent;
	std::vector<Transform*> children;
	Transform();
	~Transform();
	void SetParent(Transform* _parent);
	void Start() override;
	void Update() override;
	glm::mat4 getMatrix4x4();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);
	glm::vec4 getRotation();
	void setRotation(glm::vec4 rot);

// ToDo: Make this private after making appropriate accessors. For right now use GLM API directly.
//private: 
	glm::mat4 model = glm::mat4(1.0f);
};

