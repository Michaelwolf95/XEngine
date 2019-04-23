#pragma once
#include "Component.h"
#include "Rigidbody.h"
//using namespace XEngine;
class ENGINE_API PhysicsTester : public Component
{
public:
	static Registrar<PhysicsTester> registrar;
	PhysicsTester();
	~PhysicsTester();

	XEngine::Rigidbody* rb = nullptr;

	void Start() override;
	void Update() override;
	void OnEnable() override;
	void OnDisable() override;

	void OnCollisionEnter(XEngine::CollisionInfo);
	PhysicsTester * DeepCopy() override;
private:
	friend class ::boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive &ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
	template<class Archive>
	void load(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
};

