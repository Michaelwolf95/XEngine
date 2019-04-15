#include "UserComponent.h"

REGISTER_COMPONENT(UserComponent, "UserComponent")

UserComponent::UserComponent() {}
UserComponent::~UserComponent() {}

void UserComponent::Start()
{
	std::cout << "============================== State 1" << std::endl;
}

void UserComponent::Update()
{
}

void UserComponent::DrawInspector()
{
	ImGui::Text("Here's some Text!");
}
