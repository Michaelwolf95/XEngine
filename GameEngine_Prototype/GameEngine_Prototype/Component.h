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
	void serialize(Archive &ar, const unsigned int version);
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Component)

std::ostream & operator<<(std::ostream &os, const Component &comp);