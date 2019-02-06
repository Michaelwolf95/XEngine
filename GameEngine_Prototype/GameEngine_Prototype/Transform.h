#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"
#include "Serialization.h"
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

// ToDo: Make this private after making appropriate accessors. For right now use GLM API directly.
private: 
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 translateMatrix = glm::mat4(1.0f);
	glm::mat4 rotateMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	//glm::quat localRotation = glm::quat();
	void UpdateMatrix();

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & boost::serialization::base_object<Component>(*this);
		//ar & street1 & street2;
	}
};

