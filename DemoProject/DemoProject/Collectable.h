#pragma once
#include "Component.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;
#include "Rigidbody.h"
#include "AudioComponent.h"

class PROJECT_API Collectable : public Component
{
public:
	static Registrar<Collectable> registrar;
	Collectable();
	~Collectable();
	void Start() override;
	void Update() override;
	void DrawInspector() override;

	void OnCollisionEnter(XEngine::CollisionInfo info);

	AudioComponent* audioComponent = nullptr;

private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.

	}

};
