#pragma once
#include <vector>
#include <functional>
#include <typeinfo>
#include "Component.h"
#include "Transform.h"
#include "Serialization.h"

class GameObject
{
	typedef Component * Component_ptr; // Needed because we need an OBJECT.
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

	//typedef Component * component_pointer;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int) // file_version
	{
		//ar.register_type(static_cast<Component**>(NULL));
		//ar.register_type(static_cast<GameObject**>(NULL));
		//ar.template register_type<bus_stop_destination>();
		//ar.register_type(static_cast<bus_stop_destination *>(NULL));
		// serialization of stl collections is already defined
		// in the header
		ar & name;
		ar & components;
	}
};

std::ostream & operator<<(std::ostream &os, const GameObject &go);