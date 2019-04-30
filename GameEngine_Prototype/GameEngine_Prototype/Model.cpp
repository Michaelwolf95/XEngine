#include "Model.h"

Model::Model()
{
}


Model::~Model()
{
}

Mesh* Model::GetMeshByName(std::string name)
{
	Mesh* currentMesh = nullptr;
	bool nameExists = false;

	for(Mesh* mesh : meshes)
	{
		if (mesh->name == name)
		{
			currentMesh = mesh;
			nameExists = true;
		}	
	}

	if (nameExists == false)
		std::cout << "Name of Mesh inside the Model does not exists" << std::endl;

	return currentMesh;
}
