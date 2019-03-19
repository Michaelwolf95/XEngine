#include "MeshLibrary.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MeshLibrary::MeshLibrary() {}
MeshLibrary::~MeshLibrary() {}

Mesh *& MeshLibrary::GetAsset(std::string filepath, std::string name, aiMesh * mesh)
{
	MeshQuery meshQ{ filepath, name };
	auto search = this->library.find(meshQ);
	if (search == this->library.end())
	{
		// Not found.
		std::cout << "Mesh not found in Library" << std::endl;
		return LoadAsset(meshQ, mesh);
	}
	else
	{
		// Found
		std::cout << "Mesh loaded from Library" << std::endl;
		return this->library[meshQ];
	}
}

// New LoadAsset to work with struct MeshQuery
Mesh *& MeshLibrary::LoadAsset(MeshQuery meshQ, aiMesh * mesh)
{
	Mesh* loadedMesh = processMesh(mesh);

	library.insert( {meshQ, loadedMesh} );
	std::cout << "Mesh saved into Library" << std::endl;
	return library[meshQ];
}


Mesh *& MeshLibrary::LoadAsset(MeshQuery meshQ)
{
	return library[meshQ];
}

Mesh* MeshLibrary::processMesh(aiMesh *mesh)
{
	// data of the meshes
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// placeholder vector because assimp's vector class does not directly translate to glm's vector class
		glm::vec3 vector;

		// position of vector
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// normal of vector
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			// vertex can hold up to 8 texture coordinates
			// under assumption, we won't use models where the vertex can have multiple texture coordinates
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);


		// tangent of vector
		if (mesh->HasTangentsAndBitangents())
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// bitangent of vector
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}

		vertices.push_back(vertex);
	}

	// walk through mesh' face(mesh's trangles) and get corresponding vertex indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		// get all indices of face and store in indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// return mesh object from extracted mesh data
	return new Mesh(mesh->mName.C_Str(), vertices, indices);// :RenderableObject();
}