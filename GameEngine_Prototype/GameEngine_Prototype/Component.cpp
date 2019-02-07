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