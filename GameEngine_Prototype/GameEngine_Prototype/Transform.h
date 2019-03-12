#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"
#include "Serialization.h"
#include "GLM_Serialize.h"
//class Component;
class Transform : public Component
{
public:
	static Registrar<Transform> registrar;
	Transform();
	~Transform();
	void Start() override;
	void Update() override;
	// Hierarchy Access
	Transform* GetParent();
	std::vector<Transform*> GetChildren();
	unsigned int GetChildCount();
	void SetParent(Transform* _parent);
	// Matrix Access
	glm::mat4 getMatrix4x4();
	glm::mat4 getTranslationMatrix();
	glm::mat4 getRotationMatrix();
	glm::mat4 getScaleMatrix();
	// Position
	glm::vec3 getLocalPosition();
	glm::vec3 getPosition();
	void setLocalPosition(float x, float y, float z);
	void setLocalPosition(glm::vec3 pos);
	// Rotation
	glm::quat getLocalRotation();
	glm::quat getRotation();
	void setLocalRotation(glm::quat rot);
	glm::vec3 getLocalRotationEuler();
	void setLocalRotationEuler(float x, float y, float z);
	void setLocalRotationEuler(glm::vec3 rot);
	// Scale
	glm::vec3 getLocalScale();
	glm::vec3 getScale();
	void setLocalScale(float x, float y, float z);
	void setLocalScale(glm::vec3 scale);
	// GLM Matrix Functions
	void Translate(glm::vec3 translation);
	void Rotate(glm::vec3 rotation);
	void Scale(glm::vec3 scale);
	void LookAt(glm::vec3 lookPos, glm::vec3 up = glm::vec3(0,1,0));
	// Directions
	glm::vec3 getRightDirection();
	glm::vec3 getUpDirection();
	glm::vec3 getForwardDirection();

	// Gizmo
	void DrawGizmo();
	// Editor Utility
	void DrawInspector() override;
	void printTransformMatrix();

	void TestEulerRotation(float x, float y, float z);

private:
	friend class GameObject;
	Transform* parent = nullptr;
	std::vector<Transform*> children;

	glm::vec3 localPosition = glm::vec3();
	glm::quat localRotation = glm::quat();
	glm::vec3 localScale = glm::vec3(1.0);
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 translateMatrix = glm::mat4(1.0f);
	glm::mat4 rotateMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	void UpdateMatrix();
	glm::vec3 _calcLocalPositionFromMatrix();
	glm::quat _calcLocalRotationFromMatrix();
	glm::vec3 _calcLocalScaleFromMatrix();

	// SERIALIZATION
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		//// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(parent);
		ar & BOOST_SERIALIZATION_NVP(localPosition);
		ar & BOOST_SERIALIZATION_NVP(localScale);
		ar & BOOST_SERIALIZATION_NVP(localRotation);
		
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(parent);
		ar & BOOST_SERIALIZATION_NVP(localPosition);
		setLocalPosition(localPosition);
		ar & BOOST_SERIALIZATION_NVP(localScale);
		setLocalScale(localScale);
		ar & BOOST_SERIALIZATION_NVP(localRotation);
		setLocalRotation(localRotation);

		if (parent != nullptr)
		{
			parent->children.push_back(this);
		}
	}
};
