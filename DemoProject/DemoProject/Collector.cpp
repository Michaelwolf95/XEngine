#include "Collector.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;
#include "Text.h"
#include <string>

// Register to be created and serialized.
REGISTER_COMPONENT(Collector, "Collector")

Collector::Collector() {}
Collector::~Collector() {}

// Start is called on the objects first update.
void Collector::Start()
{
	if (textGameObject != nullptr)
	{
		std::cout << textGameObject << std::endl;
		textGameObject->FindComponent(typeid(Text), (void**)&uiText);

		if (uiText != nullptr)
		{
			//uiText->text ="0";
			uiText->setText("0");
		}
	}
}

// Update is called once per frame.
void Collector::Update()
{

}

// Draw the inspector for your custom component.
void Collector::DrawInspector()
{
	GUI::GameObjectReference(textGameObject, "Go");
	//GUI::ComponentReference<Text>(uiText, "Text");
}

void Collector::CollectItem()
{
	count++;
	if (uiText != nullptr)
	{
		uiText->setText(std::to_string(count));
	}
}

