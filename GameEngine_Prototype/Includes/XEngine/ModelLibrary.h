#pragma once
#include "AssetLibrary.h"
#include "Model.h"
#include "MeshLibrary.h"
#include "Mesh.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "GameObject.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Scene.h"

class ENGINE_API ModelLibrary : public AssetLibrary<std::string, Model*>
{
public:
	ModelLibrary();
	~ModelLibrary();

	GameObject_ptr GenerateModelGameObject(std::string filePath);
	GameObject_ptr processNodeMeshRenderer(aiNode *node, const aiScene *scene, std::string filePath);

protected:
	Model*& LoadAsset(std::string filePath) override;

private:
	void processNode(Model* model, aiNode *node, const aiScene *scene, std::string filePath);
	Material * processMeshMaterial(aiMesh * mesh, const aiScene * scene, std::string filePath);
	Mesh* processMesh(aiMesh * mesh);
	std::vector<TextureProperty> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, std::string filePath);
	unsigned int TextureFromFile(const char * path, const std::string &directory, bool gamma = false);
};