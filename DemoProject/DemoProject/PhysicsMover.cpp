#include "PhysicsMover.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;
#include "Input.h"
#include "GameTime.h"

// Register to be created and serialized.
REGISTER_COMPONENT(PhysicsMover, "PhysicsMover")

PhysicsMover::PhysicsMover() {}
PhysicsMover::~PhysicsMover() {}

// Start is called on the objects first update.
void PhysicsMover::Start()
{
	if (rigidbody.get() == nullptr)
	{
		gameObject->FindComponent(typeid(Rigidbody), (Component_ptr*)&rigidbody);

		RB_SUBSCRIBE_COLLISION_ENTER(PhysicsMover)
	}
}

// Update is called once per frame.
void PhysicsMover::Update()
{
	if (rigidbody.get() == nullptr) return;

	float horz = 0;
	if (Input::GetKey(GLFW_KEY_A) || Input::GetKey(GLFW_KEY_LEFT))
		horz -= 1;
	if (Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_RIGHT))
		horz += 1;
	float vert = 0;
	if (Input::GetKey(GLFW_KEY_W) || Input::GetKey(GLFW_KEY_UP))
		vert -= 1;
	if (Input::GetKey(GLFW_KEY_S) || Input::GetKey(GLFW_KEY_DOWN))
		vert += 1;

	glm::vec3 force = glm::vec3(horz, 0, vert) * GameTime::deltaTime * speed;
	rigidbody->AddForce(force);

	if (Input::GetKeyDown(GLFW_KEY_SPACE))
	{
		rigidbody->AddForce(glm::vec3(0, jumpForce, 0));
	}
}

// Draw the inspector for your custom component.
void PhysicsMover::DrawInspector()
{
	ImGui::InputFloat("Speed", &speed);
	ImGui::InputFloat("Jump Force", &jumpForce);
}

void PhysicsMover::OnEnable()
{

}

void PhysicsMover::OnDisable()
{
}

void PhysicsMover::OnCollisionEnter(XEngine::CollisionInfo info)
{
	std::cout << "ON COLLISION ENTER!!! - " << info.otherRigidbody->gameObject->name << std::endl;
}
