#include "ModelLibrary.h"
#include "AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>

ModelLibrary::ModelLibrary() {}

ModelLibrary::~ModelLibrary() {}

//void ModelLibrary::SaveAsset(std::string filePath, Model* model)
//{
//	library.insert({ filePath, model });
//	//std::cout << "Model Saved into Library" << std::endl;
//}

Model*& ModelLibrary::LoadAsset(std::string filePath)
{
	Model* model = new Model();
	std::string pathToObjModel = ASSET_FILE_PATH + filePath;
	// read file using ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pathToObjModel, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return model;
	}

	// Save model into model library
	//AssetManager::getInstance().modelLib.SaveAsset(pathToObjModel, model);
	library.insert({ filePath, model });

	//// retrieve the directory path of the filepath
	//std::string directory = pathToObjModel.substr(0, pathToObjModel.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(model, scene->mRootNode, scene, filePath);

	//std::cout << "Model Loaded from Library" << std::endl;
	// get model based on filePath
	//Model* model = library.at(filePath);
	return library[filePath];
}


// process each node and its children node recursively
void ModelLibrary::processNode(Model* model, aiNode *node, const aiScene *scene, std::string filePath)
{
	// process each mesh at current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];

		Mesh* mesh = processMesh(model, ai_mesh, scene, filePath);
		//mesh->mName
		model->meshes.push_back(mesh);

		// save mesh to mesh library
		AssetManager::getInstance().meshLib.ProvideAsset(filePath + model->meshes.back()->name, model->meshes.back());
	}

	// recursively call the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(model, node->mChildren[i], scene, filePath);
	}

}

Mesh* ModelLibrary::processMesh(Model* model, aiMesh *mesh, const aiScene *scene, std::string filePath)
{
	// data of the meshes
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

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
/*
Material * ModelLibrary::processMeshMaterial(Model * model, aiMesh * mesh, const aiScene * scene, std::string filePath)
{
	// process materials
	aiMaterial* aMaterial = scene->mMaterials[mesh->mMaterialIndex];

	// assume sampler names in shaders 
	// such as 'texture_diffuseN' where N is ranging from 1 to MAX_SAMPLER_NUMBER

	// 1.diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(aMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// 2.specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(aMaterial, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3.normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(aMaterial, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4.height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(aMaterial, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// TODO: Load Material from Library instead. Use a struct of name and shader files as a key.

	// mapped name of mesh to the material
	std::string meshMatName = mesh->mName.C_Str();
	meshMatName += "_mat";
	Material* MatforMesh = new Material(meshMatName, "multilights.vs", "multilights.fs");//material;// AssetManager::getInstance().materialLib.GetAsset(material->name);// , material->vertexShaderPath, material->fragmentShaderPath);
	
	
	MatforMesh->textures = textures;
	
	//MeshToMaterial.emplace(mesh->mName.C_Str(), MatforMesh);

	// add material to material library
	AssetManager::getInstance().materialLib.SaveAsset(filePath + mesh->mName.C_Str(), MatforMesh);

	return nullptr;
}


// check material textures of a given type and loads texture if not loaded yet
std::vector<Texture> ModelLibrary::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, std::string filePath)
{
	// retrieve the directory path of the filepath
	std::string directory = filePath.substr(0, filePath.find_last_of('/'));

	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;

		mat->GetTexture(type, i, &str);

		// check if texture was loaded, if so skip
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // texture with same filepath already loaded, continue to next
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);

			// store as texture loaded for entire model
			// so we dont load duplicate textures
			textures_loaded.push_back(texture);
		}
		return textures;
	}
}

*/