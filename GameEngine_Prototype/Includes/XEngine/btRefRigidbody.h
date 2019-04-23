#pragma once
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include "GameObject.h"
#include <vector>
//include "Rigidbody.h"
namespace XEngine
{
	class Rigidbody;
	struct CollisionInfo;
	//struct CollisionInfo
	//{
	//public:
	//	btCollisionObject* other;
	//	btPersistentManifold* contactManifold;
	//	CollisionInfo(btCollisionObject* o, btPersistentManifold* contact);
	//};
	class btRefRigidbody : public btRigidBody
	{
	public:
		btRefRigidbody(const btRigidBodyConstructionInfo& constructionInfo);
		~btRefRigidbody();

		XEngine::Rigidbody* owner; // XEngine Rigidbody component

		void HandleCollision(btCollisionObject* other, btPersistentManifold* contactManifold);
		void UpdateCollisionState();

		static btRefRigidbody* upcast(btCollisionObject* colObj)
		{
			if ((dynamic_cast<btRefRigidbody*>(colObj) != NULL))
				return (btRefRigidbody*)colObj;
			return 0;
		}
	private:
		//friend class XEngine::Rigidbody;
		std::vector<btCollisionObject*> collisionObjsThisFrame;
		std::vector<btCollisionObject*> currentCollisionObjs;

		void EmitCollisionEnter(btCollisionObject* other, btPersistentManifold* contactManifold);
		void EmitCollisionStay(btCollisionObject* other, btPersistentManifold* contactManifold);
		void EmitCollisionExit(btCollisionObject* other, btPersistentManifold* contactManifold);

		CollisionInfo GenerateCollisionInfo(btRefRigidbody* otherRB, btPersistentManifold* contactManifold);
	};

}