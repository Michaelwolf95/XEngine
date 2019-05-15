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
	if (isSetup) return;

	size_t delimiter = meshPath.find('|');
	std::string filePath = meshPath.substr(0, delimiter);
	std::string meshName = meshPath.substr(delimiter+1);

	// if no mesh name after demiliter, make meshName empty
	if (meshName == filePath)
	{
		meshName = "";
	}

	//std::cout << "MESH FILEPATH: " + filePath << std::endl;
	//std::cout << "MESH Name: " + meshName << std::endl;

	// Get Mesh
	if (meshPath.empty())
		std::cout << "No Mesh Path" << std::endl;
	else
		mesh = AssetManager::getInstance().meshLib.GetAsset(filePath, meshName);

	//std::cout << "VERTEXT COUNT - " << meshName << ": " << mesh->vertices.size() << std::endl;
	//std::cout << "INDICES COUNT - " << meshName << ": " << mesh->indices.size() << std::endl;
	
	if (material == nullptr)
	{
		// Get Material
		if (materialPath.empty())
		{
			std::cout << "No Material Path: Using Default Material ========================================================================" << std::endl;
			// Get Default Materia FilePath of the Mesh from Obj file
			materialPath = ASSET_FILE_PATH + "Materials/" + (std::string)mesh->name + ".material";	//filePath += fileName + ".material";

			material = AssetManager::getInstance().materialLib.GetAsset(materialPath);
		}
		else
		{
			material = AssetManager::getInstance().materialLib.GetAsset(materialPath);
		}
	}

	material->shader->use();
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
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
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(GL_TEXTURE_2D, material->textureProperties[j].getValue()->id);
	}


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
	//glDeleteVertexArrays(1, &(VAO));
	//glDeleteBuffers(1, &(VBO));
	//glDeleteBuffers(1, &(EBO));
}

//bool MeshRenderer::LoadModel()
void MeshRenderer::Load()
{
	//Setup();
}

void MeshRenderer::Draw()
{
	//material->Load();

	//if (gameObject == nullptr) return;

	if(mesh != nullptr)
	{
		glm::mat4 view = RenderManager::getInstance().getView();
		glm::mat4 projection = RenderManager::getInstance().getProjection();
		
		material->Load();

		material->shader->setMat4("view", view);
		material->shader->setMat4("projection", projection);
		material->shader->setMat4("model", this->gameObject->transform->getMatrix4x4());

		//TODO: Calculate this INSIDE THE SHADER using the VIEW MATRIX. (3rd column)
		//https://learnopengl.com/Getting-started/Transformations
		//glm::vec3 viewPos = ((CameraComponent*)RenderManager::getInstance().getCurrentCamera())->gameObject->transform->getPosition();
		//material->shader->setVec3("viewPos", viewPos);

		material->Draw();

		// DRAW THE MESH =====
		// 
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		material->EndDraw();

	}
	else
	{
		std::cout << "MESH IS NULL\n";
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
		//materialPath = "";
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
				std::string ext = fileName.substr(fileName.find_last_of("."));
				if (ext == ".obj" || ext == ".fbx")
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