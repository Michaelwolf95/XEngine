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
#include "AssetLibrary.h"
#include "MeshLibrary.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "MeshRenderer.h"

#include "CameraComponent.h"

#include "imgui_stdlib.h"
#include "imgui_inspector_extensions.h"

REGISTER_COMPONENT(XEngine::MeshRenderer, "MeshRenderer")

namespace XEngine {
MeshRenderer::MeshRenderer() {}

// Constructor
MeshRenderer::MeshRenderer(std::string const &modelPath, std::string materialPath, bool gamma): gammaCorrection(gamma)//, RenderableObject(m)
{
	this->meshPath =  std::string(modelPath);

	if (meshPath.find(ASSET_FILE_PATH) == std::string::npos)
	{
		this->meshPath = ASSET_FILE_PATH + this->meshPath;
	}

	

	if(!materialPath.empty())
		this->materialPath = ASSET_FILE_PATH + std::string(materialPath);
	
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
	size_t delimiter = meshPath.find('|');
	std::string filePath = meshPath.substr(0, delimiter);
	std::string meshName = meshPath.substr(delimiter+1);

	// if no mesh name after demiliter, make meshName emtpy
	if (meshName == filePath)
	{
		meshName = "";
	}

	std::cout << "MESH FILEPATH: " + filePath << std::endl;
	std::cout << "MESH Name: " + meshName << std::endl;

	// Get Mesh
	if (meshPath.empty())
		std::cout << "No Mesh Path" << std::endl;
	else
		mesh = AssetManager::getInstance().meshLib.GetAsset(filePath, meshName);

	// Get Material
	if (materialPath.empty())
	{
		std::cout << "No Mesh Path: Using Default Material" << std::endl;
		// Get Default Materia FilePath of the Mesh from Obj file
		materialPath = ASSET_FILE_PATH + "Materials/" + (std::string)mesh->name + ".material";	//filePath += fileName + ".material";

		material = AssetManager::getInstance().materialLib.GetAsset(materialPath);
	}
	else
	{
		material = AssetManager::getInstance().materialLib.GetAsset(materialPath);
	}
		

	/*
	if(!pathToObjModel.empty())
		model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);

	std::cout << "End Loading Model" << std::endl;

	if (model == nullptr)
	{
		std::cout << "ERROR LOADING MESH" << std::endl;
		return;
	}
	// Get MAterial or get default
	*/
	isSetup = true;
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

/*
void MeshRenderer::PrintVertices()
{
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
}*/

//bool MeshRenderer::LoadModel()
void MeshRenderer::Load()
{
	//model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);
	//return (model == nullptr
	Setup();
}

void MeshRenderer::Draw()
{
	//material->Load();

	//if (gameObject == nullptr) return;
	
	// This was the error

	if(mesh != nullptr)
	{
		glm::mat4 view = RenderManager::getInstance().getView();
		glm::mat4 projection = RenderManager::getInstance().getProjection();

		//for (unsigned int i = 0; i < meshes.size(); i++)
		//{
			material->shader->use();
			RenderManager::getInstance().currentShaderID = material->shader->ID;
			material->Load();

			material->shader->setMat4("view", view);
			material->shader->setMat4("projection", projection);
			material->shader->setMat4("model", this->gameObject->transform->getMatrix4x4());

			//TODO: Calculate this inside the SHADER using the VIEW MATRIX. (3rd column)
			glm::vec3 viewPos = ((CameraComponent*)RenderManager::getInstance().getCurrentCamera())->gameObject->transform->getPosition();
			material->shader->setVec3("viewPos", viewPos);

			// texture variables
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			// binding textures
			for (unsigned int j = 0; j < material->textureProperties.size(); j++)
			{
				// get texture before binding
				glActiveTexture(GL_TEXTURE0 + j);

				std::string number;
				std::string name = material->textureProperties[j].getValue()->type;

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
				glBindTexture(GL_TEXTURE_2D, material->textureProperties[j].getValue()->id);
			}

			material->Draw();

			// Try to delegate to Material class????
			// draw mesh
			glBindVertexArray(mesh->VAO);
			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// default once configured
			glActiveTexture(GL_TEXTURE0);
		//}
	}
}

void MeshRenderer::OnDrawGizmos()
{
	//gameObject->transform->DrawGizmo();
}

void MeshRenderer::DrawInspector()
{
	std::string* imguiMeshPath = &meshPath;
	ImGui::InputText("MeshPath", imguiMeshPath);
	//std::string* imguiMaterialPath = &materialPath;

	//ImGui::InputText("MaterialPath", imguiMaterialPath);
	
	if (ImGui::Button("Change Model"))
	{
		materialPath = "";
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

					meshPath = fileName;
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	GUI::MaterialReference(this->material, this->materialPath, "Material");
	
}
}