#pragma once
#include "XEngineProject.h"
#include "Component.h"
#include "GameObject.h"

class PROJECT_API ExampleComponent : public Component
{
public:
	static Registrar<ExampleComponent> registrar;
	//GameObject** GameObjPtr = &this->gameObject;
	GameObject* target = nullptr;
	ExampleComponent();
	~ExampleComponent();
	void Start() override;
	void Update() override;
	void DrawInspector() override;
private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(target);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(target);
	}
};