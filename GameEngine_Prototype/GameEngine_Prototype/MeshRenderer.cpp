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
#include "CameraComponent.h"

REGISTER_COMPONENT(MeshRenderer, "MeshRenderer")

MeshRenderer::MeshRenderer() {}

// Constructor
MeshRenderer::MeshRenderer(std::string const &path, Material* m , bool gamma): gammaCorrection(gamma)
{
	// filepath for .obj file.
	this->pathToObjModel = "../Assets/" + std::string(path);
	
	Setup();
}

MeshRenderer::~MeshRenderer() 
{
	OnDisable();
}

void MeshRenderer::Start() {}
void MeshRenderer::Update() {}

void MeshRenderer::OnEnable()
{
	this->render_enabled = true;
	RenderManager::getInstance().AddRenderable((RenderableObject*)this);
}

void MeshRenderer::OnDisable()
{
	this->render_enabled = false;
	RenderManager::getInstance().RemoveRenderable((RenderableObject*)this);
}

void MeshRenderer::Setup()
{
	if (isSetup)
	{
		return;
	}
	// read file using ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pathToObjModel, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
	}

	aiNode* node = scene->mRootNode;

	// find mesh recursively
	// process each mesh at current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++) //node is aiScene root node
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
	
	//if (this->gameObject->IsActiveInHierarchy())
	//{
	//	render_enabled = true;
	//	RenderManager::getInstance().AddRenderable((RenderableObject*)this);
	//}
	/*
	std::cout << "Begin Loading Model" << std::endl;
	//model->material = material;
	if(!pathToObjModel.empty())
		model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);

	std::cout << "End Loading Model" << std::endl;

	if (model == nullptr)
	{
		std::cout << "ERROR LOADING MESH" << std::endl;
		return;
	}
	*/
	//// set up each mesh here??? - DM
	//for (auto m : model->meshes) {
	//	std::cout << "setting up mesh: " << m->name << std::endl;
	//	m->Setup();
	//}

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

	//PrintVertices();
}

// Process the material for the mesh
Material* MeshRenderer::processMeshMaterial(aiMesh * mesh, const aiScene * scene, std::string filePath) // path to obj file.
{
	std::cout << "ModelLibrary.processMeshMaterial called with arguments\n";
	std::cout << "\tfilePath: " << filePath << std::endl;
	std::string matFilePath = "../Assets/Materials/" + (std::string)mesh->mName.C_Str() + ".material";	//filePath += fileName + ".material";
	// get material
	//std::string meshMatName = ;
	//Material* MatforMesh = AssetManager::getInstance().materialLib.GetAsset(meshMatName, "3Dmodel.vs", "3Dmodel.fs");

	// only used name of the material to get it (not anymore)
	std::cout << "\nmatFilePath (fileName): " << matFilePath << std::endl;
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


// Check material textures of a given type and loads texture if not loaded yet
std::vector<TextureProperty> MeshRenderer::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, std::string filePath)
{
	std::cout << "ModelLibrary::loadMaterialTextures called with arguments\n";
	std::cout << "\ttypeName: " << typeName << std::endl;
	std::cout << "\tfilePath: " << filePath << std::endl;

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

			Texture* texture = &(AssetManager::getInstance().textureLib.GetAsset(filename));
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
unsigned int MeshRenderer::TextureFromFile(const char * path, const std::string &directory, bool gamma)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	std::cout << "3. filename == " << path << std::endl;

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


//void MeshRenderer::FreeAllResources()
//{
//	//ToDo: Loop through all renderables and free their resources
//	for (size_t i = 0; i < currentRenderables.size(); i++)
//	{
//		FreeObjectResources(currentRenderables[i]);
//	}
//}

void MeshRenderer::FreeObjectResources()
{
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &(VAO));
	glDeleteBuffers(1, &(VBO));
	glDeleteBuffers(1, &(EBO));
}

void MeshRenderer::PrintVertices()
{
	/*
	for (int i = 0; i < model->meshes.size(); i++)
	{
		std::cout << "Mesh Index: " << model->meshes[i]->name << "\t";
		for (int j = 0; j < model->meshes[i]->vertices.size(); j++)
		{
			std::cout << "Vertex Index: " << j << "\t";
			std::cout << model->meshes[i]->vertices[j].Position.x << " " << model->meshes[i]->vertices[j].Position.y << " " << model->meshes[i]->vertices[j].Position.z;
			std::cout << "\t" << model->meshes[i]->vertices[j].Normal.x << " " << model->meshes[i]->vertices[j].Normal.y << " " << model->meshes[i]->vertices[j].Normal.z;
			std::cout << "\t" << model->meshes[i]->vertices[j].TexCoords.x << " " << model->meshes[i]->vertices[j].TexCoords.y << std::endl;
		}
	}
	*/
}

bool MeshRenderer::LoadModel()
{
	/*
	model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);
	return (model == nullptr);
	*/
}

void MeshRenderer::Draw()
{
	//material->Load();

	//if (gameObject == nullptr) return;
	
	// This was the error

	if(model != nullptr)
	{
		glm::mat4 view = RenderManager::getInstance().getView();
		glm::mat4 projection = RenderManager::getInstance().getProjection();

		for (unsigned int i = 0; i < model->meshes.size(); i++)
		{
			model->MeshToMaterial.at(model->meshes[i]->name)->shader->use();
			RenderManager::getInstance().currentShaderID = model->MeshToMaterial.at(model->meshes[i]->name)->shader->ID;
			model->MeshToMaterial.at(model->meshes[i]->name)->Load();

			model->MeshToMaterial.at(model->meshes[i]->name)->shader->setMat4("view", view);
			model->MeshToMaterial.at(model->meshes[i]->name)->shader->setMat4("projection", projection);
			model->MeshToMaterial.at(model->meshes[i]->name)->shader->setMat4("model", this->gameObject->transform->getMatrix4x4());

			//TODO: Calculate this inside the SHADER using the VIEW MATRIX. (3rd column)
			glm::vec3 viewPos = ((CameraComponent*)RenderManager::getInstance().getCurrentCamera())->gameObject->transform->getPosition();
			model->MeshToMaterial.at(model->meshes[i]->name)->shader->setVec3("viewPos", viewPos);

			// texture variables
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			// binding textures
			for (unsigned int j = 0; j < model->MeshToMaterial.at(model->meshes[i]->name)->textureProperties.size(); j++)
			{
				// get texture before binding
				glActiveTexture(GL_TEXTURE0 + j);

				std::string number;
				std::string name = model->MeshToMaterial.at(model->meshes[i]->name)->textureProperties[j].getValue()->type;

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
				glUniform1i(glGetUniformLocation(model->MeshToMaterial.at(model->meshes[i]->name)->shader->ID, (name + number).c_str()), j);
				// bind texture
				glBindTexture(GL_TEXTURE_2D, model->MeshToMaterial.at(model->meshes[i]->name)->textureProperties[j].getValue()->id);
			}

			model->MeshToMaterial.at(model->meshes[i]->name)->Draw();

			// Try to delegate to Material class????
			// draw mesh
			glBindVertexArray(model->meshes[i]->VAO);
			glDrawElements(GL_TRIANGLES, model->meshes[i]->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// default once configured
			glActiveTexture(GL_TEXTURE0);
		}
	}
}

void MeshRenderer::OnDrawGizmos()
{
	//gameObject->transform->DrawGizmo();
}

void MeshRenderer::DrawInspector()
{
	ImGui::InputText("ModelPath", &this->pathToObjModel[0], 48);
	if (model && !model->meshes.empty()) // first conditional check if model is not a nullptr
	{
		for (size_t i = 0; i < model->meshes.size(); i++)
		{
			ImGui::Text(model->meshes[i]->name.c_str());
			model->MeshToMaterial.at(model->meshes[i]->name)->DrawInspector();
		}
	}

	if (ImGui::Button("Change Model"))
	{
		//model->MeshToMaterial.clear();
		//model->meshes.clear();
		model = nullptr;
		isSetup = false;
		Setup();
	}

	const ImGuiPayload* payload = ImGui::GetDragDropPayload();
	if (payload != nullptr && payload->IsDataType("FILE_DRAG"))
	{
		ImGui::Text("<----- CHANGE MODEL ----->");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
			{
				IM_ASSERT(payload->DataSize == 128);
				const char* payload_n = (const char*)payload->Data;

				std::string fileName(payload_n);
				if (fileName.substr(fileName.find_last_of(".")) == ".obj")
				{
					std::cout << "Dropping MODEL!" << std::endl;
				
					// temporary fix, replace any backward slash with a forward slash
					std::replace(fileName.begin(), fileName.end(), '\\', '/' );

					pathToObjModel = fileName;
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	
}