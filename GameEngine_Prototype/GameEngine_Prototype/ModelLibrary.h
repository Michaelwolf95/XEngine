#pragma once
#include "AssetLibrary.h"
#include "Model.h"

class ModelLibrary : public AssetLibrary<std::string, Model*>
{
public:
	ModelLibrary();
	~ModelLibrary();

	void SaveAsset(std::string filePath, Model* model);
	Model*& LoadAsset(std::string filePath) override;
};

