#pragma once
#include "Singleton.h"
#include <iostream>
//#include <nlohmann/json.hpp>

#include <glm/glm.hpp>
#include <BulletPhysics/btBulletDynamicsCommon.h>

class PhysicsManager : public Singleton<PhysicsManager>
{
	friend class Singleton<PhysicsManager>;
private:
	glm::vec3 gravity = glm::vec3(0.0f, -10.0f, 0.0f);

public:
	bool isInitialized = false;
	float fixedUpdateTime = 0.02f;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	//btAlignedObjectArray<btCollisionShape*> collisionShapes;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	void AddCollisionShape(btCollisionShape* colShape);
	void RemoveCollisionShape(btCollisionShape* colShape);

protected:
	PhysicsManager();
	~PhysicsManager();
public:
	// Create static instance and configure manager
	static PhysicsManager* CreateManager();

	// Init instance
	int Init();
	// Check for physics tick, and other per-frame updates.
	void PhysicsUpdate();
	// Actual Physics calculations.
	void PhysicsTick();

	static void PhysicsTickCallback(btDynamicsWorld* world, btScalar timeStep);
private:
	
	void CleanUp();

};

