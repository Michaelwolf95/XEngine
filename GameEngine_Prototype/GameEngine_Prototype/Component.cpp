#include "Component.h"


Component::Component()
{
}

Component::~Component()
{
}

std::ostream & operator<<(std::ostream &os, const Component &comp)
{
	return os << comp.enabled; // << bs.description();
}

typemap & Component::registry() { static typemap impl; return impl; }

ComponentTypeInfo::ComponentTypeInfo()
{
	name = "UNNAMED";
	Constructor = nullptr;
}
ComponentTypeInfo::ComponentTypeInfo(std::string _name, Component*(*_constructor)())
{
	name = _name;
	Constructor = _constructor;
}