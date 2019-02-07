#pragma once
#include "Component.h"
class TestMoverComponent :
	public Component
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(moveSpeed);
	}
public:
	float moveSpeed = 2.5;
	TestMoverComponent();
	~TestMoverComponent();
	void Start() override;
	void Update() override;
};

