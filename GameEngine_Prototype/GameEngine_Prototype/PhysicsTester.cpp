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
	//RB_SUBSCRIBE_COLLISION_ENTER(PhysicsTester)

	rb = XEngine::Rigidbody::GetAttachedRigidbody(this->gameObject);
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
