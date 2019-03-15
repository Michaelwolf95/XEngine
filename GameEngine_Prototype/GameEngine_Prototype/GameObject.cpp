#include <vector>
#include "GameObject.h"
#include "ApplicationManager.h"

std::ostream & operator<<(std::ostream &os, const GameObject &go)
{
	// From Tutorial:
	//    note: we're displaying the pointer to permit verification
	//    that duplicated pointers are properly restored.
	std::vector<Component_ptr>::const_iterator it;
	for (it = go.components.begin(); it != go.components.end(); it++) {
		os << '\n' << std::hex << "0x" << *it << std::dec << ' ' << **it;
	}

	return os;
}

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
	transform->gameObject = this;// shared_from_this();
}

GameObject::~GameObject()
{
	std::cout << "\tDeconstructing " << name << "..." << std::endl;
	components.clear();
}

std::vector<GameObject*> GameObject::GetChildren()
{
	std::vector<GameObject*> children;
	for (size_t i = 0; i < this->transform->children.size(); i++)
	{
		children.push_back(this->transform->children[i]->gameObject);
	}
	return children;
}

GameObject* GameObject::GetChild(int index)
{
	if (index >= 0 && index < this->transform->children.size())
	{
		return this->transform->children[index]->gameObject;
	}
	return nullptr;
}

void GameObject::AddComponent(Component_ptr comp)
{
	components.push_back(comp);
	comp->gameObject = this;// shared_from_this();
}

void GameObject::RemoveComponent(Component_ptr comp)
{
	// If vector contains it, remove it.
	// https://stackoverflow.com/questions/39912/how-do-i-remove-an-item-from-a-stl-vector-with-a-certain-value
	auto n = std::find(components.begin(), components.end(), comp);
	if (n != components.end())
	{
		// swap the one to be removed with the last element and remove the item at the end of the container
		// to prevent moving all items after it by one
		std::swap(*n, components.back());
		components.pop_back();
	}
}

void GameObject::EmitComponentEvent(void(*eventFunction)(Component_ptr))
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

//TODO: Remove dependency on ApplicationManager
void GameObject::StartComponent(Component_ptr comp)
{
	if((ApplicationManager::getInstance().IsEditMode() == false) || comp->executeInEditMode)
		comp->callback_PerformStart();
}

void GameObject::UpdateComponents()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		UpdateComponent(components[i]);
	}
}

void GameObject::UpdateComponent(Component_ptr comp)
{
	if ((ApplicationManager::getInstance().IsEditMode() == false) || comp->executeInEditMode)
		comp->callback_PerformUpdate();
}
void GameObject::FixedUpdateComponents()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		FixedUpdateComponent(components[i]);
	}
}

void GameObject::FixedUpdateComponent(Component_ptr comp)
{
	if ((ApplicationManager::getInstance().IsEditMode() == false) || comp->executeInEditMode)
		comp->FixedUpdate();
}

Component_ptr GameObject::FilterComponent(std::function<bool(Component_ptr)> predicate)
{
	for (Component_ptr c : components)
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
	return (this->FilterComponent([&](Component_ptr c)->bool {
		if (typeid(*(c.get())) == typeInfo)
		{
			// Set the value of the pointer-pointer to the value of the pointer that we just found.
			*object = (c.get()); 
			return true;
		}
		return false;
	})!= NULL);
}

// WARNING: This can cause crashes if a shared_ptr to the object does not already exist.
// TODO: Add safety measure to prevent crashes.
GameObject_ptr GameObject::GetSelfPtr()
{
	return shared_from_this();
}
