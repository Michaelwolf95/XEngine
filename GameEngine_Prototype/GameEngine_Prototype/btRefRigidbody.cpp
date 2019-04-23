#include "btRefRigidbody.h"
#include "PhysicsManager.h"
#include "Rigidbody.h"
#include <iostream>

namespace XEngine
{
	XEngine::CollisionInfo::CollisionInfo(btCollisionObject * o, btPersistentManifold * contact)
	{
		other = o;
		contactManifold = contact;
	}

	//(const btRigidBodyConstructionInfo& constructionInfo)
	btRefRigidbody::btRefRigidbody(const btRigidBodyConstructionInfo& constructionInfo) :btRigidBody(constructionInfo)
	{

	}

	btRefRigidbody::~btRefRigidbody()
	{
	}

	void btRefRigidbody::HandleCollision(btCollisionObject * other, btPersistentManifold* contactManifold)
	{
		// Use for creating custom CollisionInfo wrapper.
		//bool isBody0 = (this == const_cast<btCollisionObject*>(contactManifold->getBody0()));

		collisionObjsThisFrame.push_back(other);
		if (std::find(currentCollisionObjs.begin(), currentCollisionObjs.end(), other) == currentCollisionObjs.end()) {
			/* v does not contain x */
			currentCollisionObjs.push_back(other);
			EmitCollisionEnter(CollisionInfo(other, contactManifold));
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
				EmitCollisionExit(CollisionInfo(*it, nullptr));
				it = currentCollisionObjs.erase(it);
			}
			else
			{
				EmitCollisionStay(CollisionInfo(*it, nullptr));
				it++;
			}

		}

		collisionObjsThisFrame.clear();
	}

	void btRefRigidbody::EmitCollisionEnter(CollisionInfo info)
	{
		//std::cout << "Collision Enter with " << other << std::endl;
		btRefRigidbody* otherRB = btRefRigidbody::upcast(info.other);
		if (otherRB != 0)
		{
			this->owner->_internal_CollisionEnterCallback(otherRB);
		}
	}

	void btRefRigidbody::EmitCollisionStay(CollisionInfo info)
	{
		//std::cout << "Collision Stay with " << other << std::endl;
		btRefRigidbody* otherRB = btRefRigidbody::upcast(info.other);
		if (otherRB != 0)
		{
			this->owner->_internal_CollisionStayCallback(otherRB);
		}
	}

	void btRefRigidbody::EmitCollisionExit(CollisionInfo info)
	{
		//std::cout << "Collision Exit with " << other << std::endl;
		btRefRigidbody* otherRB = btRefRigidbody::upcast(info.other);
		if (otherRB != 0)
		{
			this->owner->_internal_CollisionExitCallback(otherRB);
		}
	}

}