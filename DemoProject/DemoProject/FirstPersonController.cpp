#include "FirstPersonController.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/projection.hpp"
#include "imgui_inspector_extensions.h"
// Register to be created and serialized.
REGISTER_COMPONENT(FirstPersonController, "FirstPersonController")

FirstPersonController::FirstPersonController() {}
FirstPersonController::~FirstPersonController() {}

// Start is called on the objects first update.
void FirstPersonController::Start()
{
	gameObject->FindComponent<Rigidbody>(rigidbody);
	rigidbody->body->setAngularFactor(btVector3(0, 0, 0));

	Input::ShowCursor(false);
}

// Update is called once per frame.
void FirstPersonController::Update()
{
	rotateCamera();

	float horz = 0.0f;
	float vert = 0.0f;
	if (Input::GetKey(GLFW_KEY_W))
	{
		vert += 1.0f;
	}
	if (Input::GetKey(GLFW_KEY_S))
	{
		vert -= 1.0f;
	}
	if (Input::GetKey(GLFW_KEY_A))
	{
		horz -= 1.0f;
	}
	if (Input::GetKey(GLFW_KEY_D))
	{
		horz += 1.0f;
	}

	glm::vec3 forward = cameraObject->transform->getForwardDirection();

	// ProjectOnPlane
	glm::vec3 planeNorm = glm::vec3(0, 1, 0);
	float projDist = glm::dot(forward, planeNorm);
	forward = glm::normalize(forward - (projDist * planeNorm));

	glm::vec3 right = cameraObject->transform->getRightDirection();
	projDist = glm::dot(right, planeNorm);
	right = glm::normalize(right - (projDist * planeNorm));

	glm::vec3 moveDir = (forward * vert) + (right * horz);
	if (moveDir.x + moveDir.z > 0)
		moveDir = glm::normalize(moveDir);

	//rigidbody->AddForce(moveDir * moveSpeed);
	glm::vec3 velocity = moveDir * moveSpeed;
	velocity.y = rigidbody->body->getLinearVelocity().y();
	rigidbody->SetVelocity(velocity);

	// JUMP!
	if (Input::GetKeyDown(GLFW_KEY_SPACE))
	{
		rigidbody->AddImpulseForce(glm::vec3(0, 8, 0));
	}
}

// Draw the inspector for your custom component.
void FirstPersonController::DrawInspector()
{
	ImGui::PushID(this);
	GUI::GameObjectReference(cameraObject, "Camera");
	ImGui::SliderFloat("Move Speed ", &moveSpeed, 0.1f, 20.0f, "%.1f");
	ImGui::SliderFloat("X RotSpeed", &xRotSpeed, 0.1f, 20.0f, "%.1f"); //TODO: allow min-max values to be modified
	ImGui::SliderFloat("Y RotSpeed", &yRotSpeed, 0.1f, 20.0f, "%.1f");
	ImGui::SliderFloat("Z RotSpeed", &zRotSpeed, 0.1f, 20.0f, "%.1f");
	ImGui::PopID();
}


void FirstPersonController::rotateCamera()
{
	if (Input::GetKeyUp(GLFW_KEY_P)) Input::ToggleCursor(); // temporary

	float deltaXRot = xRotSpeed * XEngine::GameTime::deltaTime * Input::GetDeltaPosX();
	float deltaYRot = yRotSpeed * XEngine::GameTime::deltaTime * Input::GetDeltaPosY();

	// gets current local rotation
	glm::vec3 localRot = cameraObject->transform->getLocalRotationEuler();

	// update current rotation
	localRot = glm::vec3(localRot.x + deltaYRot, localRot.y + deltaXRot, localRot.z);

	// set camera's location rotational position
	cameraObject->transform->setLocalRotationEuler(localRot);
}