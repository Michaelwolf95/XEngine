#pragma once
#include "Component.h"
#include "Rigidbody.h"
#include <BulletPhysics/LinearMath/btAlignedAllocator.h>
#include <BulletPhysics/btBulletDynamicsCommon.h>
namespace XEngine
{

	class ENGINE_API Collider : public Component
	{
	public:
		//static Registrar<Collider> registrar;
		void Start() override;
		//void Update() override;
		Collider();
		~Collider();
		btConvexInternalShape* GetColShape();
	protected:
		bool isInitialized = false;
		virtual btConvexInternalShape* CreateCollisionShape() = 0;
		virtual void Init();
		btConvexInternalShape* colShape = nullptr;
		Rigidbody* attachedRigidbody = nullptr;
	};

}