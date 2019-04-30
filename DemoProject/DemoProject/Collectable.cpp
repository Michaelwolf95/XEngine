#include "Collectable.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;

// Register to be created and serialized.
REGISTER_COMPONENT(Collectable, "Collectable")

Collectable::Collectable() {}
Collectable::~Collectable() {}

// Start is called on the objects first update.
void Collectable::Start()
{
	RB_SUBSCRIBE_COLLISION_ENTER(Collectable)
}

// Update is called once per frame.
void Collectable::Update()
{

}

// Draw the inspector for your custom component.
void Collectable::DrawInspector()
{

}

void Collectable::OnCollisionEnter(XEngine::CollisionInfo info)
{
	if (info.otherRigidbody->gameObject->name == "Player")
	{
		std::cout << "COLLIDED WITH PLAYER!\n";
		this->gameObject->Delete();
		//SceneManager::getInstance().GetActiveScene()->DeleteGameObject(this->gameObject->GetSelfPtr());
	}
}

