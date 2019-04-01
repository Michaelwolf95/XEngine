#pragma once
#include "Serialization.h"
#include <typeindex>
#include <unordered_map>
#include <string>
#include "imgui.h"
//#include "GameObject.h" // Circular dependency - wont compile
class GameObject; // Use a "forward declaration" instead.

struct ComponentTypeInfo;
typedef std::unordered_map<std::type_index, ComponentTypeInfo> typemap;

class Component
{
public:
	static typemap & registry();

	// The owner of the component.
	// Cannot use shared_ptr here due to the way we want things to unload.
	GameObject* gameObject;
	bool enabled = true;
	bool executeInEditMode = false;
	Component();
	virtual ~Component();
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void FixedUpdate() {};
	virtual void OnDestroy() {};

	virtual void OnDrawGizmos() {};
	virtual void OnDrawGizmosSelected() {};

	virtual void DrawInspector() {};

	// Engine Callbacks
	bool isStarted = false;
	void callback_PerformStart();
	void callback_PerformUpdate();
	//void callback_PerformFixedUpdate()
private:

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(enabled);
	}
};

typedef std::shared_ptr<Component> Component_ptr;

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Component)

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

// Registers the component for use in the editor, and enables serialization.
// Note: keep this outside of namespace.
#define REGISTER_COMPONENT(T, K)                                \
BOOST_CLASS_EXPORT_GUID(T, K)                                   \
Registrar<T> T::registrar(ComponentTypeInfo(std::string(K), []() {return (Component_ptr)(new T()); }));    \
/**/

