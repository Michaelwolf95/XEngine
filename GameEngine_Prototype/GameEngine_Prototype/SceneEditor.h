#pragma once
#include "Component.h"
class SceneEditor :
	public Component
{
public:
	SceneEditor();
	~SceneEditor();
	void Start() override;
	void Update() override;

private:
	friend class boost::serialization::access;
	//friend std::ostream & operator<<(std::ostream &os, const Component &comp);
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
};

