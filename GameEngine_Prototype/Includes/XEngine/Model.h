#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"

class Model
{
public:
	std::vector<Mesh*> meshes;
	//Material* material;
	// mapping name of mesh to its respective material in the mesh renderer
	std::unordered_map<std::string, Material*> MeshToMaterial;

	Model();
	~Model();

	Mesh* GetMeshByName(std::string name);
};

