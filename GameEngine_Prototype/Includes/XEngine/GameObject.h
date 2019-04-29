#pragma once
#include <vector>
#include <functional>
#include <typeinfo>
#include "Component.h"
#include "Transform.h"
#include "Serialization.h"

typedef std::shared_ptr<GameObject> GameObject_ptr;

class ENGINE_API GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	Transform* transform;
	std::string name;
	std::vector<Component_ptr> components;
	GameObject(const char* _name = nullptr);
	~GameObject();

	GameObject* GetParent();
	std::vector<GameObject*> GetChildren();
	GameObject* GetChild(int index);

	bool IsActiveInHierarchy();
	void SetActive(bool active);
	void HandleHierarchyChanged();

	void AddComponent(Component_ptr comp);
	void RemoveComponent(Component_ptr comp);
	void EmitComponentEvent(void(*eventFunction)(Component_ptr));
	void StartComponents();
	void StartComponent(Component_ptr comp);
	void UpdateComponents();
	void UpdateComponent(Component_ptr comp);
	void FixedUpdateComponents();
	void FixedUpdateComponent(Component_ptr comp);

	Component_ptr FilterComponent(std::function<bool(Component_ptr)> predicate);
	bool FindComponent(const std::type_info& typeInfo, void** object);
	bool FindComponent(const std::type_info& typeInfo, Component_ptr* object);

	GameObject_ptr GetSelfPtr();
	//static GameObject* Duplicate(GameObject& ref);
	static GameObject_ptr DuplicateSingle(GameObject_ptr ref);
	static GameObject_ptr Duplicate(GameObject_ptr ref);
	static void GetFlattenedHierarchy(GameObject_ptr current, std::vector<GameObject_ptr>& vec);
private:
	friend class Transform;
	bool isActive = true;
	bool parentHierarchyActive = true;
	void HandleEnable();
	void HandleDisable();

	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(isActive);
		ar & BOOST_SERIALIZATION_NVP(transform);
		ar & BOOST_SERIALIZATION_NVP(components);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version) // file_version
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(isActive);
		ar & BOOST_SERIALIZATION_NVP(transform);
		transform->gameObject = this;
		
		ar & BOOST_SERIALIZATION_NVP(components);
		// ToDo: Initialize using add component instead..?
		for (Component_ptr c : components)
		{
			c->gameObject = this;
		}
		if (this->IsActiveInHierarchy())
		{
			HandleEnable();
		}
	}

};



