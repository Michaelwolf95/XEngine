#include <vector>
#include "GameObject.h"
#include "Transform.h"
#include "ApplicationManager.h"
#include "SceneManager.h"
#include "Serialization.h"
#include <sstream>
#include <memory>
#include "AssetManager.h"


#define DEFAULT_PREFAB_DIRECTORY (ASSET_FILE_PATH + std::string("Prefabs/"))	


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
	HandleDisable();
	std::cout << "\tDeconstructing " << name << "..." << std::endl;
	components.clear();
}

GameObject * GameObject::GetParent()
{
	GameObject* parent = nullptr;
	Transform* transParent = this->transform->GetParent();
	if (transParent != nullptr)
		parent = transParent->gameObject;
	return parent;
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

bool GameObject::IsActiveInHierarchy()
{
	if (!isActive) return false;
	if (this->transform->parent != nullptr)
	{
		return this->transform->parent->gameObject->IsActiveInHierarchy();
	}
	return true;
}

void GameObject::SetActive(bool active)
{
	if (isActive != active)
	{
		isActive = active;
		parentHierarchyActive = (this->transform->parent != nullptr)?
			this->transform->parent->gameObject->IsActiveInHierarchy() : true;
		// Enabling/Disabling only matters if the objects activation is not overriden by its parents.
		if (parentHierarchyActive)
		{
			if (isActive)
				HandleEnable();
			else
				HandleDisable();
			// Update children with hierarchy change.
			HandleHierarchyChanged();
		}
	}
}

void GameObject::HandleHierarchyChanged()
{
	bool newParentHierarchyActive = (this->transform->parent != nullptr) ?
		this->transform->parent->gameObject->IsActiveInHierarchy() : true;
	// If previous parent was inactive, and new parent is active -> Update as enable.
	// If previous parent was active, and new parent is inactive -> Update as disable.
	if (isActive)
	{
		if (newParentHierarchyActive && !parentHierarchyActive)
		{
			HandleEnable();
		}
		else if (!newParentHierarchyActive && parentHierarchyActive)
		{
			HandleDisable();
		}
	}
	
	parentHierarchyActive = newParentHierarchyActive;

	// Recursively update children.
	for (size_t i = 0; i < this->transform->children.size(); i++)
	{
		this->transform->children[i]->gameObject->HandleHierarchyChanged();
	}
}

void GameObject::AddComponent(Component_ptr comp)
{
	components.push_back(comp);
	comp->gameObject = this;// shared_from_this();

	if (this->IsActiveInHierarchy())
	{
		comp->OnEnable();
	}
}

void GameObject::RemoveComponent(Component_ptr comp)
{
	// If vector contains it, remove it.
	// https://stackoverflow.com/questions/39912/how-do-i-remove-an-item-from-a-stl-vector-with-a-certain-value
	auto n = std::find(components.begin(), components.end(), comp);
	if (n != components.end())
	{
		//delete comp.get();
		//comp.reset();
		comp->gameObject = nullptr;

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

bool GameObject::FindComponent(const std::type_info& typeInfo, Component_ptr* object)
{
	return (this->FilterComponent([&](Component_ptr c)->bool {
		if (typeid(*(c.get())) == typeInfo)
		{
			// Set the value of the pointer-pointer to the value of the pointer that we just found.
			*object = c;// (c.get());
			return true;
		}
		return false;
	}) != NULL);
}

// WARNING: This can cause crashes if a shared_ptr to the object does not already exist.
// TODO: Add safety measure to prevent crashes.
GameObject_ptr GameObject::GetSelfPtr()
{
	if (this->isFlaggedForDeletion) 
		return nullptr;
	return shared_from_this();
}

GameObject_ptr GameObject::DuplicateSingle(GameObject_ptr ref)
{
	GameObject_ptr dupli = SceneManager::getInstance().GetActiveScene()->CreateGameObject("TEMP");

	std::string fileName = "../Temp/DUPLICATE_GAMEOBJECT.TEMP";

	
	// SCOPE REQUIRED FOR ARCHIVE.
	{
		std::ofstream ofs(fileName);
		//std::stringstream();
		if (!ofs)
		{
			std::cout << "Cannot open outfile" << std::endl;
			return nullptr;
		}
		if (ofs.bad())
		{
			std::cout << "Out File Stream BAD" << std::endl;
			return nullptr;
		}
		boost::archive::xml_oarchive oa(ofs);
		//oa << boost::serialization::make_nvp("Hierarchy", allObjsInHierarchy);
		std::cout << "Writing " << ref->name.c_str() << std::endl;
		oa << boost::serialization::make_nvp("go", *ref);// BOOST_SERIALIZATION_NVP(&this);
		ofs.flush();
	}
	std::cout << "SAVED TO FILE" << std::endl;

	// SCOPE REQUIRED FOR ARCHIVE.
	{
		std::ifstream ifs(fileName);
		if (!ifs.good()) //Doesn't exist 
		{
			std::cout << "File doesnt exist" << std::endl;
			return nullptr;
		}
		boost::archive::xml_iarchive ia(ifs);
		//boost::archive::xml_iarchive* ia = new boost::archive::xml_iarchive(ifs);
		try
		{
			std::cout << "Attempting Load" << std::endl;
			ia >> boost::serialization::make_nvp("go", *dupli);//BOOST_SERIALIZATION_NVP(go);		// Restore from the archive
			//ia >> boost::serialization::make_nvp("Hierarchy", clonedHierarchy);
		}
		catch (const std::exception& e)
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "ERROR: FAILED TO DUPLICATE.\nProbably due to outdated serialization." << std::endl;
			std::cout << e.what() << std::endl;
			std::cout << "==================================================" << std::endl;
			return nullptr;// false;
		}
	}
	//dupli = clonedHierarchy[0];

	std::cout << "LOADED FROM FILE" << std::endl;
	dupli->name = ref->name + "_Copy";
	dupli->transform->SetParent(ref->transform->GetParent());
	std::cout << "FINISHED DUPLICATE" << std::endl;
	return dupli;
}

GameObject_ptr GameObject::Duplicate(GameObject_ptr ref)
{
	//GameObject_ptr dupli = SceneManager::getInstance().GetActiveScene()->CreateGameObject("TEMP");

	std::string fileName = "../Temp/DUPLICATE_GAMEOBJECT.TEMP";

	std::vector<GameObject_ptr> hierarchy;
	GameObject::GetFlattenedHierarchy(ref, hierarchy);

	for (size_t i = 0; i < hierarchy.size(); i++)
	{
		//clonedHierarchy.push_back(SceneManager::getInstance().GetActiveScene()->CreateGameObject((hierarchy[i]->name +"_Copy").c_str() ));
	}

	// SCOPE REQUIRED FOR ARCHIVE.
	{
		std::ofstream ofs(fileName);
		//std::stringstream();
		if (!ofs)
		{
			std::cout << "Cannot open outfile" << std::endl;
			return nullptr;
		}
		if (ofs.bad())
		{
			std::cout << "Out File Stream BAD" << std::endl;
			return nullptr;
		}
		boost::archive::xml_oarchive oa(ofs);
		//oa << boost::serialization::make_nvp("Hierarchy", allObjsInHierarchy);
		std::cout << "Writing " << ref->name.c_str() << std::endl;
		//oa << boost::serialization::make_nvp("go", *ref);// BOOST_SERIALIZATION_NVP(&this);
		oa << boost::serialization::make_nvp("gameObjects", hierarchy);
		ofs.flush();
	}
	std::cout << "SAVED TO FILE" << std::endl;

	std::vector<GameObject_ptr> clonedHierarchy;
	// SCOPE REQUIRED FOR ARCHIVE.
	{
		std::ifstream ifs(fileName);
		if (!ifs.good()) //Doesn't exist 
		{
			std::cout << "File doesnt exist" << std::endl;
			return nullptr;
		}
		boost::archive::xml_iarchive ia(ifs);
		//boost::archive::xml_iarchive* ia = new boost::archive::xml_iarchive(ifs);
		try
		{
			std::cout << "Attempting Load" << std::endl;
			//ia >> boost::serialization::make_nvp("go", *dupli);//BOOST_SERIALIZATION_NVP(go);		// Restore from the archive
			ia >> boost::serialization::make_nvp("gameObjects", clonedHierarchy);
		}
		catch (const std::exception& e)
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "ERROR: FAILED TO DUPLICATE.\nProbably due to outdated serialization." << std::endl;
			std::cout << e.what() << std::endl;
			std::cout << "==================================================" << std::endl;
			return nullptr;// false;
		}
	}
	for (size_t i = 0; i < clonedHierarchy.size(); i++)
	{
		SceneManager::getInstance().GetActiveScene()->AddExistingGameObject(clonedHierarchy[i]);
		//clonedHierarchy.push_back(SceneManager::getInstance().GetActiveScene()->CreateGameObject((hierarchy[i]->name +"_Copy").c_str() ));
	}


	GameObject_ptr dupli = clonedHierarchy[0];

	std::cout << "LOADED FROM FILE" << std::endl;
	//dupli->name = ref->name + "_Copy";
	dupli->transform->SetParent(ref->transform->GetParent());
	std::cout << "FINISHED DUPLICATE" << std::endl;

	for (size_t i = 0; i < clonedHierarchy.size(); i++)
	{
		if (clonedHierarchy[i]->IsActiveInHierarchy())
			clonedHierarchy[i]->HandleEnable();
	}

	return dupli;
}

void GameObject::GetFlattenedHierarchy(GameObject_ptr current, std::vector<GameObject_ptr>& vec)
{
	vec.push_back(current);
	std::vector<GameObject*> children = current->GetChildren();
	for (size_t i = 0; i < children.size(); i++)
	{
		GetFlattenedHierarchy(children[i]->GetSelfPtr(), vec);
	}
}

void GameObject::Delete()
{
	SceneManager::getInstance().GetActiveScene()->ScheduleDelete(this->GetSelfPtr());
	isFlaggedForDeletion = true;
}


// PREFABS
void GameObject::CreatePrefab(GameObject_ptr ref)
{
	if (CreateDirectory((DEFAULT_PREFAB_DIRECTORY).c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		// CopyFile(...)
	}
	std::string fileName = DEFAULT_PREFAB_DIRECTORY + ref->name + std::string(PREFAB_FILE_EXT);

	std::vector<GameObject_ptr> hierarchy;
	GameObject::GetFlattenedHierarchy(ref, hierarchy);

	// SCOPE REQUIRED FOR ARCHIVE.
	{
		std::ofstream ofs(fileName);
		//std::stringstream();
		if (!ofs)
		{
			std::cout << "Cannot open outfile" << std::endl;
			return;
		}
		if (ofs.bad())
		{
			std::cout << "Out File Stream BAD" << std::endl;
			return;
		}
		boost::archive::xml_oarchive oa(ofs);
		oa << boost::serialization::make_nvp("gameObjects", hierarchy);
		ofs.flush();
	}
}

GameObject_ptr GameObject::InstantiatePrefab(std::string fileName)
{

	std::vector<GameObject_ptr> clonedHierarchy;
	// SCOPE REQUIRED FOR ARCHIVE.
	{
		std::ifstream ifs(fileName);
		if (!ifs.good()) //Doesn't exist 
		{
			std::cout << "File doesnt exist" << std::endl;
			return nullptr;
		}
		boost::archive::xml_iarchive ia(ifs);
		try
		{
			ia >> boost::serialization::make_nvp("gameObjects", clonedHierarchy);
		}
		catch (const std::exception& e)
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "ERROR: FAILED TO DUPLICATE.\nProbably due to outdated serialization." << std::endl;
			std::cout << e.what() << std::endl;
			std::cout << "==================================================" << std::endl;
			return nullptr;// false;
		}
	}
	for (size_t i = 0; i < clonedHierarchy.size(); i++)
	{
		SceneManager::getInstance().GetActiveScene()->AddExistingGameObject(clonedHierarchy[i]);
	}

	GameObject_ptr dupli = clonedHierarchy[0];

	dupli->transform->SetParent(nullptr);

	for (size_t i = 0; i < clonedHierarchy.size(); i++)
	{
		if (clonedHierarchy[i]->IsActiveInHierarchy())
			clonedHierarchy[i]->HandleEnable();
	}
	return dupli;
}

//

void GameObject::HandleEnable()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->OnEnable();
	}
}

void GameObject::HandleDisable()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->OnDisable();
	}
}
