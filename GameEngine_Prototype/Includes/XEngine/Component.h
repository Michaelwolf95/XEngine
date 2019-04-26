#pragma once
//#include "XEngine.h"
#include "LibraryExport.h"
#include "Serialization.h"
#include <typeindex>
#include <unordered_map>
#include <string>
#include "imgui.h"

//#include "GameObject.h" // Circular dependency - wont compile
class GameObject; // Use a "forward declaration" instead.

struct ComponentTypeInfo;
typedef std::unordered_map<std::type_index, ComponentTypeInfo> typemap;

// Defining the components for DLL export bypasses issues where 
// the static library wont compile unused classes.
// We need them to compile for registration.
// https://stackoverflow.com/questions/873731/object-registration-in-static-library
//define ENGINE_API __declspec(dllexport)

class ENGINE_API Component
{
public:
	static typemap & registry();

	static void PrintRegistry();

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

	virtual void OnEnable() {};
	virtual void OnDisable() {};

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

struct ENGINE_API ComponentTypeInfo
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
		//ComponentTypeInfo* r = &Component::registry()[typeid(T)];
		auto got = Component::registry().find(typeid(T));
		if (got != Component::registry().end())
		{
			std::cout << "Registrar already exists.\n";
		}

		Component::registry()[typeid(T)] = s;
	}
};

// Registers the component for use in the editor, and enables serialization.
// Note: keep this outside of namespace.
#define REGISTER_COMPONENT(T, K)                                \
BOOST_CLASS_EXPORT_GUID(T, K)                                   \
Registrar<T> T::registrar(*(new ComponentTypeInfo(std::string(K), []() {return (Component_ptr)(new T()); })));    \
/**/

// Potential shortcut for component class declarations.
// Probably annoying because of the brace.
#define COMPONENT_CLASS(className)				\
class ENGINE_API className : public Component {	\
public:											\
static Registrar<className> registrar;			\
/**/