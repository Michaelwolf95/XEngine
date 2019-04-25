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
		btPolyhedralConvexShape* GetColShape();
	protected:
		bool isInitialized = false;
		virtual btPolyhedralConvexShape* CreateCollisionShape() = 0;
		virtual void Init();
		btPolyhedralConvexShape* colShape = nullptr;
		Rigidbody* attachedRigidbody = nullptr;
	};

}