#pragma once
#include "Component.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;
#include "Rigidbody.h"

class PROJECT_API FirstPersonController : public Component
{
public:
	static Registrar<FirstPersonController> registrar;
	FirstPersonController();
	~FirstPersonController();
	void Start() override;
	void Update() override;
	void DrawInspector() override;

	void rotateCamera();
	float moveSpeed = 10.0f;
	float xRotSpeed = 5.0f;
	float yRotSpeed = 5.0f;
	float zRotSpeed = 2.5f;

	GameObject* cameraObject = nullptr;

	Rigidbody* rigidbody = nullptr;

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(moveSpeed);
		ar & BOOST_SERIALIZATION_NVP(xRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(yRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(zRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(cameraObject);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(moveSpeed);
		ar & BOOST_SERIALIZATION_NVP(xRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(yRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(zRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(cameraObject);
	}

};
