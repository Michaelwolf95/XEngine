#include "ExampleComponent.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui_inspector_extensions.h"
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
	ImGui::GameObjectReference(target, "Target");
	
}