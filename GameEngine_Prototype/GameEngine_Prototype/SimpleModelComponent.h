#pragma once
#include "RenderableObject.h"
#include "Component.h"
#include "PrimitiveModels.h"
//#include "std/vector"
class SimpleModelComponent :
	public RenderableObject, public Component
{
public:
	static Registrar<SimpleModelComponent> registrar;
	SimpleModelComponent();
	SimpleModelComponent(float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int* ind, unsigned int numInd, Material* _shader = nullptr);
	~SimpleModelComponent();

	void Setup() override;

	void Draw() override;
	void Start() override;
	void Update() override;
	void OnDrawGizmos() override;	
	void DrawInspector() override;
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

		//ar & BOOST_SERIALIZATION_NVP(material);
		ar & boost::serialization::make_nvp<std::string>("materialFilePath", material->filePath);

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(numVerts);
		ar & BOOST_SERIALIZATION_NVP(vertexDataSize);// = vertDataSize;
		ar & BOOST_SERIALIZATION_NVP(numIndices);// = numInd;


		//TODO: Use the filePath instead when getting from the library.
		std::string materialFilePath;
		ar & boost::serialization::make_nvp<std::string>("materialFilePath", materialFilePath);
		material = AssetManager::getInstance().materialLib.GetAsset("TEST_MAT.material");

		vertices = DiffusedMappedCube;
		indices = DiffusedMappedCubeIndices;
		Setup();
	}
};

//BOOST_CLASS_EXPORT_GUID(SimpleModelComponent, "SimpleModelComponent") // Put in cpp