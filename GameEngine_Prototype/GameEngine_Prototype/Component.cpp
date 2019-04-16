#include "Component.h"

typemap & Component::registry() { static typemap impl; return impl; }

unsigned int Component::nextComponentID = 0;

Component::Component() 
{
	componentID = Component::nextComponentID++;
}
Component::~Component() {}

// Engine callbacks locking mechanism to assure Start gets called on first update.
void Component::callback_PerformStart()
{
	isStarted = true;
	Start();
}
void Component::callback_PerformUpdate()
{
	if (isStarted == false)
	{
		callback_PerformStart();
	}
	else
	{
		Update();
	}
}


ComponentTypeInfo::ComponentTypeInfo()
{
	name = "UNNAMED";
	Constructor = nullptr;
}
ComponentTypeInfo::ComponentTypeInfo(std::string _name, Component_ptr(*_constructor)())
{
	name = _name;
	Constructor = _constructor;
}