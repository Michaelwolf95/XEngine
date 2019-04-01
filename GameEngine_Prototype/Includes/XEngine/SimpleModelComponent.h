#pragma once
#include "RenderableObject.h"
#include "Component.h"
#include "PrimitiveModels.h"
//#include "std/vector"
class SimpleModelComponent : public RenderableObject, public Component
{
public:
	static Registrar<SimpleModelComponent> registrar;
	SimpleModelComponent();
	SimpleModelComponent(std::string name, float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int* ind, unsigned int numInd, Material* _shader = nullptr);
	~SimpleModelComponent();

	void Setup() override;

	void Draw() override;
	void Start() override;
	void Update() override;
	void OnDrawGizmos() override;	
	void DrawInspector() override;
private:
	std::string name;
	// taken from RenderableObject
	float* vertices;
	unsigned int numVerts;
	unsigned int vertexDataSize;
	unsigned int* indices;
	unsigned int numIndices;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	Material* material = nullptr;
	//

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
		ar & BOOST_SERIALIZATION_NVP(VBO);
		ar & BOOST_SERIALIZATION_NVP(VAO);
		ar & BOOST_SERIALIZATION_NVP(EBO);
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(gameObject);

		// save the material using the material file path
		ar & boost::serialization::make_nvp<std::string>("materialFilePath", material->filePath);
		AssetManager::getInstance().materialLib.SaveMaterialToFile(*material, material->filePath.c_str());

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(numVerts);
		ar & BOOST_SERIALIZATION_NVP(vertexDataSize);// = vertDataSize;
		ar & BOOST_SERIALIZATION_NVP(numIndices);// = numInd;
		ar & BOOST_SERIALIZATION_NVP(VBO);
		ar & BOOST_SERIALIZATION_NVP(VAO);
		ar & BOOST_SERIALIZATION_NVP(EBO);
		ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(gameObject);


		// load the material using the material file path
		std::string materialFilePath;
		ar & boost::serialization::make_nvp<std::string>("materialFilePath", materialFilePath);
		std::cout << "materialFilePath == " << materialFilePath << std::endl;
		material = AssetManager::getInstance().materialLib.GetAsset(materialFilePath);

		vertices = DiffusedMappedCube;
		indices = DiffusedMappedCubeIndices;
		Setup();
	}
};

//BOOST_CLASS_EXPORT_GUID(SimpleModelComponent, "SimpleModelComponent") // Put in cpp