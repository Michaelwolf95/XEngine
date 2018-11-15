#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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
	glm::quat getLocalRotation();
	void setLocalRotation(glm::quat rot);
	void setLocalRotationEuler(glm::vec3 rot);
	glm::vec3 getLocalScale();
	void setLocalScale(glm::vec3 scale);

	glm::mat4 getTranslationMatrix();
	glm::mat4 getRotationMatrix();
	glm::mat4 getScaleMatrix();

	glm::vec3 getRightDirection();
	glm::vec3 getUpDirection();
	glm::vec3 getForwardDirection();

	void DrawGizmo();

	void printTransformMatrix();

// ToDo: Make this private after making appropriate accessors. For right now use GLM API directly.
//private: 
	glm::mat4 model = glm::mat4(1.0f);
};

