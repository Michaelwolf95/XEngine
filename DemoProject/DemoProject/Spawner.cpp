#include "Spawner.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;
#include "Input.h"
#include "imgui_inspector_extensions.h"
#include "Rigidbody.h"

// Register to be created and serialized.
REGISTER_COMPONENT(Spawner, "Spawner")

Spawner::Spawner() {}
Spawner::~Spawner() {}

// Start is called on the objects first update.
void Spawner::Start()
{
	if (audioComponent == nullptr)
	{
		this->gameObject->FindComponent(typeid(AudioComponent), (void**)&audioComponent);
	}
}

// Update is called once per frame.
void Spawner::Update()
{
	//if (Input::GetKeyDown(GLFW_KEY_1))
	if(Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_1))
	{
		if (audioComponent != nullptr)
			audioComponent->Play();

		auto go = GameObject::InstantiatePrefab(prefabPath);
		go->transform->setPosition(this->gameObject->transform->getPosition());
		go->transform->setRotation(this->gameObject->transform->getRotation());

		Rigidbody* rb;
		if (go->FindComponent<Rigidbody>(rb))
		{
			// Sync physics model with object position.
			rb->SyncPhysicsModelWithTransform();

			glm::vec3 dir = this->gameObject->transform->getForwardDirection();
			dir.x *= -1;
			dir.z *= -1;
			rb->AddImpulseForce(dir*50.0f);
		}
		
	}
}

// Draw the inspector for your custom component.
void Spawner::DrawInspector()
{
	GUI::FileReference(prefabPath, ".prefab", "Prefab");
}

