#include "PhysicsManager.h"
#include "Time.h"
#include "SceneManager.h"
#include "Rigidbody.h"

PhysicsManager::PhysicsManager() {}
PhysicsManager::~PhysicsManager()
{
	CleanUp();
}

PhysicsManager * PhysicsManager::CreateManager()
{
	PhysicsManager* instance = &PhysicsManager::getInstance();
	instance->Init();
	return instance;
}

int PhysicsManager::Init()
{
	///-----initialization_start----- (Comments from Demo)

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

	
	// Callback for physics ticks.
	dynamicsWorld->setInternalTickCallback(&PhysicsManager::PhysicsTickCallback);

	isInitialized = true;
	return 0;
}

void PhysicsManager::PhysicsUpdate()
{
	dynamicsWorld->stepSimulation(Time::deltaTime, 10, fixedUpdateTime);
}

void PhysicsManager::PhysicsTick()
{
	SceneManager::getInstance().FixedUpdateActiveScene();
}

void PhysicsManager::PhysicsTickCallback(btDynamicsWorld * world, btScalar timeStep)
{
	PhysicsManager::getInstance().PhysicsTick();
}

// Cleanup and unload
void PhysicsManager::CleanUp()
{
	if (!isInitialized) return;

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;
	dynamicsWorld = nullptr;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}

void PhysicsManager::AddCollisionShape(btCollisionShape* colShape)
{
	collisionShapes.push_back(colShape);
	//auto n = std::find(collisionShapes..begin(), collisionShapes.end(), colShape);
	//if (n == collisionShapes.end())
	//{
	//	collisionShapes.push_back(colShape);
	//}
}
void PhysicsManager::RemoveCollisionShape(btCollisionShape* colShape)
{
	collisionShapes.remove(colShape);
	//auto n = std::find(collisionShapes.begin(), collisionShapes.end(), colShape);
	//if (n != collisionShapes.end())
	//{
	//	std::swap(*n, collisionShapes.back());
	//	collisionShapes.pop_back();
	//}
}