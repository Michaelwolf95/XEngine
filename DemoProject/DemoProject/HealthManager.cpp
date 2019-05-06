#include "HealthManager.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;

// Register to be created and serialized.
REGISTER_COMPONENT(HealthManager, "HealthManager")

HealthManager::HealthManager() 
{
	this->currentHealth = 100.0f;
	this->maxHealth = 100.0f;
}
HealthManager::~HealthManager() {}

// Start is called on the objects first update.
void HealthManager::Start()
{

}

// Update is called once per frame.
void HealthManager::Update()
{

}

// Draw the inspector for your custom component.
void HealthManager::DrawInspector()
{

}

void HealthManager::applyDamage(float damage)
{
	this->currentHealth -= damage;
}