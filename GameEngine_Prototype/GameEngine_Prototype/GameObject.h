#pragma once
#include <vector>
#include <functional>
#include <typeinfo>
#include "Component.h"
#include "Transform.h"
#include "Serialization.h"

class GameObject
{
	typedef Component * Component_ptr; // Needed because we need an OBJECT. ... right?
public:
	Transform* transform;
	bool isActive = true;
	std::string name;
	std::vector<Component_ptr> components;
	GameObject(const char* _name = nullptr);
	~GameObject();
	void AddComponent(Component* comp);
	void RemoveComponent(Component* comp);
	void EmitComponentEvent(void(*eventFunction)(Component*));
	void StartComponents();
	void StartComponent(Component* comp);
	void UpdateComponents();
	void UpdateComponent(Component* comp);

	Component* FilterComponent(std::function<bool(Component*)> predicate);
	bool FindComponent(const std::type_info& typeInfo, void** object);

private:
	friend std::ostream & operator<<(std::ostream &os, const GameObject &go);
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(transform);
		ar & BOOST_SERIALIZATION_NVP(components);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version) // file_version
	{
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(transform);

		// ToDo: Initialize using add component.
		//std::vector<Component_ptr> comps;
		ar & BOOST_SERIALIZATION_NVP(components);

		for (Component* c : components)
		{
			c->gameObject = this;
		}
	}

};

std::ostream & operator<<(std::ostream &os, const GameObject &go);