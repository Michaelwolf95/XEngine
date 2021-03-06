#include "DamageTester.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
#include <iostream>
using namespace XEngine;

// Register to be created and serialized.
REGISTER_COMPONENT(DamageTester, "DamageTester")

DamageTester::DamageTester() 
{
	this->target = nullptr;
	this->health = nullptr;
}
DamageTester::~DamageTester() {}

// Start is called on the objects first update.
void DamageTester::Start()
{
	RB_SUBSCRIBE_COLLISION_ENTER(DamageTester)

	HealthManager* tempManager = nullptr;

	if (this->target != nullptr)
	{
		if (this->target->FindComponent(typeid(HealthManager), (void**)&tempManager))
		{
			std::cout << "Health Manager has connected." << std::endl;
			health = (HealthManager*)(tempManager);
		}
		else
		{
			std::cout << "Health Manager failed to connect." << std::endl;
		}
	}
}

// Update is called once per frame.
void DamageTester::Update()
{

}

// Draw the inspector for your custom component.
void DamageTester::DrawInspector()
{
	GUI::GameObjectReference(target, "Target");
}

void DamageTester::OnCollisionEnter(XEngine::CollisionInfo info)
{
	if (info.otherRigidbody->gameObject->name != "Floor")
	{
		//std::cout << "COLLIDED WITH PLAYER!\n";
		this->health->applyDamage(10);
		//this->gameObject->Delete();
		//SceneManager::getInstance().GetActiveScene()->DeleteGameObject(this->gameObject->GetSelfPtr());
	}
}