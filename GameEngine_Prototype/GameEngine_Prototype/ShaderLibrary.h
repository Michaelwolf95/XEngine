#pragma once
#include "AssetLibrary.h"
#include "Shader.h"

struct ShaderQuery {
	std::string vs;
	std::string fs;

public:
	ShaderQuery(std::string _vs, std::string _fs) : vs(_vs), fs(_fs){};
	~ShaderQuery() {};

	// comparison used for map
	bool operator<(const ShaderQuery& query) const
	{
		return std::tie(vs, fs) < std::tie(query.vs, query.fs);
	}
};

class ShaderLibrary : public AssetLibrary<ShaderQuery, Shader*>
{
public:
	Shader* GetAsset(std::string vs, std::string fs);
	
protected:
	Shader*& LoadAsset(ShaderQuery query) override;
};

