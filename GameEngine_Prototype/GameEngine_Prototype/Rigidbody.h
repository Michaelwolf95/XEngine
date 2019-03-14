#pragma once
#include "Component.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
class Rigidbody : public Component
{
public:
	static Registrar<Rigidbody> registrar;

	btBoxShape* colShape;
	btDefaultMotionState* motionState;
	//btRigidBody::btRigidBodyConstructionInfo rbInfo;
	btRigidBody* body;
	float mass = 1.0f;
	bool isKinematic = false;

	btVector3 boxColliderHalfExtents;

	Rigidbody();
	~Rigidbody();
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void DrawInspector() override;
	void OnDrawGizmosSelected() override;
private:
	btTransform physTransformModel;
	btScalar _convertTransformArray[16];
	bool isInitialized = false;
	void Init();
	void SyncTransformWithPhysicsModel();
	void SyncPhysicsModelWithTransform();
	glm::mat4 btScalar2glmMat4(btScalar* matrix);

	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive &ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(isKinematic);
		ar & BOOST_SERIALIZATION_NVP(mass);
	}
	template<class Archive>
	void load(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(isKinematic);
		ar & BOOST_SERIALIZATION_NVP(mass);
	}
};

