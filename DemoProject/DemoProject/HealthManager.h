#pragma once
#include "Component.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;

class PROJECT_API HealthManager : public Component
{
public:
	static Registrar<HealthManager> registrar;
	HealthManager();
	~HealthManager();
	void Start() override;
	void Update() override;
	void DrawInspector() override;
	int currentHealth, maxHealth;
	void applyDamage(int);

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		//ar & BOOST_SERIALIZATION_NVP(currentHealth);
		//ar & BOOST_SERIALIZATION_NVP(maxHealth);

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		//ar & BOOST_SERIALIZATION_NVP(currentHealth);
		//ar & BOOST_SERIALIZATION_NVP(maxHealth);

	}

};
