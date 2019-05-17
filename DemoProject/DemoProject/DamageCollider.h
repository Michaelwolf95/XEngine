#pragma once
#include "Component.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;
#include "Rigidbody.h"
#include <vector>

class PROJECT_API DamageCollider : public Component
{
public:
	static Registrar<DamageCollider> registrar;
	DamageCollider();
	~DamageCollider();
	void Start() override;
	void Update() override;
	void DrawInspector() override;

	int damageValue = 10;
	bool destroySelfAfterCollision = false;

	bool filterByTargetName = false;
	std::vector<std::string> targetNames;


	void OnCollisionEnter(XEngine::CollisionInfo info);
private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(damageValue);
		ar & BOOST_SERIALIZATION_NVP(destroySelfAfterCollision);
		ar & BOOST_SERIALIZATION_NVP(filterByTargetName);
		ar & BOOST_SERIALIZATION_NVP(targetNames);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(damageValue);
		ar & BOOST_SERIALIZATION_NVP(destroySelfAfterCollision);
		ar & BOOST_SERIALIZATION_NVP(filterByTargetName);
		ar & BOOST_SERIALIZATION_NVP(targetNames);
	}

};
