#include "BoxCollider.h"
#include <BulletPhysics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include "RenderManager.h"
#include "GameObject.h"

REGISTER_COMPONENT(XEngine::BoxCollider, "BoxCollider")
namespace XEngine
{
	BoxCollider::BoxCollider()
	{
		//glm::vec3 scale = this->gameObject->transform->getScale();
		halfExtents = new btVector3(1,1,1);
	}
	BoxCollider::~BoxCollider() {}

	void BoxCollider::Start()
	{
	}

	void BoxCollider::Update()
	{
		glm::vec3 scale = this->gameObject->transform->getScale();
		halfExtents->setX(scale.x * boxSize.x);
		halfExtents->setY(scale.y * boxSize.y);
		halfExtents->setZ(scale.z * boxSize.z);
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
		//TODO: Replace this with a "DrawWorldSpaceBox" Method.

		glm::vec3 pos = this->gameObject->transform->getPosition();
		//glm::vec3 halfExt = *(glm::vec3*)&colShape->getHalfExtentsWithMargin();

		glm::vec3 scale = this->gameObject->transform->getScale();
		glm::vec3 halfExt(scale.x * boxSize.x, scale.y * boxSize.y, scale.z * boxSize.z);
		// this->gameObject->transform->getScale();// *0.5f;
		//colShape->getHalfExtentsWithMargin()
		//RenderManager::DrawWorldSpaceBox(pos, halfExt, glm::vec4(0, 0, 1, 1), 2);

		glm::vec3 right = glm::normalize(this->gameObject->transform->getRightDirection());
		glm::vec3 up = glm::normalize(this->gameObject->transform->getUpDirection());
		glm::vec3 forward = glm::normalize(this->gameObject->transform->getForwardDirection());
		glm::vec3 corners[8] = {
			pos + (forward * halfExt.z) + (up * halfExt.y) + (right * halfExt.x), // 0
			pos + (forward * halfExt.z) + (up * halfExt.y) - (right * halfExt.x), // 1
			pos + (forward * halfExt.z) - (up * halfExt.y) + (right * halfExt.x), // 2
			pos + (forward * halfExt.z) - (up * halfExt.y) - (right * halfExt.x), // 3
			pos - (forward * halfExt.z) + (up * halfExt.y) + (right * halfExt.x), // 4
			pos - (forward * halfExt.z) + (up * halfExt.y) - (right * halfExt.x), // 5
			pos - (forward * halfExt.z) - (up * halfExt.y) + (right * halfExt.x), // 6
			pos - (forward * halfExt.z) - (up * halfExt.y) - (right * halfExt.x), // 7
		};
		glm::vec4 color(0, 1, 0, 0.5f);
		int size = 1;
		//RenderManager::DrawWorldSpacePoint(pos + right * L, glm::vec4(1, 0, 0, 1), 5); // Center
		RenderManager::DrawWorldSpaceLine(corners[0], corners[1], color, size);
		RenderManager::DrawWorldSpaceLine(corners[0], corners[2], color, size);
		RenderManager::DrawWorldSpaceLine(corners[3], corners[1], color, size);
		RenderManager::DrawWorldSpaceLine(corners[3], corners[2], color, size);

		RenderManager::DrawWorldSpaceLine(corners[0], corners[4], color, size);
		RenderManager::DrawWorldSpaceLine(corners[1], corners[5], color, size);
		RenderManager::DrawWorldSpaceLine(corners[2], corners[6], color, size);
		RenderManager::DrawWorldSpaceLine(corners[3], corners[7], color, size);

		RenderManager::DrawWorldSpaceLine(corners[4], corners[5], color, size);
		RenderManager::DrawWorldSpaceLine(corners[4], corners[6], color, size);
		RenderManager::DrawWorldSpaceLine(corners[7], corners[5], color, size);
		RenderManager::DrawWorldSpaceLine(corners[7], corners[6], color, size);
	}
}