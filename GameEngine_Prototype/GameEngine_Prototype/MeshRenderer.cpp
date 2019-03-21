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
MeshRenderer::MeshRenderer(std::string const &path, Material* m , bool gamma):
	gammaCorrection(gamma), 
	RenderableObject(m)
{
	// filepath for .obj file.
	this->pathToObjModel ="../Assets/"  + std::string(path);

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

	std::cout << "Begin Loading Model" << std::endl;
	model = AssetManager::getInstance().modelLib.GetAsset(pathToObjModel);
	std::cout << "Finish Loading Model" << std::endl;


	if (model == nullptr)
	{
		std::cout << "ERROR LOADING MESH" << std::endl;
		return;
	}
	isSetup = true;
}

void MeshRenderer::Draw()
{
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	material->shader->setMat4("view", view);
	material->shader->setMat4("projection", projection);
	material->shader->setMat4("model", this->gameObject->transform->getMatrix4x4());
	
	// for mesh name
	std::string meshMatName = "_mat";

	glm::vec3 viewPos = ((CameraComponent*)RenderManager::getInstance().getCurrentCamera())->gameObject->transform->getPosition();
	
	material->shader->setVec3("viewPos", viewPos);
	material->shader->setVec3("lightPos", RenderManager::getInstance().lights.back()->getLightPos());
	material->shader->setVec3("lightColor", RenderManager::getInstance().lights.back()->getLightColor());

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