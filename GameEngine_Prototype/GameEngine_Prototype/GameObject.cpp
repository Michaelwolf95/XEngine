#include <vector>
#include "GameObject.h"
//#include "Transform.h"


GameObject::GameObject(const char* _name)
{
	if (_name == nullptr)
	{
		name = "New GameObject";
	}
	else
	{
		name = _name;
	}
	transform = new Transform();
}

GameObject::~GameObject()
{
}

void GameObject::AddComponent(Component * comp)
{
	components.push_back(comp);
	comp->gameObject = this;
}

void GameObject::RemoveComponent(Component * comp)
{
	// If vector contains it, remove it.
	// https://stackoverflow.com/questions/39912/how-do-i-remove-an-item-from-a-stl-vector-with-a-certain-value
	auto n = std::find(components.begin(), components.end(), comp);
	if (n != components.end())
	{
		// swap the one to be removed with the last element
		// and remove the item at the end of the container
		// to prevent moving all items after '5' by one
		std::swap(*n, components.back());
		components.pop_back();
		comp->gameObject = nullptr;
	}
}

void GameObject::EmitComponentEvent(void(*eventFunction)(Component*))
{
	for (size_t i = 0; i < components.size(); i++)
	{
		eventFunction(components[i]);
	}
}

void GameObject::StartComponents()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		StartComponent(components[i]);
	}
}

void GameObject::StartComponent(Component * comp)
{
	comp->Start();
}

void GameObject::UpdateComponents()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		UpdateComponent(components[i]);
	}
}

void GameObject::UpdateComponent(Component * comp)
{
	comp->Update();
}
