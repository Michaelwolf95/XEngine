#include "SimpleModelComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <typeinfo>

#include "ApplicationManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "Input.h"

REGISTER_COMPONENT(SimpleModelComponent, "SimpleModelComponent")

SimpleModelComponent::SimpleModelComponent()
{
	//Setup();
}

SimpleModelComponent::SimpleModelComponent(std::string _name, float* verts, unsigned int numV, unsigned int vertDataSize, unsigned int * ind, unsigned int numInd, Material * _material)
	//: RenderableObject(verts, numV, vertDataSize, ind, numInd, _material) // removed from renderableobject
{
	name = _name;
	this->vertices = verts;
	this->numVerts = numV;
	this->vertexDataSize = vertDataSize;
	this->indices = ind;
	this->numIndices = numInd;
	this->material = _material;
	Setup();
	//std::cout << this->material->to_string() << std::endl;
}

SimpleModelComponent::~SimpleModelComponent()
{
	std::cout << "\t\tDeleting SimpleModelComponent" << std::endl;
	RenderManager::getInstance().RemoveRenderable((RenderableObject*)this);
}

void SimpleModelComponent::Setup()
{
	if (isSetup)
	{
		//RenderManager::getInstance().RemoveRenderable((RenderableObject*)this);
		return;
	}
	//std::cout << "Setting up SimpleModelComponent." << std::endl;
	render_enabled = true;
	RenderManager::getInstance().AddRenderable((RenderableObject*)this);

	if (material == nullptr)
	{
		//material = RenderManager::defaultMaterial;
		
		try
		{
			std::cout << "Warning: material was NULL!\n";
			std::string pathToMaterial = "../Assets/Materials/" + name + ".material";
			material = AssetManager::getInstance().materialLib.GetAsset(pathToMaterial);
		}
		catch (std::exception e)
		{
			// not tested yet
			std::cout << "ERROR: SimpleModelComponent::Setup: " << e.what() << std::endl;
			std::cout << "Loading defaultMaterial...\n";
			material = RenderManager::defaultMaterial;
		}
		//material = new Material("MuliLight Model", "multilights.vs", "multilights.fs");
		//std::cout << "Material set to default." << std::endl;
	}
	else
	{
		//std::cout << "Material already set." << std::endl;
		//std::cout << this->material << std::endl;
		//std::cout << this->material->shader->ID << std::endl;
	}

	// generates buffer names which points to data lists
	glGenVertexArrays(1, &VAO);
	// geerates buffer manager to store and access main object data
	glGenBuffers(1, &VBO);
	// generates buffer manager to store and access compression data
	glGenBuffers(1, &EBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);

	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// assigning vertex array to buffer assigned to VBO
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize * numVerts * sizeof(float), vertices, GL_STATIC_DRAW); 
	
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// assigning indices array to buffer assigned to EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// 4. then set the vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexDataSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexDataSize * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexDataSize * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	isSetup = true;
}

void SimpleModelComponent::Draw()
{


	//for (int i = 0; i < this->numVerts; i++)
	//{
	//	if (i % 8 == 0) std::cout << std::endl;
	//	std::cout << vertices[i] << " ";
	//}

	//std::cout << "\n\n";
	//std::cout << "SimpleModelComponent->Draw()\n";
	//if (Component::enabled == false) return;
	if (gameObject == nullptr) {
		std::cout << "***gameObject was NULL" << std::endl;
		return;
	}

	//material->shader->use(); // moved to material::Load

	//RenderManager::getInstance().currentShaderID = material->shader->ID; // moved to material::Load
	material->Load();
	
	// create transformations
	// View & projection from RenderManager, which uses active camera.
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();

	// Model uses GameObject transform.
	glm::mat4 model = (gameObject->transform->getMatrix4x4());

	material->shader->setMat4("view", view);
	material->shader->setMat4("projection", projection);
	material->shader->setMat4("model", model);


	glm::vec3 viewPos  = ((CameraComponent*)RenderManager::getInstance().getCurrentCamera())->gameObject->transform->getPosition();
	material->shader->setVec3("viewPos", viewPos);
	
	//glUniformli
	material->shader->setInt("Texture", material->textureProperties[0].getValue()->id);

	// TODO:: place this into material (with texture properties?)
	// bind textures on corresponding texture unit
	glActiveTexture(GL_TEXTURE0);
	// put the texture with this ID into texture location 0
	glBindTexture(GL_TEXTURE_2D, material->textureProperties[0].getValue()->id);

	material->Draw();

	// draw model
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
	glBindVertexArray(0);

	// default texture position
	glActiveTexture(GL_TEXTURE0);
}

void SimpleModelComponent::Start()
{
	if (isSetup == false)
	{
		Setup();
	}
}

void SimpleModelComponent::Update()
{
}

void SimpleModelComponent::OnDrawGizmos()
{
	//gameObject->transform->DrawGizmo();
}

void SimpleModelComponent::DrawInspector()
{

	this->material->DrawInspector();

	const ImGuiPayload* payload = ImGui::GetDragDropPayload();
	if (payload != nullptr && payload->IsDataType("FILE_DRAG"))
	{
		ImGui::Text("<----- CHANGE MATERIAL ----->");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
			{
				IM_ASSERT(payload->DataSize == 128);
				const char* payload_n = (const char*)payload->Data;

				std::string fileName(payload_n);
				if (fileName.substr(fileName.find_last_of(".")) == ".material")
				{
					std::cout << "Dropping Material!" << std::endl;
					fileName = fileName.substr(fileName.find_last_of("\\") + 1); // NOTE: MAKE SURE THIS WORKS ON ALL SYSTEMS!!!
					size_t lastindex = fileName.find_last_of(".");
					fileName = fileName.substr(0, lastindex);
					Material* mat = AssetManager::getInstance().materialLib.GetAsset(fileName);
					if (mat != nullptr)
					{
						this->material = mat;
						std::cout << "Dropping Material!" << std::endl;
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

}
