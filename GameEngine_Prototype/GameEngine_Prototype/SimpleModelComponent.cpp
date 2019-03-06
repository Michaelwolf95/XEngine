#include "SimpleModelComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ApplicationManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "Input.h"
#include "PointLightComponent.h"
#include "GlobalLightComponent.h"

REGISTER_COMPONENT(SimpleModelComponent, "SimpleModelComponent")

SimpleModelComponent::SimpleModelComponent()
{

}

SimpleModelComponent::SimpleModelComponent(float * verts, unsigned int numV, unsigned int vertDataSize, unsigned int * ind, unsigned int numInd, Material * _material)
	: RenderableObject(verts, numV, vertDataSize, ind, numInd, _material)
{
	Setup();
}

SimpleModelComponent::~SimpleModelComponent()
{
	std::cout << "Deleting SimpleModelComponent" << std::endl;
	RenderManager::getInstance().RemoveRenderable((RenderableObject*)this);
}

void SimpleModelComponent::Setup()
{
	if (isSetup)
	{
		return;
	}
	//std::cout << "Setting up SimpleModelComponent." << std::endl;
	render_enabled = true;
	RenderManager::getInstance().AddRenderable((RenderableObject*)this);

	if (material == nullptr)
	{
		material = RenderManager::defaultMaterial;
		//std::cout << "Material set to default." << std::endl;
	}
	else
	{
		//std::cout << "Material already set." << std::endl;
		//std::cout << this->material << std::endl;
		//std::cout << this->material->shader->ID << std::endl;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize * numVerts * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

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
	//if (Component::enabled == false) return;
	if (gameObject == nullptr) return;

	// create transformations
	// View & projection from RenderManager, which uses active camera.
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	glm::mat4 view = RenderManager::getInstance().getView();

	// Model uses GameObject transform.
	glm::mat4 model = (gameObject->transform->getMatrix4x4());

	// retrieve the matrix uniform locations
	unsigned int modelLoc = glGetUniformLocation(material->shader->ID, "model");
	unsigned int viewLoc = glGetUniformLocation(material->shader->ID, "view");
	// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	material->shader->setMat4("projection", projection);


	if (material->useLight)
	{
		int i = 0;
		int pointLightCount = 0;
		int globalLightCount = 0;
		int spotLightCount = 0;
		material->shader->setInt("numLights", RenderManager::getInstance().lights.size());
		//glm::vec3 viewPos(view[3].x, view[3].y, view[3].z);
		glm::vec3 viewPos  = ((CameraComponent*)RenderManager::getInstance().getCurrentCamera())->gameObject->transform->getPosition();
		//material->shader->setVec3("viewPos", RenderManager::getInstance().getCurrentCamera()->getPosition()); // TODO: not working properly
		material->shader->setVec3("viewPos", viewPos);
		for (Light* light : RenderManager::getInstance().lights)
		{
			std::string strPointLights = "pointLights[]";
			std::string strGlobalLights = "globalLights[]";

			strPointLights.insert(12, std::to_string(pointLightCount));
			strGlobalLights.insert(13, std::to_string(globalLightCount));

			if (light->getTypeID() == PointLightComponent::TYPE_ID) {
				material->shader->setVec3(strPointLights + ".color", light->getLightColor());
				material->shader->setVec3(strPointLights + ".position", light->getLightPos());
				material->shader->setFloat(strPointLights + ".intensity", ((LightComponent *)light)->getIntensity());
				material->shader->setFloat(strPointLights + ".constant", ((PointLightComponent *)light)->getConstant());
				material->shader->setFloat(strPointLights + ".linear", ((PointLightComponent *)light)->getLinear());
				material->shader->setFloat(strPointLights + ".quadratic", ((PointLightComponent *)light)->getQuadratic());
				pointLightCount++;
			}

			if (light->getTypeID() == GlobalLightComponent::TYPE_ID) {
				material->shader->setVec3(strGlobalLights + ".color", light->getLightColor());
				material->shader->setVec3(strGlobalLights + ".position", light->getLightPos());
				material->shader->setFloat(strGlobalLights + ".intensity", ((LightComponent *)light)->getIntensity());
				material->shader->setVec3(strGlobalLights + ".direction", ((GlobalLightComponent *)light)->getDirection());
				globalLightCount++;
			}
			i++;
		}
		material->shader->setInt("numPointLights", pointLightCount);
		material->shader->setInt("numGlobalLights", globalLightCount);
		//material->shader->setMat3
	}


	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
	glBindVertexArray(0);

	//gameObject->transform->DrawGizmo();
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
