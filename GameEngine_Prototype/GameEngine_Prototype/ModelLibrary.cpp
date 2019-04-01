#include "ModelLibrary.h"
#include "AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>
#include "MeshRenderer.h"

ModelLibrary::ModelLibrary() {}

ModelLibrary::~ModelLibrary() {}

// Load asset using the filepath of the obj
Model*& ModelLibrary::LoadAsset(std::string filePath)
{
	std::cout << "ModelLibrary::LoadAsset with arguments\n";
	std::cout << "\tfilePath: " << filePath << std::endl;

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
		Mesh* mesh = AssetManager::getInstance().meshLib.GetAsset(filePath, ai_mesh->mName.C_Str(), ai_mesh);
		
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
Material* ModelLibrary::processMeshMaterial(aiMesh * mesh, const aiScene * scene, std::string filePath)
{
	std::cout << "ModelLibrary::processMeshMaterial with arguments\n";
	std::cout << "\tfilePath: " << filePath << std::endl;

	std::string matFilePath = "../Assets/Materials/" + (std::string)mesh->mName.C_Str() + ".material";	//filePath += fileName + ".material";
	// get material
	//std::string meshMatName = ;
	//Material* MatforMesh = AssetManager::getInstance().materialLib.GetAsset(meshMatName, "3Dmodel.vs", "3Dmodel.fs");
	
	// only used name of the material to get it
	Material* MatforMesh = AssetManager::getInstance().materialLib.GetAsset(matFilePath);

	// process materials
	aiMaterial* aMaterial = scene->mMaterials[mesh->mMaterialIndex];

	// such as 'texture_diffuseN' where N is ranging from 1 to MAX_SAMPLER_NUMBER
	std::vector<Texture> textures;

	// 1.diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(aMaterial, aiTextureType_DIFFUSE, "texture_diffuse", filePath);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// 2.specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(aMaterial, aiTextureType_SPECULAR, "texture_specular", filePath);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3.normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(aMaterial, aiTextureType_NORMALS, "texture_normal", filePath);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4.height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(aMaterial, aiTextureType_HEIGHT, "texture_height", filePath);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

//	// TODO: Load Material from Library instead. Use a struct of name and shader files as a key.

//	// mapped name of mesh to the material
//	std::string meshMatName = mesh->mName.C_Str();
//	meshMatName += "_mat";
//	//Material* MatforMesh = new Material(meshMatName, "3Dmodel.vs", "3Dmodel.fs");//material;// AssetManager::getInstance().materialLib.GetAsset(material->name);// , material->vertexShaderPath, material->fragmentShaderPath);
//	Material* MatforMesh = new Material(meshMatName, "multilights.vs", "multilights.fs");//material;// AssetManager::getInstance().materialLib.GetAsset(material->name);// , material->vertexShaderPath, material->fragmentShaderPath);
		
	// save textures in material
	MatforMesh->textures = textures;
	
	return MatforMesh;
}


// Check material textures of a given type and loads texture if not loaded yet
std::vector<Texture> ModelLibrary::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, std::string filePath)
{
	std::cout << "ModelLibrary::loadMaterialTextures with arguments\n";
	std::cout << "\ttypeName: " << typeName << std::endl;
	std::cout << "\tfilePath: " << filePath << std::endl;
	// retrieve the directory path of the filepath
	std::string directory = filePath.substr(0, filePath.find_last_of('/'));

	std::vector<Texture> textures;

	std::vector<Texture> textures_loaded;

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
			texture.id = TextureFromFile(str.C_Str(), directory);
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

// Get the texture from the file
unsigned int ModelLibrary::TextureFromFile(const char * path, const std::string &directory, bool gamma)
{
	std::cout << "ModelLibrary::TextureFromFile with arguments\n";
	std::cout << "\tpath: " << path << std::endl;
	std::cout << "\tdirectory: " << directory << std::endl;
	std::cout << "\tgamma: " << gamma << std::endl;

	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	std::cout << "\tderived filename: " << filename << std::endl;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

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