#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GameObject.h"
#include "RenderManager.h"
#include "RenderableObject.h"
#include "Component.h"
#include "Mesh.h"
#include "Shader.h"
#include "ApplicationManager.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "RenderManager.h"
#include "Model.h"
#include "AssetManager.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "MeshRenderer.h"
#include "AssetManager.h"

REGISTER_COMPONENT(MeshRenderer, "MeshRenderer")

MeshRenderer::MeshRenderer() {}

// Constructor
MeshRenderer::MeshRenderer(std::string const &path, Material* m , bool gamma):
	gammaCorrection(gamma), 
	RenderableObject(m)
{
	// filepath for .obj file.
	this->pathToObjModel = ASSET_FILE_PATH + std::string(path);

	Setup();
}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::Start() {}
void MeshRenderer::Update() {}

void MeshRenderer::Setup()
{
	if (isSetup)
	{
		return;
	}

	render_enabled = true;
	RenderManager::getInstance().AddRenderable((RenderableObject*)this);

	if (material == nullptr)
	{
		material = RenderManager::defaultMaterial;
	}
	std::cout << model << std::endl;
	//model->material = material;
	model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);
	std::cout << model << std::endl;
	if (model == nullptr)
	{
		std::cout << "ERROR LOADING MESH" << std::endl;
		return;
	}
	/*
	if (!model->meshes.empty())
	{
		for (size_t i = 0; i < model->meshes.size(); i++)
		{
			model->MeshToMaterial[model->meshes[i]->name] = material;
		}
	}
	*/
	isSetup = true;
}

void MeshRenderer::Draw()
{
	//if (gameObject == nullptr) return;
	
	// This was the error
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	material->shader->setMat4("view", view);
	material->shader->setMat4("projection", projection);

	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	material->shader->setMat4("model", this->gameObject->transform->getMatrix4x4());
	
	// for mesh name
	std::string meshMatName = "_mat";

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		// name key for material
		std::string MatKey = model->meshes[i]->name + meshMatName;

		// texture variables
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		// binding textures
		for (unsigned int j = 0; j < model->MeshToMaterial.at(MatKey)->textures.size(); j++)
		{
			// get texture before binding
			glActiveTexture(GL_TEXTURE0 + j);

			std::string number;
			std::string name = model->MeshToMaterial.at(MatKey)->textures[j].type;

			// transfer unsigned to stream
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			// set texture unit
			glUniform1i(glGetUniformLocation(material->shader->ID, (name + number).c_str()), j);

			// bind texture
			glBindTexture(GL_TEXTURE_2D, model->MeshToMaterial.at(MatKey)->textures[j].id);
		}

		// draw mesh
		glBindVertexArray(model->meshes[i]->VAO);
		glDrawElements(GL_TRIANGLES, model->meshes[i]->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// default once configured
		glActiveTexture(GL_TEXTURE0);

	}
}

bool MeshRenderer::LoadModel()
{
	model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);
	return (model == nullptr);
}

//bool MeshRenderer::LoadModel()
//{
//	model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);
//
//	// if model is not found in model library.
//	if (AssetManager::getInstance().modelLib.library.find(pathToObjModel) == AssetManager::getInstance().modelLib.library.end())
//	{
//		std::cout << "Model not found in Model Library" << std::endl;
//
//		// read file using ASSIMP
//		Assimp::Importer importer;
//		const aiScene* scene = importer.ReadFile(pathToObjModel, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//
//		// Save model into model library
//		AssetManager::getInstance().modelLib.SaveAsset(pathToObjModel, model);
//
//		// check for errors
//		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//		{
//			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
//			return false;
//		}
//		// retrieve the directory path of the filepath
//		directory = pathToObjModel.substr(0, pathToObjModel.find_last_of('/'));
//
//		// process ASSIMP's root node recursively
//		processNode(scene->mRootNode, scene);
//
//	}
//	else // if model is found
//	{
//		std::cout << "Model is found in Model Library" << std::endl;
//
//		model = AssetManager::getInstance().modelLib.LoadAsset(pathToObjModel);
//
//		// get material for the meshes
//		for (int i = 0; i < model->meshes.size(); i++)
//		{
//			// load material from lib
//			Material* materialInLib = AssetManager::getInstance().materialLib.LoadAsset(pathToObjModel + model->meshes[i]->name);
//
//			// save material in MeshRenderer
//			MeshToMaterial.emplace(model->meshes[i]->name, materialInLib);
//		}
//	}
//
//	//std::cout << "Model Library Size:" << AssetManager::getInstance().modelLib.library.size() << std::endl;
//	//std::cout << "Material Model Library Size:" << AssetManager::getInstance().materialLib.library.size() << std::endl;
//	//std::cout << "Mesh Model Library Size:" << AssetManager::getInstance().meshLib.library.size() << std::endl;
//
//	return true;
//}
//
//// process each node and its children node recursively
//void MeshRenderer::processNode(aiNode *node, const aiScene *scene)
//{
//	// process each mesh at current node
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//
//		//mesh->mName
//		model->meshes.push_back(processMesh(mesh, scene));
//
//		// save mesh to mesh library
//		AssetManager::getInstance().meshLib.SaveAsset( pathToObjModel + model->meshes.back()->name, model->meshes.back());
//	}
//
//	// recursively call the children nodes
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		processNode(node->mChildren[i], scene);
//	}
//
//}
//
//Mesh* MeshRenderer::processMesh(aiMesh *mesh, const aiScene *scene)
//{
//	// data of the meshes
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//	std::vector<Texture> textures;
//
//	// Walk through each of the mesh's vertices
//	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//	{
//		Vertex vertex;
//
//		// placeholder vector because assimp's vector class does not directly translate to glm's vector class
//		glm::vec3 vector;
//
//		// position of vector
//		vector.x = mesh->mVertices[i].x;
//		vector.y = mesh->mVertices[i].y;
//		vector.z = mesh->mVertices[i].z;
//		vertex.Position = vector;
//
//		// normal of vector
//		if (mesh->HasNormals())
//		{
//			vector.x = mesh->mNormals[i].x;
//			vector.y = mesh->mNormals[i].y;
//			vector.z = mesh->mNormals[i].z;
//			vertex.Normal = vector;
//		}
//
//		// texture coordinates
//		if (mesh->mTextureCoords[0])
//		{
//			glm::vec2 vec;
//
//			// vertex can hold up to 8 texture coordinates
//			// under assumption, we won't use models where the vertex can have multiple texture coordinates
//			vec.x = mesh->mTextureCoords[0][i].x;
//			vec.y = mesh->mTextureCoords[0][i].y;
//			vertex.TexCoords = vec;
//		}
//		else
//			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//
//
//		// tangent of vector
//		if (mesh->HasTangentsAndBitangents())
//		{
//			vector.x = mesh->mTangents[i].x;
//			vector.y = mesh->mTangents[i].y;
//			vector.z = mesh->mTangents[i].z;
//			vertex.Tangent = vector;
//
//			// bitangent of vector
//			vector.x = mesh->mBitangents[i].x;
//			vector.y = mesh->mBitangents[i].y;
//			vector.z = mesh->mBitangents[i].z;
//			vertex.Bitangent = vector;
//		}
//
//		vertices.push_back(vertex);
//	}
//		
//	// walk through mesh' face(mesh's trangles) and get corresponding vertex indices
//	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//
//		// get all indices of face and store in indices vector
//		for (unsigned int j = 0; j < face.mNumIndices; j++)
//			indices.push_back(face.mIndices[j]);
//	}
//
//	// process materials
//	aiMaterial* aMaterial = scene->mMaterials[mesh->mMaterialIndex];
//
//	// assume sampler names in shaders 
//	// such as 'texture_diffuseN' where N is ranging from 1 to MAX_SAMPLER_NUMBER
//
//	// 1.diffuse maps
//	std::vector<Texture> diffuseMaps = loadMaterialTextures(aMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//		
//	// 2.specular maps
//	std::vector<Texture> specularMaps = loadMaterialTextures(aMaterial, aiTextureType_SPECULAR, "texture_specular");
//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//
//	// 3.normal maps
//	std::vector<Texture> normalMaps = loadMaterialTextures(aMaterial, aiTextureType_HEIGHT, "texture_normal");
//	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//
//	// 4.height maps
//	std::vector<Texture> heightMaps = loadMaterialTextures(aMaterial, aiTextureType_AMBIENT, "texture_height");
//	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
//
//	// mapped name of mesh to the material
//	Material* MatforMesh = material;// AssetManager::getInstance().materialLib.GetAsset(material->name);// , material->vertexShaderPath, material->fragmentShaderPath);
//	MatforMesh->textures = textures;
//	MeshToMaterial.emplace(mesh->mName.C_Str(), MatforMesh);
//	// add material to material library
//	AssetManager::getInstance().materialLib.SaveAsset( pathToObjModel + mesh->mName.C_Str(), MatforMesh);
//
//	// return mesh object from extracted mesh data
//	return new Mesh(mesh->mName.C_Str(), vertices, indices);// :RenderableObject();
//}
//
//// check material textures of a given type and loads texture if not loaded yet
//std::vector<Texture> MeshRenderer::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
//{
//	std::vector<Texture> textures;
//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		
//		mat->GetTexture(type, i, &str);
//
//		// check if texture was loaded, if so skip
//		bool skip = false;
//		for (unsigned int j = 0; j < textures_loaded.size(); j++)
//		{
//			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
//			{
//				textures.push_back(textures_loaded[j]);
//				skip = true; // texture with same filepath already loaded, continue to next
//				break;
//			}
//		}
//		if (!skip)
//		{
//			Texture texture;
//			texture.id = TextureFromFile(str.C_Str(), this->directory);
//			texture.type = typeName;
//			texture.path = str.C_Str();
//			textures.push_back(texture);
//
//			// store as texture loaded for entire model
//			// so we dont load duplicate textures
//			textures_loaded.push_back(texture);
//		}
//		return textures;
//	}
//}
//
//unsigned int MeshRenderer::TextureFromFile(const char * path, const std::string &directory, bool gamma)
//{
//	std::string filename = std::string(path);
//	filename = directory + '/' + filename;
//
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//
//	if (data)
//	{
//		GLenum format;
//
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//	return textureID;
//}

void MeshRenderer::OnDrawGizmos()
{
	//gameObject->transform->DrawGizmo();
}

void MeshRenderer::DrawInspector()
{
	ImGui::InputText("ModelPath", &this->pathToObjModel[0], 48);
	if(this->material != nullptr)
		this->material->DrawInspector();
	if (!model->meshes.empty())
	{
		for (size_t i = 0; i < model->meshes.size(); i++)
		{
			ImGui::Text(model->meshes[i]->name.c_str());
			model->MeshToMaterial.at(model->meshes[i]->name+"_mat")->DrawInspector();
		}
	}

	if (ImGui::Button("Change Model"))
	{
		model->MeshToMaterial.clear();
		model->meshes.clear();
		model = nullptr;

		LoadModel();
	}
}