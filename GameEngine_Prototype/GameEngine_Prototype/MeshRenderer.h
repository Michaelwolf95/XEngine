#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "RenderableObject.h"
#include "Component.h"
#include "GameObject.h"
#include "Model.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


class MeshRenderer: public RenderableObject, public Component
{
	public:
		float* vertices;
		unsigned int numVerts;
		unsigned int vertexDataSize;
		unsigned int* indices;
		unsigned int numIndices;
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
		Material* material = nullptr;


		static Registrar<MeshRenderer> registrar;

		Model* model = nullptr; // = new Model();//std::vector<Mesh> meshes;
		bool gammaCorrection;
		std::string pathToObjModel;

		//std::string directory;
		//std::vector<Texture> textures_loaded;

		
		
		// Constructor
		MeshRenderer(std::string const &path, Material* m = nullptr, bool gamma = false);

		MeshRenderer();
		// Deconstructor
		~MeshRenderer();

		// Renderable Object functions
		void Setup() override; 
		void Draw() override;

		// Component functions
		void Start() override;
		void Update() override;
		void OnDrawGizmos() override;

		void DrawInspector() override;

		void FreeObjectResources();
		//void FreeAllResources()

		void PrintVertices();

	private:
		//Material* _material;
		bool isSetup = false;

		bool LoadModel();



		//void processNode(aiNode *node, const aiScene *scene);
		//Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
		//std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		//unsigned int TextureFromFile(const char * path, const std::string &directory, bool gamma = false);

		friend class boost::serialization::access;
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			//// invoke serialization of the base class 
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(pathToObjModel);
			ar & BOOST_SERIALIZATION_NVP(material);

		}
		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			// invoke serialization of the base class 
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(pathToObjModel);
			ar & BOOST_SERIALIZATION_NVP(material);
			Setup();
		}
		unsigned int TextureFromFile(const char * path, const std::string & directory, bool gamma);
};