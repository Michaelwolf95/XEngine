#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "RenderableObject.h"
#include "Component.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;


class MeshRenderer: public RenderableObject,
					public Component
{
	public:
		vector<Texture> textures_loaded;
		vector<Mesh> meshes;
		string directory;
		bool gammaCorrection = false;
		Material* _material;

		// Constructor
		MeshRenderer(string const &path, Material material);

		// Deconstructor
		~MeshRenderer();

		// Renderable Object functions
		void Setup() override; //loadModel
		void Draw() override;

		// Component functions
		void Start() override;
		void Update() override;

	private:
		void loadModel(string const &path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
		unsigned int TextureFromFile(const char * path, const string &directory, bool gamma);

};