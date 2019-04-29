#include "ExampleComponent.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui_inspector_extensions.h"
using namespace XEngine;

REGISTER_COMPONENT(ExampleComponent, "ExampleComponent")

ExampleComponent::ExampleComponent() {}
ExampleComponent::~ExampleComponent() {}

void ExampleComponent::Start()
{
	if (target != nullptr)
	{
		std::cout << target->name << std::endl;
	}
}

void ExampleComponent::Update()
{
}

void ExampleComponent::DrawInspector()
{
	GUI::GameObjectReference(target, "Target");

	//GUI::ComponentReference(typeid(XEngine::Rigidbody), ((Component*&)rigidbody), "Rigidbody");
	GUI::ComponentReference<Rigidbody>(rigidbody, "Rigidbody");

	GUI::ComponentReference<Rigidbody>(sRigidbody, "sRigidbody");


	//std::cout << "Count: " << wRigidbody.use_count() << std::endl;
	auto sp = wRigidbody.lock();
	if (sp.get() != nullptr)
	{
		//std::cout << "Before: " << sp->gameObject->name << std::endl;
	}
	GUI::ComponentReference<Rigidbody>(sp, "wRigidbody");

	if (sp.get() != nullptr)
	{
		//std::cout << "After: " << sp->gameObject->name << std::endl;
		//std::cout << "_Count: " << wRigidbody.use_count() << std::endl;
	}
	wRigidbody = sp;

	//ImGui::Text("%i", wRigidbody.use_count());
	/*if (wRigidbody.get() != nullptr)
		std::cout << wRigidbody->gameObject->name << std::endl;*/
}