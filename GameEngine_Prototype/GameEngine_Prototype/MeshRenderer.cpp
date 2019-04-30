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
MeshRenderer::MeshRenderer(std::string const &path, Material* m , bool gamma): gammaCorrection(gamma)//, RenderableObject(m)
{
	// filepath for .obj file.
	this->pathToObjModel = ASSET_FILE_PATH + std::string(path);
	
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
	
	//if (this->gameObject->IsActiveInHierarchy())
	//{
	//	render_enabled = true;
	//	RenderManager::getInstance().AddRenderable((RenderableObject*)this);
	//}

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
}

bool MeshRenderer::LoadModel()
{
	model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);
	return (model == nullptr);
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