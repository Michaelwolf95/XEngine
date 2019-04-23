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
	RB_SUBSCRIBE_COLLISION_ENTER(PhysicsTester)
	//RB_SUBSCRIBE_COLLISION_EXIT(PhysicsTester)
	/*rb = XEngine::Rigidbody::GetAttachedRigidbody(this->gameObject);
	if (rb != nullptr)
	{
		rb->OnCollisionEnterEvent.connect(boost::bind(&PhysicsTester::OnCollisionEnter, this, _1));
	}*/
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

void PhysicsTester::OnCollisionEnter(XEngine::CollisionInfo info)
{
	std::cout << "ON COLLISION ENTER!!! - " << info.otherRigidbody->gameObject->name << std::endl;
}

PhysicsTester * PhysicsTester::DeepCopy()
{
	return new PhysicsTester(*this);
}