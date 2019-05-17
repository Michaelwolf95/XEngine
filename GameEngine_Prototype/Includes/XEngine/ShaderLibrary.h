#pragma once
#include "AssetLibrary.h"
#include "Shader.h"

// Query used as a key for mapping the mesh library
struct ShaderQuery 
{
	std::string vs;
	std::string fs;

public:
	ShaderQuery(std::string _vs, std::string _fs) : vs(_vs), fs(_fs){};
	~ShaderQuery() {};

	// Default operator used in for comparison inside the library map
	bool operator<(const ShaderQuery& query) const
	{
		return std::tie(vs, fs) < std::tie(query.vs, query.fs);
	}
};

class ENGINE_API ShaderLibrary : public AssetLibrary<ShaderQuery, Shader*>
{
public:
	ShaderLibrary();
	~ShaderLibrary();

	Shader* GetAsset(std::string vs, std::string fs);
	
protected:
	Shader*& LoadAsset(ShaderQuery query) override;
};

