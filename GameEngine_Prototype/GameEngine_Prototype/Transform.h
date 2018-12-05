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
	glm::vec3 getLocalPosition();
	void setLocalPosition(glm::vec3 pos);
	void setLocalPosition(float x, float y, float z);
	glm::quat getLocalRotation();
	glm::vec3 getLocalRotationEuler();
	void setLocalRotation(glm::quat rot);
	void setLocalRotationEuler(glm::vec3 rot);
	void setLocalRotationEuler(float x, float y, float z);
	glm::vec3 getLocalScale();
	void setLocalScale(glm::vec3 scale);
	void setLocalScale(float x, float y, float z);

	void Translate(glm::vec3 translation);
	void Rotate(glm::vec3 rotation);
	void Scale(glm::vec3 scale);
	void LookAt(glm::vec3 lookPos, glm::vec3 up = glm::vec3(0,1,0));

	glm::mat4 getTranslationMatrix();
	glm::mat4 getRotationMatrix();
	glm::mat4 getScaleMatrix();

	glm::vec3 getRightDirection();
	glm::vec3 getUpDirection();
	glm::vec3 getForwardDirection();

	void DrawGizmo();

	void printTransformMatrix();

	void TestEulerRotation(float x, float y, float z);

private: 
	// These get serialized
	glm::vec3 localPosition = glm::vec3(0, 0, 0);
	glm::quat localRotation = glm::quat(1, 0, 0, 0);
	glm::vec3 localScale = glm::vec3(1, 1, 1);

	// Flag for whether the transform has been updated since the model was last updated. 
	bool isDirty = false; 

	glm::mat4 model = glm::mat4(1.0f);
	
	glm::mat4 translateMatrix = glm::mat4(1.0f);
	glm::mat4 rotateMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	void UpdateModelMatrix();
};

