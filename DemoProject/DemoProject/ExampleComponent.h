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

	// Other types of component pointers.
	std::shared_ptr<Rigidbody> sRigidbody;
	std::weak_ptr<Rigidbody> wRigidbody;

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(target);
		ar & BOOST_SERIALIZATION_NVP(rigidbody);

		//if (sRigidbody.get() != nullptr && (sRigidbody->gameObject != nullptr))
		//{
		//	sRigidbody.reset();
		//}
		ar & BOOST_SERIALIZATION_NVP(sRigidbody);
		ar & BOOST_SERIALIZATION_NVP(wRigidbody);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(target);
		ar & BOOST_SERIALIZATION_NVP(rigidbody);
		ar & BOOST_SERIALIZATION_NVP(sRigidbody);
		ar & BOOST_SERIALIZATION_NVP(wRigidbody);
	}
};