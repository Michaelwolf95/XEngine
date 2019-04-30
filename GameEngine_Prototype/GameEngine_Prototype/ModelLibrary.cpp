#include "ModelLibrary.h"
#include "AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>
#include "MeshRenderer.h"

ModelLibrary::ModelLibrary() {}

ModelLibrary::~ModelLibrary() {}

// Load asset using the filepath of the obj
Model*& ModelLibrary::LoadAsset(std::string filePath)
{
	std::cout << "ModelLibrary::LoadAsset called with argument\n";
	std::cout << "\tfilePath: " << filePath << std::endl;
	Model* model = new Model();
	std::string pathToObjModel = filePath;

	// read file using ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pathToObjModel, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return model;
	}

	// process ASSIMP's root node recursively
	processNode(model, scene->mRootNode, scene, filePath);

	// Save model into model library
	library.insert({ filePath, model });

	return library[filePath];
}


// Process each node and its children node recursively
void ModelLibrary::processNode(Model* model, aiNode *node, const aiScene *scene, std::string filePath)
{
	// process each mesh at current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
		
		// create mesh and save or load mesh GetAsset(MeshQuery meshQ, aiMesh * mesh)
		Mesh* mesh = AssetManager::getInstance().meshLib.GetAsset(filePath, ai_mesh->mName.C_Str(), ai_mesh);
		
		// add material to material library

		Material* mat = processMeshMaterial(ai_mesh, scene, filePath);

		// put one to one relationship for mesh to material
		model->MeshToMaterial.emplace(mesh->name, mat);

		model->meshes.push_back(mesh);
	}

	// recursively call the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(model, node->mChildren[i], scene, filePath);
	}
}

