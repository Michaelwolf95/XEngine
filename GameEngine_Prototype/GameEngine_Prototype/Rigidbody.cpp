#include "Rigidbody.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Time.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

REGISTER_COMPONENT(Rigidbody, "Rigidbody")

Rigidbody::Rigidbody()
{
	
}

Rigidbody::~Rigidbody()
{
	// TODO: Remove from physics manager.
}
void Rigidbody::Init()
{
	colShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	// TODO: Extract to PhysicsManager method.
	PhysicsManager::getInstance().collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	//btScalar mass(1.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	glm::vec3 pos = this->gameObject->transform->getPosition();
	startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
	//rbInfo = btRigidBody::btRigidBodyConstructionInfo(mass, myMotionState, colShape, localInertia);
	body = new btRigidBody(rbInfo);

	PhysicsManager::getInstance().dynamicsWorld->addRigidBody(body);

	//body->serialize() // Might be useful?
	//body.
}

void Rigidbody::Start() 
{
	Init();
}
void Rigidbody::Update() 
{
	//body->getMotionState()->getWorldTransform(physTransformModel);
	//PhysicsManager::getInstance().dynamicsWorld->debugDrawObject(physTransformModel,);
	if (body && body->getMotionState())
	{
		//body->setWorldTransform();
		/*
		// Interpolation
		bool isDynamic = (mass != 0.f);`
		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
		{
			colShape->calculateLocalInertia(mass, localInertia);
			
			glm::vec3 deltaPos(localInertia.getX(), localInertia.getY(), localInertia.getZ());
			deltaPos *= Time::deltaTime;
			glm::vec3 newPos = this->gameObject->transform->getPosition() + deltaPos;
			this->gameObject->transform->setPosition(newPos);
			//SyncPhysicsModel();
		}
		*/
	}
}

void Rigidbody::FixedUpdate()
{
	//std::cout << "Tick." << std::endl;
	//https://stackoverflow.com/questions/22002077/getting-bullet-physics-transform-matrix-for-opengl
	if (body && body->getMotionState())
	{
		SyncPhysicsModel();
	}
}

void Rigidbody::SyncPhysicsModel()
{
	body->getMotionState()->getWorldTransform(physTransformModel);
	physTransformModel.getOpenGLMatrix(_convertTransformArray);
	glm::mat4 model = btScalar2glmMat4(_convertTransformArray);
	//glm::rotate()
	//model = glm::tra
	this->gameObject->transform->setLocalMatrix4x4(model);
}

// Temp utility - move elsewhere later
glm::mat4 Rigidbody::btScalar2glmMat4(btScalar* matrix)
{
	return glm::mat4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
		//matrix[15], matrix[14], matrix[13], matrix[12],
		//matrix[11], matrix[10], matrix[9], matrix[8],
		//matrix[7], matrix[6], matrix[5], matrix[4],
		//matrix[3], matrix[2], matrix[1], matrix[0]);
}

