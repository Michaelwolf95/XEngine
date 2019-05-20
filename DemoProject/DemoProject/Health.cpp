#include "Health.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
#include <iostream>
using namespace XEngine;

// Register to be created and serialized.
REGISTER_COMPONENT(Health, "Health")

Health::Health() 
{
	this->healthText = nullptr;
	this->health = nullptr;
	this->target = nullptr;
	//this->executeInEditMode = true;
}
Health::~Health() {}

// Start is called on the objects first update.
void Health::Start()
{
	this->connectToComponents();

	if (this->health != nullptr && this->healthText != nullptr)
		this->healthText->setText(std::to_string(this->health->currentHealth) + "/" + std::to_string(this->health->maxHealth));
}

// Update is called once per frame.
void Health::Update()
{
	//if (this->health->currentHealth > 0)
	//{
	//	this->healthText->setText(std::to_string(this->health->currentHealth) + "/" + std::to_string(this->health->maxHealth));
	//}
	//else
	//{
	//	this->healthText->setText("0/" + std::to_string(this->health->maxHealth));
	//}
}

// Draw the inspector for your custom component.
void Health::DrawInspector()
{
	GUI::GameObjectReference(target, "Target");
}

void Health::UpdateHPText(float damage)
{
	this->healthText->setText(std::to_string(this->health->currentHealth) + "/" + std::to_string(this->health->maxHealth));
}

void Health::UpdateHPText_Death()
{
	this->healthText->setText(std::to_string(0) + "/" + std::to_string(this->health->maxHealth));
}

void Health::connectToComponents()
{
	Text* tempText = nullptr;


	if (this->gameObject->FindComponent(typeid(Text), (void**)&tempText))
	{
		//std::cout << this->gameObject->name <<": Health Text has connected." << std::endl;
		healthText = (Text*)((char*)tempText - 8);
	}
	else
	{
		//std::cout << this->gameObject->name << ": Health Text failed to connect." << std::endl;
	}

	if (this->target != nullptr)
	{
		HealthManager* tempManager = nullptr;
		if (this->target->FindComponent(typeid(HealthManager), (void**)&tempManager))
		{
			//std::cout << this->gameObject->name << ": Health Manager has connected." << std::endl;
			health = (HealthManager*)(tempManager);
			health->OnTakeDamageEvent.connect(boost::bind(&Health::UpdateHPText, this, _1));
			health->OnDeathEvent.connect(boost::bind(&Health::UpdateHPText_Death, this));
		}
		else
		{
			//std::cout << this->gameObject->name << ": Health Manager failed to connect." << std::endl;
		}
	}
}