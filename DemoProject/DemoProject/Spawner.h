#pragma once
#include "Component.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;

class PROJECT_API Spawner : public Component
{
public:
	static Registrar<Spawner> registrar;
	Spawner();
	~Spawner();
	void Start() override;
	void Update() override;
	void DrawInspector() override;

	std::string prefabPath;

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(prefabPath);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(prefabPath);
	}

};
