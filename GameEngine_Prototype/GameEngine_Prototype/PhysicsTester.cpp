#include "PhysicsTester.h"
#include "boost/bind.hpp"

REGISTER_COMPONENT(PhysicsTester, "PhysicsTester")

PhysicsTester::PhysicsTester()
{
}


PhysicsTester::~PhysicsTester()
{
}

void PhysicsTester::Start()
{
	gameObject->FindComponent(typeid(XEngine::Rigidbody), (void**)&rb);
	if (rb != nullptr)
	{
		std::cout << "Subscribe" << std::endl;
		rb->OnCollisionEnterEvent.connect(boost::bind(&PhysicsTester::OnCollisionEnter, this));
	}
}

void PhysicsTester::Update()
{
}

void PhysicsTester::OnEnable()
{
}

void PhysicsTester::OnDisable()
{
}

void PhysicsTester::OnCollisionEnter()
{
	std::cout << "ON COLLISION ENTER!!!" << std::endl;
}
