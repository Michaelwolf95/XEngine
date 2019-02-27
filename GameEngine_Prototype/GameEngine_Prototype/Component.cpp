#include "Component.h"

//ComponentTypeInfo::ComponentTypeInfo(std::string _name, Component*(*_constructor)())
//{
//	name = _name;
//	Constructor = _constructor;
//}

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