#include "ExampleComponent.h"

REGISTER_COMPONENT(ExampleComponent, "ExampleComponent")

ExampleComponent::ExampleComponent() {}
ExampleComponent::~ExampleComponent() {}

void ExampleComponent::Start()
{
	std::cout << "============================== State 1" << std::endl;
}

void ExampleComponent::Update()
{
}

void ExampleComponent::DrawInspector()
{
	ImGui::Text("Here's some Text 5!");
}