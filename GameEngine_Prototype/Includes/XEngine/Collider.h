#pragma once
#include "Component.h"
#include <BulletPhysics/LinearMath/btAlignedAllocator.h>
#include <BulletPhysics/btBulletDynamicsCommon.h>

class ENGINE_API Collider : public Component
{
public:
	//static Registrar<Collider> registrar;
	//void Start() override;
	//void Update() override;
	btPolyhedralConvexShape* colShape;
	Collider();
	~Collider();
};

