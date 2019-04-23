#pragma once
#include "XEngineProject.h"
#include "Component.h"

class PROJECT_API ExampleComponent : public Component
{
public:
	static Registrar<ExampleComponent> registrar;
	ExampleComponent();
	~ExampleComponent();
	void Start() override;
	void Update() override;
	void DrawInspector() override;
	ExampleComponent * DeepCopy() override;
private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
};