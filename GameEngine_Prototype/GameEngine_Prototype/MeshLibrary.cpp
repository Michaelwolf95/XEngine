#include "MeshLibrary.h"
#include "AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MeshLibrary::MeshLibrary() {}
MeshLibrary::~MeshLibrary() {}

// Overloading method: pass filepath and name to create a query to load asset, requires aiMesh to create mesh if not loaded from library
Mesh *& MeshLibrary::GetAsset(std::string filepath, std::string name)
{
	MeshQuery meshQ{ filepath, name };
	auto search = this->library.find(meshQ);
	if (search == this->library.end())
	{
		// Not found.
		//std::cout << "Mesh not found in Library" << std::endl;
		return LoadAsset(meshQ);
	}
	else
	{
		// Found
		//std::cout << "Mesh loaded from Library" << std::endl;
		return this->library[meshQ];
	}
}

// Overloading method for LoadAsset. requires aiMesh
Mesh *& MeshLibrary::LoadAsset(MeshQuery meshQ)
{
	// Loading Mesh from model file.
	std::cout << "Loading Mesh:\n\tPath:" << meshQ.filePath << "\n\tName:" << meshQ.name << std::endl;
	Mesh* loadedMesh = processMesh(meshQ);
	library.insert( {meshQ, loadedMesh} );

	return library[meshQ];
}

//// Overrided method( not commonly used )
//Mesh *& MeshLibrary::LoadAsset(MeshQuery meshQ)
//{
//	return library[meshQ];
//}

// Process mesh from aiMesh
Mesh* MeshLibrary::processMesh(MeshQuery meshQ)
{
	// load model
	Model* model = AssetManager::getInstance().modelLib.GetAsset(meshQ.filePath);
	
	// get mesh by name from model
	return model->GetMeshByName(meshQ.name);
}