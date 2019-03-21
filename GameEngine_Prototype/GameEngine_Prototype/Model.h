#pragma once
#include<vector>
#include"Mesh.h"
#include"Material.h"

class Model
{
public:
	std::vector<Mesh*> meshes;
	//Material* material;
	// mapping name of mesh to its respective material in the mesh renderer
	std::unordered_map<std::string, Material*> MeshToMaterial;

	Model();
	~Model();
};

