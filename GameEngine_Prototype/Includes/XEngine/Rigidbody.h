#pragma once
#include "Component.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "btRefRigidbody.h"
#include "Collider.h"
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
	class Rigidbody;
	class Collider;
	struct CollisionInfo
	{
	public:
		Rigidbody* otherRigidbody;
		Collider* otherCollider;
		glm::vec3 contactPoint;
		glm::vec3 contactNormal;
		CollisionInfo(Rigidbody* _other,
			Collider* _otherCollider = nullptr,
			glm::vec3 _contactPoint = glm::vec3(0, 0, 0),
			glm::vec3 _contactNormal = glm::vec3(0, 0, 0)
		);
	};

	class ENGINE_API Rigidbody : public Component
	{
	public:
		static Registrar<Rigidbody> registrar;

		std::shared_ptr<btRefRigidbody> body;// = nullptr;
		btMotionState* motionState = nullptr;
		//btRigidBody::btRigidBodyConstructionInfo rbInfo;
		float mass = 1.0f;
		bool isKinematic = false;

		//btVector3* localInertia = nullptr;
		//btVector3 localInertia;// (1, 1, 1);

		//btBoxShape* colShape = nullptr;
		//btVector3* boxColliderHalfExtents = nullptr;

		boost::signals2::signal<void(CollisionInfo&)> OnCollisionEnterEvent;
		boost::signals2::signal<void(CollisionInfo&)> OnCollisionStayEvent;
		boost::signals2::signal<void(CollisionInfo&)> OnCollisionExitEvent;

		Rigidbody();
		~Rigidbody();
		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void OnEnable() override;
		void DrawInspector() override;
		//void OnDrawGizmosSelected() override;

		void AddForce(glm::vec3 force);

		static Rigidbody* GetAttachedRigidbody(GameObject* go);

		void AttachCollider(Collider* col);
		void DetachCollider(Collider* col);
		bool isDynamic();
		bool getIsInitialized();

		void setIsTrigger(bool _isTrigger);
	private:
		friend class btRefRigidbody;
		friend class Collider;
		//friend class BoxCollider; //temp

		bool isTrigger = false;

		btTransform* physTransformModel = nullptr;

		// TODO: Convert to array once compound collider support is implemented.
		Collider* attachedCollider = nullptr;

		//btScalar* _convertTransformArray[16];
		bool isInitialized = false;
		void Init();
		void SyncTransformWithPhysicsModel();
		void SyncPhysicsModelWithTransform();
		glm::mat4 btScalar2glmMat4(btScalar* matrix);

		// Internal callbacks
		void _internal_CollisionEnterCallback(CollisionInfo& info);
		void _internal_CollisionStayCallback(CollisionInfo& info);
		void _internal_CollisionExitCallback(CollisionInfo& info);

		friend class ::boost::serialization::access;
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive>
		void save(Archive &ar, const unsigned int version) const
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(isKinematic);
			ar & BOOST_SERIALIZATION_NVP(mass);
			ar & BOOST_SERIALIZATION_NVP(isTrigger);
		}
		template<class Archive>
		void load(Archive &ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(isKinematic);
			ar & BOOST_SERIALIZATION_NVP(mass);
			ar & BOOST_SERIALIZATION_NVP(isTrigger);
		}
	};

}

#define RB_SUBSCRIBE_COLLISION_ENTER(ClassType)								\
XEngine::Rigidbody* _rb = XEngine::Rigidbody::GetAttachedRigidbody(this->gameObject);	\
if (_rb != nullptr) _rb->OnCollisionEnterEvent.connect(boost::bind(&ClassType::OnCollisionEnter, this, _1));	\
/**/
#define RB_SUBSCRIBE_COLLISION_STAY(ClassType)								\
XEngine::Rigidbody* _rb = XEngine::Rigidbody::GetAttachedRigidbody(this->gameObject);	\
if (_rb != nullptr) _rb->OnCollisionEnterEvent.connect(boost::bind(&ClassType::OnCollisionStay, this, _1));	\
/**/
#define RB_SUBSCRIBE_COLLISION_EXIT(ClassType)								\
XEngine::Rigidbody* _rb = XEngine::Rigidbody::GetAttachedRigidbody(this->gameObject);	\
if (_rb != nullptr) _rb->OnCollisionEnterEvent.connect(boost::bind(&ClassType::OnCollisionExit, this, _1));	\
/**/