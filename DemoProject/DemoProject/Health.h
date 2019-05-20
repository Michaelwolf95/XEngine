#pragma once
#include "Component.h"
#include "HealthManager.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;

class PROJECT_API Health : public Component
{
public:
	static Registrar<Health> registrar;
	Health();
	~Health();
	void Start() override;
	void Update() override;
	void DrawInspector() override;
	Text* healthText;
	HealthManager* health;
	GameObject* target;

	void UpdateHPText(float damage);
	void UpdateHPText_Death();

private:
	void connectToComponents();
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(target);


	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(target);
	}

};
