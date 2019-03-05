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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;


class MeshRenderer: public RenderableObject, public Component
{
	public:
		static Registrar<MeshRenderer> registrar;

		//GameObject* gameObject; // The owner of the component.
		vector<Texture> textures_loaded;
		vector<Mesh> meshes;
		string directory;
		bool gammaCorrection;
		
		string pathToObjModel;

		// Constructor
		MeshRenderer(string const &path, Material* m = nullptr, bool gamma = false);

		//MeshRenderer(string const & path, Material * m);

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

	private:
		//Material* _material;

		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
		unsigned int TextureFromFile(const char * path, const string &directory, bool gamma = false);

		friend class boost::serialization::access;
		BOOST_SERIALIZATION_SPLIT_MEMBER()
			template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			//// invoke serialization of the base class 
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(pathToObjModel);

		}
		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			// invoke serialization of the base class 
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
			ar & BOOST_SERIALIZATION_NVP(pathToObjModel);
			Setup();
		}
};