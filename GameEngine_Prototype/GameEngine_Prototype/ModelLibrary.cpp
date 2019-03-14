#include "ModelLibrary.h"
#include "AssetManager.h"


ModelLibrary::ModelLibrary()
{
}


ModelLibrary::~ModelLibrary()
{
}

void ModelLibrary::SaveAsset(std::string filePath, Model* model)
{
	library.insert({ filePath, model });
	std::cout << "Model Saved into Library" << std::endl;
}

Model*& ModelLibrary::LoadAsset(std::string filePath)
{
	std::cout << "Model Loaded into Library" << std::endl;
	// get model based on filePath
	Model* model = library.at(filePath);
	return model;
}