#pragma once
//#include "GameObject.h" // Circular dependency - wont compile
class GameObject; // Use a "forward declaration" instead.

#include "Serialization.h"
#include <typeindex>
#include <unordered_map>
#include <string>

//typedef std::unordered_map<std::type_index, std::string> typemap;
struct ComponentTypeInfo;
typedef std::unordered_map<std::type_index, ComponentTypeInfo> typemap;


class Component
{
public:
	static typemap & registry();

	GameObject* gameObject; // The owner of the component.
	bool enabled = true;
	bool executeInEditMode = false;
	Component();
	~Component();
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void OnDestroy() {};
	virtual void OnDrawGizmos() {};
private:
	friend class boost::serialization::access;
	friend std::ostream & operator<<(std::ostream &os, const Component &comp);
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(enabled);
	}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Component)
//BOOST_IS_ABSTRACT(Component)
//BOOST_CLASS_EXPORT_GUID(Component) //"Component") // Optional 2nd argument for the name.

std::ostream & operator<<(std::ostream &os, const Component &comp);


struct ComponentTypeInfo
{
public:
	std::string name;
	Component*(*Constructor)(void);
	ComponentTypeInfo();
	ComponentTypeInfo(std::string _name, Component*(*_constructor)());
	//{
	//	name = _name;
	//	Constructor = _constructor;
	//}
};


// https://stackoverflow.com/questions/10589779/enumerating-derived-classes-in-c-executable
template <typename T> struct Registrar
{
	Registrar(ComponentTypeInfo & s)
	{
		Component::registry()[typeid(T)] = s;
	}
	//Registrar(std::string const & s) 
	//{ 
	//	//Component::typemap()[typeid(T)] = s; 
	//	Component::registry()[typeid(T)] = s;
	//}
};

#define REGISTER_COMPONENT(T, K)                                \
BOOST_CLASS_EXPORT_GUID(T, K)                                   \
Registrar<T> T::registrar(ComponentTypeInfo(std::string(K), []() {return (Component*)(new T()); }));    \
/**/

//Registrar<T> T::registrar(K);									\
