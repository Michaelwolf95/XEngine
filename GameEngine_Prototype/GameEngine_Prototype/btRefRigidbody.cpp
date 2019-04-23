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

void btRefRigidbody::HandleCollision(btCollisionObject * other, btPersistentManifold* contactManifold)
{
	collisionObjsThisFrame.push_back(other);
	if (std::find(currentCollisionObjs.begin(), currentCollisionObjs.end(), other) == currentCollisionObjs.end()) {
		/* v does not contain x */
		currentCollisionObjs.push_back(other);
		EmitCollisionEnter(other);
	}
	// Collision Objects this frame.
	if (std::find(collisionObjsThisFrame.begin(), collisionObjsThisFrame.end(), other) == collisionObjsThisFrame.end()) {
		/* v does not contain x */
		collisionObjsThisFrame.push_back(other);
	}
}

void btRefRigidbody::UpdateCollisionState()
{
	//std::cout << "Num Collisions: " << currentCollisionObjs.size() << std::endl;
	std::vector<btCollisionObject*>::iterator it = currentCollisionObjs.begin();
	while (it != currentCollisionObjs.end()) 
	{
		if (std::find(collisionObjsThisFrame.begin(), collisionObjsThisFrame.end(), *it) == collisionObjsThisFrame.end())
		{
			// is not in vector
			EmitCollisionExit(*it);
			it = currentCollisionObjs.erase(it);
		}
		else
		{
			EmitCollisionStay(*it);
			it++;
		}

	}

	collisionObjsThisFrame.clear();
}

void btRefRigidbody::EmitCollisionEnter(btCollisionObject * other)
{
	//std::cout << "Collision Enter with " << other << std::endl;
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
		this->owner->_internal_CollisionStayCallback(otherRB);
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
