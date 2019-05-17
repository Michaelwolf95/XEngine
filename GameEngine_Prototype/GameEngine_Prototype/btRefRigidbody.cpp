#include "btRefRigidbody.h"
#include "PhysicsManager.h"
#include "Rigidbody.h"
#include <iostream>

namespace XEngine
{

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

		// Find other in set.
		collisionObjsThisFrame.push_back(other);
		std::vector<std::weak_ptr<btCollisionObject>>::iterator isi =
		std::find_if(currentCollisionObjs.begin(), currentCollisionObjs.end(),
			[&other](std::weak_ptr<btCollisionObject> const& item)
		{
			auto sp = item.lock();
			return sp.get() == other;
		}); 
		
		if(isi == currentCollisionObjs.end())
		//if (std::find(currentCollisionObjs.begin(), currentCollisionObjs.end(), other) == currentCollisionObjs.end())
		{
			/* v does not contain x */
			btRefRigidbody* otherRB = btRefRigidbody::upcast(other);
			if (otherRB != 0)
			{
				currentCollisionObjs.push_back(otherRB->owner->body);
				EmitCollisionEnter(other, contactManifold);
			}
			//currentCollisionObjs.push_back(other);
			//EmitCollisionEnter(other, contactManifold);
		}

		// Collision Objects this frame.
		if (std::find(collisionObjsThisFrame.begin(), collisionObjsThisFrame.end(), other) == collisionObjsThisFrame.end()) {
			/* v does not contain x */
			collisionObjsThisFrame.push_back(other);
		}
		//std::cout << "Finished handling Collision.\n";
	}

	void btRefRigidbody::UpdateCollisionState()
	{
		//std::cout << "Updating Collision State.\n";
		//std::cout << "Num Collisions: " << currentCollisionObjs.size() << std::endl;
		std::vector<std::weak_ptr<btCollisionObject>>::iterator it = currentCollisionObjs.begin();
		while (it != currentCollisionObjs.end())
		{
			if ((*it).expired())
			{
				it++;
				continue;
			}
			auto sp = (*it).lock();
			if (std::find(collisionObjsThisFrame.begin(), collisionObjsThisFrame.end(), sp.get()) == collisionObjsThisFrame.end())
			{
				// is not in vector
				if (!(*it).expired())
				{
					EmitCollisionExit(sp.get(), nullptr);
				}
				it = currentCollisionObjs.erase(it);
			}
			else
			{
				EmitCollisionStay(sp.get(), nullptr);
				//EmitCollisionStay(*it, nullptr);
				it++;
			}

		}

		collisionObjsThisFrame.clear();
	}

	void btRefRigidbody::EmitCollisionEnter(btCollisionObject* other, btPersistentManifold* contactManifold)
	{
		//std::cout << "Collision Enter with " << other << std::endl;
		btRefRigidbody* otherRB = btRefRigidbody::upcast(other);
		if (otherRB != 0)
		{
			CollisionInfo info = GenerateCollisionInfo(otherRB, contactManifold);
			this->owner->_internal_CollisionEnterCallback(info);
		}
	}

	void btRefRigidbody::EmitCollisionStay(btCollisionObject* other, btPersistentManifold* contactManifold)
	{
		//std::cout << "Collision Stay with " << other << std::endl;
		btRefRigidbody* otherRB = btRefRigidbody::upcast(other);
		if (otherRB != 0)
		{
			CollisionInfo info = GenerateCollisionInfo(otherRB, contactManifold);
			this->owner->_internal_CollisionStayCallback(info);
		}
	}

	void btRefRigidbody::EmitCollisionExit(btCollisionObject* other, btPersistentManifold* contactManifold)
	{
		//std::cout << "Collision Exit with " << other << std::endl;
		btRefRigidbody* otherRB = btRefRigidbody::upcast(other);
		if (otherRB != 0)
		{
			CollisionInfo info = GenerateCollisionInfo(otherRB, contactManifold);
			this->owner->_internal_CollisionExitCallback(info);
		}
	}

	CollisionInfo btRefRigidbody::GenerateCollisionInfo(btRefRigidbody * otherRB, btPersistentManifold * contactManifold)
	{
		CollisionInfo info(otherRB->owner);
		// TODO: Get Collider object for collision.

		// TODO: Use this to modify CollisionInfo for appropriate relative space.
		//bool isBody0 = (this == const_cast<btCollisionObject*>(contactManifold->getBody0()));
		//if (isBody0)
		if(contactManifold != nullptr)
		{
			if (contactManifold->getNumContacts() > 0)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(0);
				if (pt.getDistance() < 0.f)
				{
					const btVector3& ptPoint = pt.getPositionWorldOnB();
					//const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normal = pt.m_normalWorldOnB; // There's no "normalWorldOnA" ??
					//TODO: Rotate these points to game world, in the same way as sync transform?
					info.contactPoint = glm::vec3(ptPoint[0], ptPoint[1], ptPoint[2]);
					info.contactNormal = glm::vec3(normal[0], normal[1], normal[2]);

				}
			}
		}
		return info;
	}

}