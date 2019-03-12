#pragma once
#include "Serialization.h"
#include <typeindex>
#include <unordered_map>
#include <string>
#include "imgui.h"
//#include <memory>
//#include "GameObject.h" // Circular dependency - wont compile
class GameObject; // Use a "forward declaration" instead.
//typedef std::shared_ptr<GameObject> GameObject_ptr; // Forward declare typedef?


//typedef std::unordered_map<std::type_index, std::string> typemap;
struct ComponentTypeInfo;
typedef std::unordered_map<std::type_index, ComponentTypeInfo> typemap;

class Component
{
public:
	static typemap & registry();

	// The owner of the component.
	GameObject* gameObject;
	//std::shared_ptr<GameObject> gameObject;
	bool enabled = true;
	bool executeInEditMode = false;
	Component();
	virtual ~Component();
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void OnDestroy() {};
	virtual void OnDrawGizmos() {};
	virtual void DrawInspector() {};
private:
	friend class boost::serialization::access;
	friend std::ostream & operator<<(std::ostream &os, const Component &comp);
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(enabled);
	}
};

typedef std::shared_ptr<Component> Component_ptr; // Needed because we need an OBJECT. ... right?

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Component)
//BOOST_IS_ABSTRACT(Component)
//BOOST_CLASS_EXPORT_GUID(Component) //"Component") // Optional 2nd argument for the name.

std::ostream & operator<<(std::ostream &os, const Component &comp);


struct ComponentTypeInfo
{
public:
	std::string name;
	Component_ptr(*Constructor)(void);
	ComponentTypeInfo();
	ComponentTypeInfo(std::string _name, Component_ptr(*_constructor)());
};


// https://stackoverflow.com/questions/10589779/enumerating-derived-classes-in-c-executable
template <typename T> struct Registrar
{
	Registrar(ComponentTypeInfo & s)
	{
		Component::registry()[typeid(T)] = s;
	}
};

#define REGISTER_COMPONENT(T, K)                                \
BOOST_CLASS_EXPORT_GUID(T, K)                                   \
Registrar<T> T::registrar(ComponentTypeInfo(std::string(K), []() {return (Component_ptr)(new T()); }));    \
/**/

