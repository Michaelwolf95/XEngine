#pragma once
#include "Component.h"
class ENGINE_API TestMoverComponent : public Component
{
public:
	static Registrar<TestMoverComponent> registrar;
	float moveSpeed = 2.5;
	TestMoverComponent();
	~TestMoverComponent();
	void Start() override;
	void Update() override;
	TestMoverComponent* DeepCopy() override;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(moveSpeed);
	}
};

