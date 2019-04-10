#pragma once
#include "XEngine.h"
#include "Component.h"

class DLLExport TestComponent : public Component
{
public:
	static Registrar<TestComponent> registrar;
	void Start() override;
	void Update() override;
	TestComponent();
	~TestComponent();

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

