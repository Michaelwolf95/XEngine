#include <vector>
#include "GameObject.h"
//#include "Transform.h"
#include "ApplicationManager.h"

std::ostream & operator<<(std::ostream &os, const GameObject &go)
{
	//std::list<bus_stop *>::const_iterator it;
	// note: we're displaying the pointer to permit verification
	// that duplicated pointers are properly restored.
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
	std::cout << name << ", Transform: " << transform << std::endl;
}

GameObject::~GameObject()
{
	std::cout << "\tGameObject " << name << " Deconstructed." << std::endl;
	components.clear();
}
//
//// Public Access
//void GameObject::SetParent(GameObject_ptr newParent)
//{
//	if (newParent == parent)
//		return;
//	if (newParent == nullptr)
//	{
//		std::cout << "NOT YET IMPLEMENTED: GameObject set parent to null. Should be set as a rootGameObject for the scene." << std::endl;
//
//		// TODO: Set as rootGameObject in scene...?
//
//
//		this->transform->parent = nullptr;
//		return;
//	}
//	GameObject_ptr sharedSelf = shared_from_this();
//	
//	GameObject_ptr prevParent = this->parent;
//	if (prevParent != nullptr)
//	{
//		prevParent->RemoveChild(sharedSelf);
//	}
//
//	//newParent->children.push_back(sharedSelf);
//	newParent->AddChild(sharedSelf);
//
//	this->transform->parent = newParent->transform;
//
//}
//// PRIVATE ACCESS
//void GameObject::AddChild(GameObject_ptr child)
//{
//	if(child->parent.get() != this)
//	{
//		children.push_back(child);
//		
//		//child->transform->SetParent(this->transform);
//		//comp->gameObject = this;// shared_from_this();
//	}
//}
//// PRIVATE ACCESS
//void GameObject::RemoveChild(GameObject_ptr child)
//{
//	if (child->parent.get() == this)
//	{
//		std::swap(child, children.back());
//		children.pop_back();
//	}
//}

//
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


//void GameObject::AddComponent(Component* comp)
//{
//	//std::shared_ptr<comp> my_ptr(raw_ptr)
//	AddComponent(std::make_shared<Component>(comp));
//}
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
		// swap the one to be removed with the last element
		// and remove the item at the end of the container
		// to prevent moving all items after '5' by one
		std::swap(*n, components.back());
		components.pop_back();
		//comp->gameObject = nullptr;
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

void GameObject::StartComponent(Component_ptr comp)
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

void GameObject::UpdateComponent(Component_ptr comp)
{
	if ((ApplicationManager::getInstance().IsEditMode() == false) || comp->executeInEditMode)
		comp->Update();
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

GameObject_ptr GameObject::GetSelfPtr()
{
	return shared_from_this();
}
