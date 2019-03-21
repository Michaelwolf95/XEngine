#pragma once
#include "AssetLibrary.h"
#include "Model.h"
#include "MeshLibrary.h"
#include "Mesh.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelLibrary : public AssetLibrary<std::string, Model*>
{
public:
	ModelLibrary();
	~ModelLibrary();

protected:
	Model*& LoadAsset(std::string filePath) override;

private:
	void processNode(Model* model, aiNode *node, const aiScene *scene, std::string filePath);
	Material * processMeshMaterial(aiMesh * mesh, const aiScene * scene, std::string filePath);
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, std::string filePath);
	unsigned int TextureFromFile(const char * path, const std::string &directory, bool gamma = false);
};

