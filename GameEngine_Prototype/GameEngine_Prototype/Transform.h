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
	void OnDrawGizmos() override;

	Transform* GetParent();
	void SetParent(Transform* _parent);
	std::vector<Transform*> GetChildren();
	unsigned int GetChildCount();

	glm::mat4 getMatrix4x4();
	glm::vec3 getPosition();
	glm::vec3 getLocalPosition();
	void setLocalPosition(glm::vec3 pos);
	void setLocalPosition(float x, float y, float z);
	glm::quat getRotation();
	glm::quat getLocalRotation();
	glm::vec3 getLocalRotationEuler();
	void setLocalRotation(glm::quat rot);
	void setLocalRotationEuler(glm::vec3 rot);
	void setLocalRotationEuler(float x, float y, float z);
	glm::vec3 getScale();
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

	void DrawInspector() override;

	void printTransformMatrix();

	void TestEulerRotation(float x, float y, float z);

// ToDo: Make this private after making appropriate accessors. For right now use GLM API directly.
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
		
		//std::cout << "Saved Parent Address:" << parent << std::endl;
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

		//std::cout << "Loaded Parent Address: " << parent << std::endl;

		if (parent != nullptr)
		{
			parent->children.push_back(this);
			//std::cout << "  Parent GO:" << parent->gameObject->name << std::endl;
		}
	}
};

//ToDo: Split save/load using https://www.boost.org/doc/libs/1_38_0/libs/serialization/doc/serialization.html

//BOOST_CLASS_EXPORT_GUID(Transform, "Transform")