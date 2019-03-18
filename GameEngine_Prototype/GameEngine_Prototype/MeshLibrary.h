#pragma once
#include "AssetLibrary.h"
#include "ModelLibrary.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct MeshQuery
{
	std::string filePath;
	std::string name;

public :
	MeshQuery(std::string _filePath, std::string _type) : filePath(_filePath), name(_type) {};
	~MeshQuery(){};

	bool operator<(const MeshQuery& query) const
	{
		// if they both string variables are equal
		/*
		if (!filePath.compare(query.filePath) &&
			!name.compare(query.name))
			return true;
		else
			return false;
			*/

		return std::tie(filePath, name) < std::tie(query.filePath, query.name);
	}

};

// Note for meshes from obj files, use the filepath + name of mesh
class MeshLibrary: public AssetLibrary<MeshQuery, Mesh*>
{
public:
	MeshLibrary();
	~MeshLibrary();

protected:
	Mesh*& GetAsset(std::string filepath, std::string name, aiMesh * mesh);

	Mesh*& LoadAsset(MeshQuery meshQ, aiMesh * mesh);
	Mesh*& LoadAsset(MeshQuery meshQ) override;

	// The ModelLibrary is given specific privledges to access the private members of the MeshLibrary
	friend class ModelLibrary;
	// Friend classes can provide an asset directly.
private:
	Mesh * processMesh(aiMesh * mesh);
};

