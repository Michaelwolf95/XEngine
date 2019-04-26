#include "ExampleComponent.h"

REGISTER_COMPONENT(ExampleComponent, "ExampleComponent")

ExampleComponent::ExampleComponent() {}
ExampleComponent::~ExampleComponent() {}

void ExampleComponent::Start()
{

}

void ExampleComponent::Update()
{
}

void ExampleComponent::DrawInspector()
{
	ImGui::Text("Hi Gabe!");
}