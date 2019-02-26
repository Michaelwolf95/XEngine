#pragma once
#include "RenderableObject.h"
#include "Component.h"
#include "PrimitiveModels.h"
//#include "std/vector"
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
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		//// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(numVerts);
		ar & BOOST_SERIALIZATION_NVP(vertexDataSize);// = vertDataSize;
		ar & BOOST_SERIALIZATION_NVP(numIndices);// = numInd;
		ar & BOOST_SERIALIZATION_NVP(material);
		//ar & BOOST_SERIALIZATION_NVP(indices);// = ind;
		//ar & BOOST_SERIALIZATION_NVP(vertices);// = verts;

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(numVerts);
		ar & BOOST_SERIALIZATION_NVP(vertexDataSize);// = vertDataSize;
		ar & BOOST_SERIALIZATION_NVP(numIndices);// = numInd;
		ar & BOOST_SERIALIZATION_NVP(material);
		//ar & BOOST_SERIALIZATION_NVP(indices);// = ind;
		//ar & BOOST_SERIALIZATION_NVP(vertices);
		vertices = CUBE_VERTS;
		indices = CUBE_INDICES;
		Setup();
	}
};

//BOOST_CLASS_EXPORT_GUID(SimpleModelComponent, "SimpleModelComponent") // Put in cpp