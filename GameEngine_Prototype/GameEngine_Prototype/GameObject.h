#pragma once
#include <vector>
#include <functional>
#include <typeinfo>
#include "Component.h"
#include "Transform.h"
#include "Serialization.h"

//class GameObject;
typedef std::shared_ptr<GameObject> GameObject_ptr; // Needed because we need an OBJECT.

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	Transform* transform;
	bool isActive = true;
	std::string name;
	std::vector<Component_ptr> components;
	GameObject(const char* _name = nullptr);
	~GameObject();

	std::vector<GameObject*> GetChildren();
	GameObject* GetChild(int index);
	//void SetParent(GameObject_ptr parent);

	//void AddComponent(Component* comp);
	void AddComponent(Component_ptr comp);
	void RemoveComponent(Component_ptr comp);
	void EmitComponentEvent(void(*eventFunction)(Component_ptr));
	void StartComponents();
	void StartComponent(Component_ptr comp);
	void UpdateComponents();
	void UpdateComponent(Component_ptr comp);

	Component_ptr FilterComponent(std::function<bool(Component_ptr)> predicate);
	bool FindComponent(const std::type_info& typeInfo, void** object);

	GameObject_ptr GetSelfPtr();
private:
	// For now, the child hierarchy is stored in the GameObject AS WELL as the transform.
	//friend class Transform;
	//GameObject_ptr parent = nullptr;
	//std::vector<GameObject_ptr> children;
	//void AddChild(GameObject_ptr child);
	//void RemoveChild(GameObject_ptr child);
	
	

	friend std::ostream & operator<<(std::ostream &os, const GameObject &go);
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(transform);
		ar & BOOST_SERIALIZATION_NVP(components);

		//std::cout << "Transform: " << transform << std::endl;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version) // file_version
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(transform);

		// ToDo: Initialize using add component.
		//std::vector<Component_ptr> comps;
		ar & BOOST_SERIALIZATION_NVP(components);

		for (Component_ptr c : components)
		{
			c->gameObject = this;// shared_from_this();
		}
		std::cout << "Transform: " << transform << std::endl;
	}

};


std::ostream & operator<<(std::ostream &os, const GameObject &go);