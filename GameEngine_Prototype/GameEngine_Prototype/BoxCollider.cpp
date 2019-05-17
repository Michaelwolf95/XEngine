#include "BoxCollider.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include "RenderManager.h"
#include "GameObject.h"
#include "PhysicsManager.h"

REGISTER_COMPONENT(XEngine::BoxCollider, "BoxCollider")
namespace XEngine
{
	BoxCollider::BoxCollider()
	{
		//glm::vec3 scale = this->gameObject->transform->getScale();
		halfExtents = new btVector3(1,1,1);
	}
	BoxCollider::~BoxCollider() 
	{
		if (isInitialized)
		{
			//std::cout << "\tDeleting/removing BoxCollider..." << std::endl;
			delete halfExtents;
			halfExtents = nullptr;
			Collider::~Collider();
		}
	}

	btConvexInternalShape * BoxCollider::CreateCollisionShape()
	{
		if (!isInitialized)
		{
			colShape = new btBoxShape(*halfExtents);
			glm::vec3 scale = this->gameObject->transform->getScale();
			halfExtents->setX(scale.x * boxSize.x);
			halfExtents->setY(scale.y * boxSize.y);
			halfExtents->setZ(scale.z * boxSize.z);

			colShape->setLocalScaling(*halfExtents);
		}
		return colShape;
	}


	void BoxCollider::Start()
	{
		Collider::Start();
	}

	void BoxCollider::Update()
	{
		//TODO: Sync using shape setLocalScale instead. Also move to base class.
		glm::vec3 scale = this->gameObject->transform->getScale();
		halfExtents->setX(scale.x * boxSize.x);
		halfExtents->setY(scale.y * boxSize.y);
		halfExtents->setZ(scale.z * boxSize.z);

		colShape->setLocalScaling(*halfExtents);
	}
	void BoxCollider::FixedUpdate()
	{
	}
	void BoxCollider::DrawInspector()
	{
		ImGui::InputFloat3("Size", (float*)&boxSize);
	}
	void BoxCollider::OnDrawGizmosSelected()
	{
		glm::vec4 color(0, 1, 0, 0.5f);
		glm::mat4 model = this->gameObject->transform->getMatrix4x4();
		model = glm::scale(model, boxSize*2.0f);
		RenderManager::DrawWorldSpaceBox(model, color, 1);
	}
}