#include "btRefRigidbody.h"
#include "Rigidbody.h"
#include <iostream>

//(const btRigidBodyConstructionInfo& constructionInfo)
btRefRigidbody::btRefRigidbody(const btRigidBodyConstructionInfo& constructionInfo) :btRigidBody(constructionInfo)
{

}

btRefRigidbody::~btRefRigidbody()
{
}

void btRefRigidbody::HandleCollision(btCollisionObject * other)
{
	if (std::find(currentCollisionObjs.begin(), currentCollisionObjs.end(), other) != currentCollisionObjs.end()) {
		/* v contains x */
		//EmitCollisionStay(other);
	}
	else {
		/* v does not contain x */
		currentCollisionObjs.push_back(other);
		EmitCollisionEnter(other);
	}
}

void btRefRigidbody::UpdateCollisionState()
{
	std::vector<btCollisionObject*>::iterator it = currentCollisionObjs.begin();
	while (it != currentCollisionObjs.end()) {

		if (this->checkCollideWith(*it))
		{
			EmitCollisionStay(*it);
			it++;
		}
		else
		{
			EmitCollisionExit(*it);
			it = currentCollisionObjs.erase(it);
		}
	}
}

void btRefRigidbody::EmitCollisionEnter(btCollisionObject * other)
{
	std::cout << "Collision Enter with " << other << std::endl;
	btRefRigidbody* otherRB = btRefRigidbody::upcast(other);
	if (otherRB != 0)
	{
		this->owner->_internal_CollisionEnterCallback(otherRB);
	}
}

void btRefRigidbody::EmitCollisionStay(btCollisionObject * other)
{
	//std::cout << "Collision Stay with " << other << std::endl;
	btRefRigidbody* otherRB = btRefRigidbody::upcast(other);
	if (otherRB != 0)
	{
		
	}
}

void btRefRigidbody::EmitCollisionExit(btCollisionObject * other)
{
	//std::cout << "Collision Exit with " << other << std::endl;
	btRefRigidbody* otherRB = btRefRigidbody::upcast(other);
	if (otherRB != 0)
	{
		this->owner->_internal_CollisionExitCallback(otherRB);
	}
}
