#include "Transform.h"



Transform::Transform() : Component::Component()
{
}

Transform::~Transform()
{

}

void Transform::SetParent(Transform * _parent)
{
	// ToDo: Make sure the object isn't a child.
	parent = _parent;
}


void Transform::Start()
{

}
void Transform::Update()
{

}
void Transform::OnDestroy()
{

}
