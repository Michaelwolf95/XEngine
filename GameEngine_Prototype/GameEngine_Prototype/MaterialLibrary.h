#pragma once
#include "AssetLibrary.h"
#include "Material.h"

// Note for material for meshes from obj files, use the filepath + name of mesh
class MaterialLibrary : public AssetLibrary<std::string, Material*>
{
public:
	MaterialLibrary();
	~MaterialLibrary();

	void SaveAsset(std::string filePath, Material* material);
	Material*& LoadAsset(std::string filePath) override;
};

