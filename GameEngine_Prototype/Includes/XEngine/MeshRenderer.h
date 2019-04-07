//#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <boost/serialization/unordered_map.hpp>

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
		//float* vertices;
		//unsigned int numVerts;
		//unsigned int vertexDataSize;
		//unsigned int* indices;
		//unsigned int numIndices;
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;


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
		void OnEnable() override;
		void OnDisable() override;
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
			
			std::vector<std::string> vecMaterials;
			for (size_t i = 0; i < model->meshes.size(); i++)
			{
				std::cout << model->meshes[i]->name << std::endl;
				AssetManager::getInstance().materialLib.SaveMaterialToFile(*model->MeshToMaterial.at(model->meshes[i]->name), model->MeshToMaterial.at(model->meshes[i]->name)->filePath.c_str());
				vecMaterials.push_back(model->MeshToMaterial.at(model->meshes[i]->name)->filePath);
			}
			ar & boost::serialization::make_nvp<std::vector<std::string>>("meshMaterialFilePaths", vecMaterials);
		}
		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			// invoke serialization of the base class 
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(pathToObjModel);

			std::vector<std::string> vecMaterials;
			ar & boost::serialization::make_nvp<std::vector<std::string>>("meshMaterialFilePaths", vecMaterials);
			for (size_t i = 0; i < vecMaterials.size(); i++)
			{
				AssetManager::getInstance().materialLib.GetAsset(vecMaterials[i]); // load into library
			}

			Setup();
		}
		unsigned int TextureFromFile(const char * path, const std::string & directory, bool gamma);
};