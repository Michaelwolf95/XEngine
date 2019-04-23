#pragma once
#include "Component.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "btRefRigidbody.h"
//#include "Serialization.h"
//#include <BulletPhysics/LinearMath/btAlignedAllocator.h>
//ATTRIBUTE_ALIGNED16(class)
#include "boost/signals2.hpp"
#include "boost/function.hpp"
#include <boost/bind.hpp>

// Forward declare for friend access.
class boost::serialization::access;

namespace XEngine
{

	class ENGINE_API Rigidbody : public Component
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

		boost::signals2::signal<void()> OnCollisionEnterEvent;
		boost::signals2::signal<void()> OnCollisionStayEvent;
		boost::signals2::signal<void()> OnCollisionExitEvent;

		Rigidbody();
		~Rigidbody();
		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void DrawInspector() override;
		void OnDrawGizmosSelected() override;

		void AddForce(glm::vec3 force);

		static Rigidbody* GetAttachedRigidbody(GameObject* go);

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

#define RB_SUBSCRIBE_COLLISION_ENTER(ClassType)								\
XEngine::Rigidbody* _rb = XEngine::Rigidbody::GetAttachedRigidbody(this->gameObject);	\
if (_rb != nullptr) _rb->OnCollisionEnterEvent.connect(boost::bind(&ClassType::OnCollisionEnter, this));	\
/**/
//#define RB_SUBSCRIBE_COLLISION_STAY(ClassType)								\
//XEngine::Rigidbody* _rb = XEngine::Rigidbody::GetAttachedRigidbody(this->gameObject);	\
//if (_rb != nullptr) _rb->OnCollisionEnterEvent.connect(boost::bind(&ClassType::OnCollisionStay, this));	\
///**/
//#define RB_SUBSCRIBE_COLLISION_EXIT(ClassType)								\
//XEngine::Rigidbody* _rb = XEngine::Rigidbody::GetAttachedRigidbody(this->gameObject);	\
//if (_rb != nullptr) _rb->OnCollisionEnterEvent.connect(boost::bind(&ClassType::OnCollisionExit, this));	\
///**/