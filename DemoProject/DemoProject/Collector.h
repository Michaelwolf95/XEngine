#pragma once
#include "Component.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;
#include "Text.h"
#include <shared_ptr.hpp>

class PROJECT_API Collector : public Component
{
public:
	static Registrar<Collector> registrar;
	Collector();
	~Collector();
	void Start() override;
	void Update() override;
	void DrawInspector() override;

	GameObject* textGameObject = nullptr;
	Text* uiText = nullptr;
	//std::shared_ptr<Text> uiText = nullptr;
	int count = 0;
	void CollectItem();

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(textGameObject);
		//ar & BOOST_SERIALIZATION_NVP(uiText);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		ar & BOOST_SERIALIZATION_NVP(textGameObject);
		//ar & BOOST_SERIALIZATION_NVP(uiText);

	}

};
