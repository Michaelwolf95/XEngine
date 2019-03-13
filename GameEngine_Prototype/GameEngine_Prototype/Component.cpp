#include "Component.h"

typemap & Component::registry() { static typemap impl; return impl; }

std::ostream & operator<<(std::ostream &os, const Component &comp)
{
	return os << comp;// .enabled;
}

Component::Component() {}
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



void Component::DrawInspector()
{

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