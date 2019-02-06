#include "Component.h"

Component::Component()
{
}

Component::~Component()
{
}

template<class Archive>
void Component::serialize(Archive &ar, const unsigned int version)
{
	ar & enabled;
}

std::ostream & operator<<(std::ostream &os, const Component &comp)
{
	return os << comp.enabled; // << bs.description();
}