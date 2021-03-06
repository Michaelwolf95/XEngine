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
#include <BulletPhysics/LinearMath/btAlignedAllocator.h>
#include "BulletPhysics/BulletCollision/CollisionShapes/btEmptyShape.h"
#include "Serialization.h"

#include "Input.h"

REGISTER_COMPONENT(XEngine::Rigidbody, "Rigidbody")
namespace XEngine
{
	Rigidbody::Rigidbody() 
	{
		if (isKinematic)
			mass = 0.0f;
	}
	Rigidbody::~Rigidbody()
	{
		//std::cout << "Deconstructing Rigidbody..." << std::endl;
		if (isInitialized)
		{
			//PhysicsManager::getInstance().RemoveCollisionShape(colShape);

			// Remove from physics manager.
			if (body != nullptr && PhysicsManager::getInstance().dynamicsWorld != nullptr)
			{
				PhysicsManager::getInstance().dynamicsWorld->removeRigidBody(body.get());

				//delete body;
				//body = nullptr;
			}
			//delete boxColliderHalfExtents;
			//boxColliderHalfExtents = nullptr;
			delete physTransformModel;
			physTransformModel = nullptr;
			//delete colShape;
			//colShape = nullptr;
			delete motionState;
			motionState = nullptr;

			//std::cout << "Finished Deconstructing Rigidbody" << std::endl;
		}
	}
	void Rigidbody::Init()
	{
		if (isInitialized)
			return;

		//std::cout << "Initializing Rigidbody..." << std::endl;

		// Empty Collision Shape
		// Create "Empty shape" when no collision shape exists.
		// Needed for physics to simulate.
		//https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=147
		btCollisionShape* colShape = new btEmptyShape();
		PhysicsManager::getInstance().AddCollisionShape(colShape);

		// Create Dynamic Objects
		physTransformModel = new btTransform();
		physTransformModel->setIdentity();
		SyncPhysicsModelWithTransform();

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		motionState = new btDefaultMotionState(*physTransformModel);
		//btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape);
		body = std::shared_ptr<btRefRigidbody>(new btRefRigidbody(rbInfo));
		body->owner = this; // L337 HACKS
		// Wait.. there was just body->setUserPointer((void*)this);

		if (isKinematic)
		{
			// Kinematic static
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			body->setActivationState(DISABLE_DEACTIVATION);
			body->setLinearFactor(btVector3(0, 0, 0));
		}

		physTransformModel->setFromOpenGLMatrix(glm::value_ptr(this->gameObject->transform->getModelRef()));
		body->setWorldTransform(*physTransformModel);

		PhysicsManager::getInstance().dynamicsWorld->addRigidBody(body.get());

		body->setUserPointer((void*)this);

		//body->serialize() // Might be useful?
		isInitialized = true;


		// Set for Trigger.
		setIsTrigger(isTrigger);
	}
	void Rigidbody::Start()
	{
		Init();
	}
	void Rigidbody::Update()
	{
		SyncPhysicsModelWithTransform();
		//SyncTransformWithPhysicsModel();
	}

	void Rigidbody::FixedUpdate()
	{
		this->body->UpdateCollisionState();

		//https://stackoverflow.com/questions/22002077/getting-bullet-physics-transform-matrix-for-opengl
		if (body && body->getMotionState())
		{
			SyncTransformWithPhysicsModel();
		}
	}

	void Rigidbody::OnEnable()
	{
		Init();
	}

	void Rigidbody::SyncTransformWithPhysicsModel()
	{
		btVector3 pos = body->getWorldTransform().getOrigin();
		btQuaternion orn = body->getWorldTransform().getRotation();
		glm::vec3 newPos(pos[0], pos[1], pos[2]);
		glm::quat newRot(orn[3], orn[0], orn[1], orn[2]);

		this->gameObject->transform->setPosition(newPos);
		this->gameObject->transform->setRotation(newRot); // TODO: MAKE WORLD SPACE

		//physTransformModel = body->getWorldTransform();
		//physTransformModel->getOpenGLMatrix();
	}
	void Rigidbody::SyncPhysicsModelWithTransform()
	{
		//physTransformModel.setFromOpenGLMatrix(glm::value_ptr(this->gameObject->transform->getModelRef()));
		//body->setWorldTransform(physTransformModel);

		// Note: We can set collision shape local scaling.

		// OLD METHOD.
		glm::vec3 pos = this->gameObject->transform->getPosition();
		glm::quat rot = this->gameObject->transform->getRotation();
		physTransformModel->setOrigin(btVector3(pos.x, pos.y, pos.z));
		physTransformModel->setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

		//physTransformModel->setFromOpenGLMatrix(glm::value_ptr(this->gameObject->transform->getModelRef()));
		if (body != nullptr)
		{
			body->setWorldTransform(*physTransformModel);
		}
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

	void Rigidbody::_internal_CollisionEnterCallback(CollisionInfo& info)
	{
		//std::cout << "CollisionEnter " << gameObject->name << " with " << other->owner->gameObject->name << std::endl;
		OnCollisionEnterEvent(info);
		//std::cout << "CollisionEnter " << gameObject->name << " with " << info.otherRigidbody->gameObject->name << std::endl;
	}

	void Rigidbody::_internal_CollisionStayCallback(CollisionInfo& info)
	{
		//std::cout << "CollisionEnter " << gameObject->name << " with " << other->owner->gameObject->name << std::endl;
		OnCollisionStayEvent(info);
	}

	void Rigidbody::_internal_CollisionExitCallback(CollisionInfo& info)
	{
		//std::cout << "CollisionExit " << gameObject->name << " with " << other->owner->gameObject->name << std::endl;
		OnCollisionExitEvent(info);
	}

	void Rigidbody::DrawInspector()
	{
		ImGui::Checkbox("IsKinematic", &isKinematic);
		if (isKinematic == false)
		{
			ImGui::InputFloat("Mass", &mass);
		}

		bool trigger = isTrigger;
		ImGui::Checkbox("IsTrigger", &trigger);
		if (trigger != isTrigger)
		{
			setIsTrigger(trigger);
		}
	}


	void Rigidbody::AddForce(glm::vec3 force)
	{
		this->body->activate(true);
		this->body->applyCentralForce(btVector3(-force.x, force.y, -force.z));
	}
	void Rigidbody::AddImpulseForce(glm::vec3 force)
	{
		this->body->activate(true);
		this->body->applyCentralImpulse(btVector3(-force.x, force.y, -force.z));
	}
	void Rigidbody::SetVelocity(glm::vec3 velocity)
	{
		this->body->activate(true);
		this->body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
	}
	Rigidbody * Rigidbody::GetAttachedRigidbody(GameObject* go)
	{
		Rigidbody* rb = nullptr;
		while (go != nullptr)
		{
			go->FindComponent(typeid(XEngine::Rigidbody), (void**)&rb);
			if (rb == nullptr)
			{
				go = go->GetParent();
			}
			else break;
		}
		return rb;
	}

	// https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=5194
	void Rigidbody::AttachCollider(Collider * col)
	{
		// Check if collider is already attached. (Changing to vector later)
		if (this->attachedCollider == col)
			return;

		// 1) Remove rigidbody from world.
		PhysicsManager::getInstance().dynamicsWorld->removeRigidBody(body.get());

		// 2) Assign new shape.

		auto shape = col->GetColShape();
		this->body->setCollisionShape(shape);

		// 3) Recompute the inertia tensor for dynamic objects (mass>0) using newShape->calcLocalInertia(...) and use body->setMassProps
		btVector3 localInertia(0, 0, 0);
		if (isDynamic())
		{
			shape->calculateLocalInertia(this->mass, localInertia);
			body->setMassProps(this->mass, localInertia);
		}
		
		// 4) add the body to the world
		PhysicsManager::getInstance().dynamicsWorld->addRigidBody(body.get());

		this->attachedCollider = col;
	}
	void Rigidbody::DetachCollider(Collider * col)
	{

	}

	bool Rigidbody::isDynamic()
	{
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		if (isKinematic)
			mass = 0.0f;
		return (mass != 0.f);
	}
	bool Rigidbody::getIsInitialized()
	{
		return isInitialized;
	}
	void Rigidbody::setIsTrigger(bool _isTrigger)
	{
		isTrigger = _isTrigger;
		if (isInitialized == false)
			return;
		if (isTrigger)
		{
			// Set
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
		else
		{
			// Unset
			body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
	}
	CollisionInfo::CollisionInfo(Rigidbody * _other, Collider * _otherCollider, glm::vec3 _contactPoint, glm::vec3 _contactNormal)
	{
		otherRigidbody = _other;
		otherCollider = _otherCollider;
		contactPoint = _contactPoint;
		contactNormal = _contactNormal;
	}
}