#pragma once
#include "Component.h"
#include <BulletPhysics/LinearMath/btAlignedAllocator.h>
#include <BulletPhysics/btBulletDynamicsCommon.h>

class Collider : public Component
{
public:
	//static Registrar<Collider> registrar;
	//void Start() override;
	//void Update() override;
	btPolyhedralConvexShape* colShape;
	Collider();
	~Collider();
};

