#pragma once
#include "RenderableObject.h"
#include "Component.h"
//#include "PrimitiveModels.h"
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
	//friend class boost::serialization::access;
	//template<class Archive>
	//void serialize(Archive &ar, const unsigned int version)
	//{
	//	std::cout << "Serializing SimpleModelComponent." << std::endl;
	//	// save/load base class information
	//	//ar & boost::serialization::base_object<Component>(*this);
	//	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	//	//std::cout << "\tType: ." << std::endl;
	//}
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		std::cout << "Saving SimpleModelComponent." << std::endl;
		//// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(numVerts);
		ar & BOOST_SERIALIZATION_NVP(vertexDataSize);// = vertDataSize;
		ar & BOOST_SERIALIZATION_NVP(numIndices);// = numInd;
		ar & BOOST_SERIALIZATION_NVP(material);
		//ar & BOOST_SERIALIZATION_NVP(indices);// = ind;
		//ar & BOOST_SERIALIZATION_NVP(vertices);// = verts;

		//std::vector<float> v(vertices, vertices + (numVerts * vertexDataSize * sizeof(float)));
		//ar & boost::serialization::make_nvp("vertices", v);
		//
		//std::vector<unsigned int> iVec(indices, indices + (numIndices * sizeof(unsigned int)));
		////std::vector<unsigned int> iVec(indices, indices + (sizeof(indices) / sizeof(indices[0])));
		//ar & boost::serialization::make_nvp("indices", iVec);

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		std::cout << "Loading SimpleModelComponent." << std::endl;
		// invoke serialization of the base class 
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(numVerts);
		ar & BOOST_SERIALIZATION_NVP(vertexDataSize);// = vertDataSize;
		ar & BOOST_SERIALIZATION_NVP(numIndices);// = numInd;
		ar & BOOST_SERIALIZATION_NVP(material);
		//ar & BOOST_SERIALIZATION_NVP(indices);// = ind;
		//ar & BOOST_SERIALIZATION_NVP(vertices);
		
		/*std::cout << "\tLoading Verts..." << std::endl;
		std::vector<float> vertVec(numVerts * vertexDataSize);
		const boost::serialization::nvp<std::vector<float>> nvp = boost::serialization::make_nvp("vertices", vertVec);
		std::cout << "\t\t get NVP" << std::endl;
		ar & nvp;
		vertices = &nvp.value()[0];*/
		//ar & boost::serialization::make_nvp("vertices", vertVec);
		////std::cout << "\t\t get NVP" << std::endl;
		////ar & nvp = boost::serialization::make_nvp("vertices", vertVec);
		//std::cout << "\t\t Converting to Array." << std::endl;
		//vertices = &vertVec[0];
		//std::vector<float> verts = &v[0];

		//std::cout << "\t Loaded Verts" << std::endl;
		//std::cout << "\tLoading Indices..." << std::endl;
		//std::vector<unsigned int> iVec(numIndices);
		//const boost::serialization::nvp<std::vector<unsigned int>> nvp2 = boost::serialization::make_nvp("indices", iVec);
		//std::cout << "\t\t get NVP" << std::endl;
		//ar & nvp2;
		//std::cout << "\t\t getting from archive...." << std::endl;
		//indices = &nvp2.value()[0];
		////nvp2.load();
		////std::vector<float> verts = &v[0];
		//std::cout << "\t Loaded Indices" << std::endl;

		//vertices = CUBE_VERTS;
		//indices = CUBE_INDICES;
		float cube_verts[]{
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0 L Bottom Back
			0.5f, -0.5f, -0.5f,   1.0f, 0.0f, // 1 R Bottom Back
			0.5f,  0.5f, -0.5f,   1.0f, 1.0f, // 2 R Top Back
			0.5f,  0.5f, -0.5f,   1.0f, 1.0f, // 2 R Top Back
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3 L Top Back
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0 L Bottom Back

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,   1.0f, 1.0f, //
			0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,   0.0f, 1.0f, //
			0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,   1.0f, 0.0f, //
			0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,   1.0f, 0.0f, //
			0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		unsigned int cube_indices[]{
			0, 1, 2,	0, 2, 3,      // front
			4, 5, 6,	4, 6, 7,      // back
			8, 9, 10,	8, 10, 11,    // top
			12, 13, 14, 12, 14, 15,   // bottom
			16, 17, 18, 16, 18, 19,   // right
			20, 21, 22, 20, 22, 23,   // left
		};
		vertices = cube_verts;
		indices = cube_indices;
		Setup();
	}
	//template<class Archive>
	//void save_construct_data(Archive & ar, const unsigned int version) const
	//{
	//	std::cout << "Saving SimpleModelComponent." << std::endl;
	//	//// invoke serialization of the base class 
	//	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);

	//	ar & BOOST_SERIALIZATION_NVP(vertices);// = verts;
	//	ar & BOOST_SERIALIZATION_NVP(numVerts);
	//	ar & BOOST_SERIALIZATION_NVP(vertexDataSize);// = vertDataSize;
	//	ar & BOOST_SERIALIZATION_NVP(indices);// = ind;
	//	ar & BOOST_SERIALIZATION_NVP(numIndices);// = numInd;

	//	// save data required to construct instance
	//	ar << t->member;

	//}
	//template<class Archive>
	//void load_construct_data(Archive & ar, const unsigned int version)
	//{
	//	std::cout << "Loading SimpleModelComponent." << std::endl;
	//	// invoke serialization of the base class 
	//	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);

	//	ar & BOOST_SERIALIZATION_NVP(vertices);// = verts;
	//	ar & BOOST_SERIALIZATION_NVP(numVerts);
	//	ar & BOOST_SERIALIZATION_NVP(vertexDataSize);// = vertDataSize;
	//	ar & BOOST_SERIALIZATION_NVP(indices);// = ind;
	//	ar & BOOST_SERIALIZATION_NVP(numIndices);// = numInd;

	//	// retrieve data from archive required to construct new instance
	//	int m;
	//	ar >> m;
	//	// invoke inplace constructor to initialize instance of my_class
	//	::new(t)my_class(m);

	//	Setup();
	//}
};

//BOOST_CLASS_EXPORT_GUID(SimpleModelComponent, "SimpleModelComponent") // Put in cpp