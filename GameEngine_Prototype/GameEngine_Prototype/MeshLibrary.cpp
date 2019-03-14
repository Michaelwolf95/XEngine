#include "MeshLibrary.h"

MeshLibrary::MeshLibrary() {}
MeshLibrary::~MeshLibrary() {}

void MeshLibrary::ProvideAsset(std::string filePath, Mesh * mesh)
{
	library.insert({ filePath, mesh });
	//std::cout << "Mesh Saved into Library" << std::endl;
}

Mesh *& MeshLibrary::LoadAsset(std::string filePath)
{
	//std::cout << "Mesh Loaded from Library" << std::endl;
	Mesh* mesh = library.at(filePath);
	return mesh;
}
