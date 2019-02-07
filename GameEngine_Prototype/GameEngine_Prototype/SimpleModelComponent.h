#pragma once
#include "RenderableObject.h"
#include "Component.h"
class SimpleModelComponent :
	public RenderableObject, public Component
{
public:
	SimpleModelComponent();
	SimpleModelComponent(float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int* ind, unsigned int numInd, Material* _shader = nullptr);
	~SimpleModelComponent();

	void Setup() override;

	void Draw() override;
	void Start() override;
	void Update() override;
	void OnDrawGizmos() override;	
private:
	bool isSetup = false;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		std::cout << "Serializing SimpleModelComponent." << std::endl;
		// save/load base class information
		//ar & boost::serialization::base_object<Component>(*this);
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		//std::cout << "\tType: ." << std::endl;
	}
};

//BOOST_CLASS_EXPORT_GUID(SimpleModelComponent, "SimpleModelComponent") // Put in cpp