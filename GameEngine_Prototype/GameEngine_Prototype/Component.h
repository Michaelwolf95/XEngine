#pragma once
//#include "GameObject.h" // Circular dependency - wont compile
class GameObject; // Use a "forward declaration" instead.

#include "Serialization.h"

class Component
{
public:
	GameObject* gameObject; // The owner of the component.
	bool enabled = true;
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
