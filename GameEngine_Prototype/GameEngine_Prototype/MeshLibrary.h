#pragma once
#include "AssetLibrary.h"
#include "Mesh.h"

// Note for meshes from obj files, use the filepath + name of mesh
class MeshLibrary: public AssetLibrary<std::string, Mesh*>
{
public:
	MeshLibrary();
	~MeshLibrary();

	void SaveAsset(std::string filePath, Mesh* mesh);
	Mesh*& LoadAsset(std::string filePath) override;
};

