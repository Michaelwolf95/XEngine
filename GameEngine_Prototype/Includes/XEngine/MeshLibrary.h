#pragma once
#include "AssetLibrary.h"
#include "ModelLibrary.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Query used as a key for mapping the mesh library
struct MeshQuery
{
	std::string filePath;
	std::string name;

public :
	MeshQuery(std::string _filePath, std::string _type) : filePath(_filePath), name(_type) {};
	~MeshQuery(){};

	// Default operator used in for comparison inside the library map
	bool operator<(const MeshQuery& query) const
	{
		return std::tie(filePath, name) < std::tie(query.filePath, query.name);
	}

};

// Note for meshes from obj files, use the filepath + name of mesh
class MeshLibrary: public AssetLibrary<MeshQuery, Mesh*>
{
public:
	MeshLibrary();
	~MeshLibrary();

	Mesh*& GetAsset(std::string filepath, std::string name);

protected:
	
	//Mesh*& LoadAsset(MeshQuery meshQ, aiMesh * mesh); 
	Mesh*& LoadAsset(MeshQuery meshQ) override;

	friend class ModelLibrary;
	
private:
	Mesh * processMesh(MeshQuery meshQ);
};

