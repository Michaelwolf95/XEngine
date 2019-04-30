#include "SphereCollider.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include "RenderManager.h"
#include "GameObject.h"
#include "PhysicsManager.h"

REGISTER_COMPONENT(XEngine::SphereCollider, "SphereCollider")
namespace XEngine
{


	SphereCollider::SphereCollider()
	{
	}


	SphereCollider::~SphereCollider()
	{
	}
	btConvexInternalShape * SphereCollider::CreateCollisionShape()
	{
		if (!isInitialized)
		{
			colShape = new btSphereShape((btScalar)radius);
		}
		return colShape;
	}
	void SphereCollider::Start()
	{
		Collider::Start();
	}
	void SphereCollider::Update()
	{
		((btSphereShape*)colShape)->setUnscaledRadius(radius);
		glm::vec3 scale = this->gameObject->transform->getScale();
		btVector3 halfExtents(
			scale.x * radius,
			scale.y * radius,
			scale.z * radius);

		colShape->setLocalScaling(halfExtents);
	}
	void SphereCollider::DrawInspector()
	{
		ImGui::InputFloat("Radius", &radius);
	}
	void SphereCollider::OnDrawGizmosSelected()
	{
		glm::vec3 pos = this->gameObject->transform->getPosition();
		glm::vec3 scale = this->gameObject->transform->getScale();

		glm::vec4 color(0, 1, 0, 0.5f);

		RenderManager::DrawWorldSpaceSphere(pos, scale, radius, color, 1);
	}
}