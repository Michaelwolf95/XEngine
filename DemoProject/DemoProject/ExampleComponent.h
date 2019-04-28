#pragma once
#include "XEngineProject.h"
#include "Component.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include <shared_ptr.hpp>
using namespace XEngine;

class PROJECT_API ExampleComponent : public Component
{
public:
	static Registrar<ExampleComponent> registrar;
	ExampleComponent();
	~ExampleComponent();
	void Start() override;
	void Update() override;
	void DrawInspector() override;

	// Note: We may want to use shared_ptrs for this sort of thing.
	GameObject* target = nullptr;
	Rigidbody* rigidbody = nullptr;

	std::shared_ptr<Rigidbody> sRigidbody;

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(target);
		ar & BOOST_SERIALIZATION_NVP(rigidbody);
		ar & BOOST_SERIALIZATION_NVP(sRigidbody);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(target);
		ar & BOOST_SERIALIZATION_NVP(rigidbody);
		ar & BOOST_SERIALIZATION_NVP(sRigidbody);
	}
};