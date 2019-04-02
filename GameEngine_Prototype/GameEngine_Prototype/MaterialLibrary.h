#pragma once
#include "AssetLibrary.h"
#include "Material.h"
#include "Scene.h"

/*
// Query used as a key for mapping the mesh library
struct MaterialQuery
{
	std::string name;
	std::string vertPath;
	std::string fragPath;

public:
	MaterialQuery(std::string _name, std::string _vertPath, std::string _fragPath) : name(_name), vertPath(_vertPath), fragPath(_fragPath) {};
	~MaterialQuery() {};

	// Default operator used in for comparison inside the library map
	bool operator<(const MaterialQuery& query) const
	{
		return std::tie(name, vertPath, fragPath) < std::tie(query.name, query.vertPath, query.fragPath);
	}
};
*/
// Note for material for meshes from obj files, use the filepath + name of mesh

class MaterialLibrary : public AssetLibrary<std::string, Material*>
{
public:
	MaterialLibrary();
	~MaterialLibrary();

	//void SaveAllMaterials();

	Material*& GetAsset(std::string fileName);
	Material*& LoadAsset(std::string fileName) override;

	//void SaveMaterialToFile(const Material & s);
	void SaveMaterialToFile(const Material & s, const char * filePath);
	//bool LoadMaterialFromFileByName(Material &m, const char * materialName);
	bool LoadMaterialFromFile(Material &m, const char * filePath);

protected:
	friend class ModelLibrary;
};

