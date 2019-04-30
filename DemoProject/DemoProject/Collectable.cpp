#include "Collectable.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;
#include "Collector.h"

// Register to be created and serialized.
REGISTER_COMPONENT(Collectable, "Collectable")

Collectable::Collectable() {}
Collectable::~Collectable() {}

// Start is called on the objects first update.
void Collectable::Start()
{
	RB_SUBSCRIBE_COLLISION_ENTER(Collectable)

	if (audioComponent == nullptr)
	{
		this->gameObject->FindComponent(typeid(AudioComponent), (void**)&audioComponent);
	}
}

// Update is called once per frame.
void Collectable::Update()
{

}

// Draw the inspector for your custom component.
void Collectable::DrawInspector()
{
	GUI::ComponentReference<AudioComponent>(audioComponent, "Audio");
}

void Collectable::OnCollisionEnter(XEngine::CollisionInfo info)
{
	if (info.otherRigidbody->gameObject->name == "Player")
	{
		std::cout << "COLLIDED WITH PLAYER!\n";

		if(audioComponent != nullptr)
			audioComponent->Play();


		Collector* collector;
		if(info.otherRigidbody->gameObject->FindComponent(typeid(Collector), (void**)&collector))
		{
			collector->CollectItem();
		}

		this->gameObject->Delete();
		//SceneManager::getInstance().GetActiveScene()->DeleteGameObject(this->gameObject->GetSelfPtr());
	}
}

