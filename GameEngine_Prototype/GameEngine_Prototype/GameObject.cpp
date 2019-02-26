#include <vector>
#include "GameObject.h"
//#include "Transform.h"
#include "ApplicationManager.h"


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
	transform->gameObject = this;
}

GameObject::~GameObject()
{
}


std::ostream & operator<<(std::ostream &os, const GameObject &go)
{
	//std::list<bus_stop *>::const_iterator it;
	// note: we're displaying the pointer to permit verification
	// that duplicated pointers are properly restored.
	std::vector<Component*>::const_iterator it;
	for (it = go.components.begin(); it != go.components.end(); it++) {
		os << '\n' << std::hex << "0x" << *it << std::dec << ' ' << **it;
	}

	return os;
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
	if((ApplicationManager::getInstance().IsEditMode() == false) || comp->executeInEditMode)
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
	if ((ApplicationManager::getInstance().IsEditMode() == false) || comp->executeInEditMode)
		comp->Update();
}

Component* GameObject::FilterComponent(std::function<bool(Component*)> predicate)
{
	for (Component* c : components)
	{
		if (predicate(c))
		{
			return c;
		}
	}
	return NULL;
}

// Finds an object matching the typeInfo type definition.
bool GameObject::FindComponent(const std::type_info& typeInfo, void** object)
{
	return (this->FilterComponent([&](Component* c)->bool {
		if (typeid(*c) == typeInfo)
		{
			// Set the value of the pointer-pointer to the value of the pointer that we just found.
			*object = c; 
			return true;
		}
		return false;
	}));
}
