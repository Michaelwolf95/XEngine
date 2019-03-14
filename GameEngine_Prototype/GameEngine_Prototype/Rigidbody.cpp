#include "Rigidbody.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Time.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "RenderManager.h"
//#include <BulletPhysics/Bullet3Common/b3TransformUtil.h>

REGISTER_COMPONENT(Rigidbody, "Rigidbody")

Rigidbody::Rigidbody() {}
Rigidbody::~Rigidbody()
{
	// TODO: Remove from physics manager.
	if (isInitialized)
	{
		PhysicsManager::getInstance().RemoveCollisionShape(colShape);

		if (body != nullptr && PhysicsManager::getInstance().dynamicsWorld != nullptr)
		{
			//if(PhysicsManager::getInstance())
			PhysicsManager::getInstance().dynamicsWorld->removeRigidBody(body);

			delete body;
			body = nullptr;
		}
	}
}
void Rigidbody::Init()
{
	//btVector3 x = *((btVector3*)&this->gameObject->transform->localScale);

	boxColliderHalfExtents = *(btVector3*)&this->gameObject->transform->getScale() * 0.5f;
	colShape = new btBoxShape(boxColliderHalfExtents);// , btVector3(0.5f, 0.5f, 0.5f));
	PhysicsManager::getInstance().AddCollisionShape(colShape);

	/// Create Dynamic Objects
	//btTransform startTransform;
	physTransformModel.setIdentity();
	SyncPhysicsModelWithTransform();

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	if (isKinematic)
		mass = 0.0f;
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		colShape->calculateLocalInertia(mass, localInertia);
	}

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	motionState = new btDefaultMotionState(physTransformModel);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
	//rbInfo = btRigidBody::btRigidBodyConstructionInfo(mass, myMotionState, colShape, localInertia);
	body = new btRigidBody(rbInfo);

	if (isKinematic)
	{
		// Kinematic static
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}

	physTransformModel.setFromOpenGLMatrix(glm::value_ptr(this->gameObject->transform->getModelRef()));
	body->setWorldTransform(physTransformModel);

	PhysicsManager::getInstance().dynamicsWorld->addRigidBody(body);

	//body->serialize() // Might be useful?
	//body.
	isInitialized = true;
}

void Rigidbody::Start() 
{
	Init();
}
void Rigidbody::Update() 
{
	boxColliderHalfExtents = *(btVector3*)&this->gameObject->transform->getScale() * 0.5f;
	SyncPhysicsModelWithTransform();
	//SyncTransformWithPhysicsModel();
}

void Rigidbody::FixedUpdate()
{
	//std::cout << "Tick." << std::endl;
	//https://stackoverflow.com/questions/22002077/getting-bullet-physics-transform-matrix-for-opengl
	if (body && body->getMotionState())
	{
		SyncTransformWithPhysicsModel();

		//SyncPhysicsModel();
		//physTransformModel.getOpenGLMatrix(glm::value_ptr(this->gameObject->transform->getModelRef()));
		//physTransformModel.setFromOpenGLMatrix(glm::value_ptr(this->gameObject->transform->getModelRef()));
		//body->setWorldTransform(physTransformModel);
	}
}

void Rigidbody::SyncTransformWithPhysicsModel()
{
	btVector3 pos = body->getWorldTransform().getOrigin();
	btQuaternion orn = body->getWorldTransform().getRotation();
	glm::vec3 newPos(pos[0], pos[1], pos[2]);
	glm::quat newRot(orn[3], orn[0], orn[1], orn[2]);

	this->gameObject->transform->setPosition(newPos);
	this->gameObject->transform->setRotation(newRot); // TODO: MAKE WORLD SPACE
}
void Rigidbody::SyncPhysicsModelWithTransform()
{
	//physTransformModel.setFromOpenGLMatrix(glm::value_ptr(this->gameObject->transform->getModelRef()));
	//body->setWorldTransform(physTransformModel);

	glm::vec3 pos = this->gameObject->transform->getPosition();
	glm::quat rot = this->gameObject->transform->getRotation();
	physTransformModel.setOrigin(btVector3(pos.x, pos.y, pos.z));
	physTransformModel.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
}

// Temp utility - move elsewhere later
glm::mat4 Rigidbody::btScalar2glmMat4(btScalar* matrix)
{
	return glm::mat4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}

void Rigidbody::DrawInspector()
{
	ImGui::Checkbox("IsKinematic", &isKinematic);
	if (isKinematic == false)
	{
		ImGui::InputFloat("Mass", &mass);
	}
}
void Rigidbody::OnDrawGizmosSelected()
{
	glm::vec3 pos = this->gameObject->transform->getPosition();
	//glm::vec3 halfExt = *(glm::vec3*)&colShape->getHalfExtentsWithMargin();
	glm::vec3 halfExt = this->gameObject->transform->getScale() * 0.5f;
	//colShape->getHalfExtentsWithMargin()
	//RenderManager::DrawWorldSpaceBox(pos, halfExt, glm::vec4(0, 0, 1, 1), 2);

	glm::vec3 right = glm::normalize(this->gameObject->transform->getRightDirection());
	glm::vec3 up = glm::normalize(this->gameObject->transform->getUpDirection());
	glm::vec3 forward = glm::normalize(this->gameObject->transform->getForwardDirection());
	glm::vec3 corners[8] = {
		pos + (forward * halfExt.z) + (up * halfExt.y) + (right * halfExt.x), // 0
		pos + (forward * halfExt.z) + (up * halfExt.y) - (right * halfExt.x), // 1
		pos + (forward * halfExt.z) - (up * halfExt.y) + (right * halfExt.x), // 2
		pos + (forward * halfExt.z) - (up * halfExt.y) - (right * halfExt.x), // 3
		pos - (forward * halfExt.z) + (up * halfExt.y) + (right * halfExt.x), // 4
		pos - (forward * halfExt.z) + (up * halfExt.y) - (right * halfExt.x), // 5
		pos - (forward * halfExt.z) - (up * halfExt.y) + (right * halfExt.x), // 6
		pos - (forward * halfExt.z) - (up * halfExt.y) - (right * halfExt.x), // 7
	};
	glm::vec4 color(0, 1, 0, 0.5f);
	int size = 1;
	//RenderManager::DrawWorldSpacePoint(pos + right * L, glm::vec4(1, 0, 0, 1), 5); // Center
	RenderManager::DrawWorldSpaceLine(corners[0], corners[1], color, size);
	RenderManager::DrawWorldSpaceLine(corners[0], corners[2], color, size);
	RenderManager::DrawWorldSpaceLine(corners[3], corners[1], color, size);
	RenderManager::DrawWorldSpaceLine(corners[3], corners[2], color, size);

	RenderManager::DrawWorldSpaceLine(corners[0], corners[4], color, size);
	RenderManager::DrawWorldSpaceLine(corners[1], corners[5], color, size);
	RenderManager::DrawWorldSpaceLine(corners[2], corners[6], color, size);
	RenderManager::DrawWorldSpaceLine(corners[3], corners[7], color, size);

	RenderManager::DrawWorldSpaceLine(corners[4], corners[5], color, size);
	RenderManager::DrawWorldSpaceLine(corners[4], corners[6], color, size);
	RenderManager::DrawWorldSpaceLine(corners[7], corners[5], color, size);
	RenderManager::DrawWorldSpaceLine(corners[7], corners[6], color, size);
}