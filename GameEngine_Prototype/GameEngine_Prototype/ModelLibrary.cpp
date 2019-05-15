#include "ModelLibrary.h"
#include "AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>
#include "MeshRenderer.h"
#include "GameObject.h"
#include <string>

#include "Scene.h"
#include "SceneManager.h"
#include "Serialization.h"
using namespace XEngine;

ModelLibrary::ModelLibrary() {}

ModelLibrary::~ModelLibrary() {}

// Get model with multiple MeshRenderer in GameObject tree form
GameObject_ptr ModelLibrary::GenerateModelGameObject(std::string filePath)
{
	// read file using ASSIMP
	Assimp::Importer importer;

	if (filePath.find(ASSET_FILE_PATH) == std::string::npos)
	{
		filePath = ASSET_FILE_PATH + filePath;
	}

	//std::cout << "WHHHHHHHHHHHHHHHHHHHHHEEEEEEEEEERRRRRREEEEEEEEEEEEEEE" + filePath << std::endl;

	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	// process ASSIMP's root node recursively
	GameObject_ptr rootGameObj = processNodeMeshRenderer(scene->mRootNode, scene, filePath);

	return rootGameObj;
}

// recursive function to get the node with meshrenderer
GameObject_ptr ModelLibrary::processNodeMeshRenderer(aiNode *node, const aiScene *scene, std::string filePath)
{
	// get active scene
	Scene_ptr gameScene = SceneManager::getInstance().GetActiveScene();

	// make node game obj 
	GameObject_ptr nodeGameObj = gameScene->CreateGameObject(node->mName.C_Str());

	// process each MeshRenderer at current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];

		// set file paths
		std::string meshPath = filePath + "|" + ai_mesh->mName.C_Str();
		std::string materialPath = "Materials/" + (std::string)ai_mesh->mName.C_Str() + ".material";

		// make MeshRenderer
		std::shared_ptr<XEngine::MeshRenderer> nodeMeshRenderer(new XEngine::MeshRenderer(meshPath, materialPath));

		// If the node has multiple meshes, generate a child gameobject for it.
		if (i > 0)
		{
			GameObject_ptr childMeshObject = gameScene->CreateGameObject(ai_mesh->mName.C_Str());
			childMeshObject->AddComponent(nodeMeshRenderer);
			childMeshObject->transform->SetParent(nodeGameObj->transform);
		}
		else
		{
			// attach MeshRenderer to node game obj
			nodeGameObj->AddComponent(nodeMeshRenderer);
		}
	}

	// recursively call the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// set child node game obj
		GameObject_ptr childGameObj = processNodeMeshRenderer(node->mChildren[i], scene, filePath);
		childGameObj->transform->SetParent(nodeGameObj->transform);
	}

	return nodeGameObj;
}


// Load asset using the filepath of the obj
Model*& ModelLibrary::LoadAsset(std::string filePath)
{
	std::cout << "Loading Model: " << filePath << std::endl;
	//std::cout << "\tfilePath: " << filePath << std::endl;
	Model* model = new Model();
	std::string pathToObjModel = filePath;

	// read file using ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pathToObjModel, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return model;
	}

	// process ASSIMP's root node recursively
	processNode(model, scene->mRootNode, scene, filePath);

	// Save model into model library
	library.insert({ filePath, model });

	return library[filePath];
}


// Process each node and its children node recursively
void ModelLibrary::processNode(Model* model, aiNode *node, const aiScene *scene, std::string filePath)
{
	// process each mesh at current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
		
		// create mesh and save or load mesh GetAsset(MeshQuery meshQ, aiMesh * mesh)
		Mesh* mesh = processMesh(ai_mesh);
		
		// add material to material library

		Material* mat = processMeshMaterial(ai_mesh, scene, filePath);

		// put one to one relationship for mesh to material
		model->MeshToMaterial.emplace(mesh->name, mat);

		model->meshes.push_back(mesh);
	}

	// recursively call the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(model, node->mChildren[i], scene, filePath);
	}

}

// Process the material for the mesh
Material* ModelLibrary::processMeshMaterial(aiMesh * mesh, const aiScene * scene, std::string filePath) // path to obj file.
{
	//std::cout << "ModelLibrary.processMeshMaterial called with arguments\n";
	//std::cout << "\tfilePath: " << filePath << std::endl;
	std::string matFilePath = ASSET_FILE_PATH + "Materials/" + (std::string)mesh->mName.C_Str() + ".material";	//filePath += fileName + ".material";
	// get material
	//std::string meshMatName = ;
	//Material* MatforMesh = AssetManager::getInstance().materialLib.GetAsset(meshMatName, "3Dmodel.vs", "3Dmodel.fs");
	
	// only used name of the material to get it (not anymore)
	//std::cout << "\nmatFilePath (fileName): " << matFilePath << std::endl;
	Material* MatforMesh = AssetManager::getInstance().materialLib.GetAsset(matFilePath);

	// if the loaded material does not have the texture properties then add it
	if (MatforMesh->textureProperties.empty())
	{
		// process materials
		aiMaterial* aMaterial = scene->mMaterials[mesh->mMaterialIndex];

		// 1.diffuse maps
		std::vector<TextureProperty> diffuseMaps = loadMaterialTextures(aMaterial, aiTextureType_DIFFUSE, "texture_diffuse", filePath);
		MatforMesh->textureProperties.insert(MatforMesh->textureProperties.end(), diffuseMaps.begin(), diffuseMaps.end());

		// 2.specular maps
		std::vector<TextureProperty> specularMaps = loadMaterialTextures(aMaterial, aiTextureType_SPECULAR, "texture_specular", filePath);
		MatforMesh->textureProperties.insert(MatforMesh->textureProperties.end(), specularMaps.begin(), specularMaps.end());

		// 3.normal maps
		std::vector<TextureProperty> normalMaps = loadMaterialTextures(aMaterial, aiTextureType_NORMALS, "texture_normal", filePath);
		MatforMesh->textureProperties.insert(MatforMesh->textureProperties.end(), normalMaps.begin(), normalMaps.end());

		// 4.height maps
		std::vector<TextureProperty> heightMaps = loadMaterialTextures(aMaterial, aiTextureType_HEIGHT, "texture_height", filePath);
		MatforMesh->textureProperties.insert(MatforMesh->textureProperties.end(), heightMaps.begin(), heightMaps.end());
	}

	return MatforMesh;
}

// Process mesh from aiMesh
Mesh* ModelLibrary::processMesh(aiMesh * mesh)
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


// Check material textures of a given type and loads texture if not loaded yet
std::vector<TextureProperty> ModelLibrary::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, std::string filePath)
{
	//std::cout << "ModelLibrary::loadMaterialTextures called with arguments\n";
	//std::cout << "\ttypeName: " << typeName << std::endl;
	//std::cout << "\tfilePath: " << filePath << std::endl;

	// retrieve the directory path of the filepath
	std::string directory = filePath.substr(0, filePath.find_last_of('/'));

	std::vector<TextureProperty> textures;

	std::vector<TextureProperty> textures_loaded;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;

		mat->GetTexture(type, i, &str);

		// check if texture was loaded, if so skip
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].getValue()->path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // texture with same filepath already loaded, continue to next
				break;
			}
		}
		if (!skip)
		{
			// get Texture
			std::string filename = str.C_Str();
			filename = directory + '/' + filename;

			Texture* texture = (AssetManager::getInstance().textureLib.GetAsset(filename));
			texture->id = TextureFromFile(filename.c_str(), directory);
			texture->type = typeName;

			// turn into textureProperty
			TextureProperty textureProp;
			textureProp.setValue(texture);

			textures.push_back(textureProp);

			// store as texture loaded for entire model
			// so we dont load duplicate textures
			textures_loaded.push_back(textureProp);
		}
	}
	return textures;
}

// Get the texture from the file
unsigned int ModelLibrary::TextureFromFile(const char * path, const std::string &directory, bool gamma)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	//std::cout << "3. filename == " << path << std::endl;

	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;

		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}