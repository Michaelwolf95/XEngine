#pragma once
#include "Component.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
class Rigidbody : public Component
{
public:
	static Registrar<Rigidbody> registrar;

	btCollisionShape* colShape;
	btDefaultMotionState* motionState;
	//btRigidBody::btRigidBodyConstructionInfo rbInfo;
	btRigidBody* body;
	float mass = 1.0f;
	//bool isDynamic = true;

	Rigidbody();
	~Rigidbody();
	void Start() override;
	void Update() override;
	void FixedUpdate() override;

private:
	btTransform physTransformModel;
	btScalar _convertTransformArray[16];
	bool isInitialized = false;
	void Init();
	void SyncTransformWithPhysicsModel();
	void SyncPhysicsModelWithTransform();
	glm::mat4 btScalar2glmMat4(btScalar* matrix);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);

	}
};

