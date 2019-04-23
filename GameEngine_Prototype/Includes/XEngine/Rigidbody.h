#pragma once
#include "Component.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "btRefRigidbody.h"
//#include "Serialization.h"
//#include <BulletPhysics/LinearMath/btAlignedAllocator.h>
//ATTRIBUTE_ALIGNED16(class)

// Forward declare for friend access.
class boost::serialization::access;

namespace XEngine
{
	class Rigidbody : public Component
	{
	public:
		static Registrar<Rigidbody> registrar;

		btRefRigidbody* body = nullptr;
		btDefaultMotionState* motionState = nullptr;
		//btRigidBody::btRigidBodyConstructionInfo rbInfo;
		float mass = 1.0f;
		bool isKinematic = false;

		btBoxShape* colShape = nullptr;
		btVector3* boxColliderHalfExtents = nullptr;

		Rigidbody();
		~Rigidbody();
		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void DrawInspector() override;
		void OnDrawGizmosSelected() override;

		void AddForce(glm::vec3 force);

	private:
		friend class btRefRigidbody;

		btTransform* physTransformModel = nullptr;
		//btScalar* _convertTransformArray[16];
		bool isInitialized = false;
		void Init();
		void SyncTransformWithPhysicsModel();
		void SyncPhysicsModelWithTransform();
		glm::mat4 btScalar2glmMat4(btScalar* matrix);

		// Internal callbacks
		void _internal_CollisionEnterCallback(btRefRigidbody* other);
		void _internal_CollisionStayCallback(btRefRigidbody* other);
		void _internal_CollisionExitCallback(btRefRigidbody* other);

		friend class ::boost::serialization::access;
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

}