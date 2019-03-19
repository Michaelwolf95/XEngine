#pragma once
#include "AssetLibrary.h"
#include "Material.h"
#include "Scene.h"

struct MaterialQuery
{
	std::string name;
	std::string vertPath;
	std::string fragPath;

public:
	MaterialQuery(std::string _name, std::string _vertPath, std::string _fragPath) : name(_name), vertPath(_vertPath), fragPath(_fragPath) {};
	~MaterialQuery() {};

	// comparison used for map
	bool operator<(const MaterialQuery& query) const
	{
		return std::tie(name, vertPath, fragPath) < std::tie(query.name, query.vertPath, query.fragPath);
	}
};

// Note for material for meshes from obj files, use the filepath + name of mesh
class MaterialLibrary : public AssetLibrary<MaterialQuery, Material*>
{
public:
	MaterialLibrary();
	~MaterialLibrary();

	Material*& GetAsset(std::string filepath, std::string vertPath, std::string fragPath);
	Material*& LoadAsset(MaterialQuery materialQ) override;
	void SaveMaterialToFile(const Material & s);
	void SaveMaterialToFile(const Material & s, const char * fileName);
	bool LoadMaterialFromFileByName(Material &m, const char * materialName);
	bool LoadMaterialFromFile(Material &m, const char * fileName);

protected:
	friend class ModelLibrary;
	//static Material * processMeshMaterial(aiMesh * mesh, const aiScene * scene, std::string filePath);
};

