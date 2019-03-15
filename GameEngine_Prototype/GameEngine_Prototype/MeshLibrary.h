#pragma once
#include "AssetLibrary.h"
#include "ModelLibrary.h"
#include "Mesh.h"

// Note for meshes from obj files, use the filepath + name of mesh
class MeshLibrary: public AssetLibrary<std::string, Mesh*>
{
public:
	MeshLibrary();
	~MeshLibrary();

protected:
	Mesh*& LoadAsset(std::string filePath) override;

	// The ModelLibrary is given specific privledges to access the private members of the MeshLibrary
	friend class ModelLibrary;
	// Friend classes can provide an asset directly.
	void ProvideAsset(std::string key, Mesh* mesh); // retrieves the obj from lib?
};

