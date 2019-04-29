#pragma once
#include "Component.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;
#include "Rigidbody.h"

class PROJECT_API PhysicsMover : public Component
{
public:
	static Registrar<PhysicsMover> registrar;
	PhysicsMover();
	~PhysicsMover();
	void Start() override;
	void Update() override;
	void DrawInspector() override;
	void OnEnable() override;
	void OnDisable() override;

	void OnCollisionEnter(XEngine::CollisionInfo info);

	float speed = 5.0f;
	float jumpForce = 10.0f;
	std::shared_ptr<Rigidbody> rigidbody;
private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(speed);
		ar & BOOST_SERIALIZATION_NVP(jumpForce);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(speed);
		ar & BOOST_SERIALIZATION_NVP(jumpForce);
	}

};
