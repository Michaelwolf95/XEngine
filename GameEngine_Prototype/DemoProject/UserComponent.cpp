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
