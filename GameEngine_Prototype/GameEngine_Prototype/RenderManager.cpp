#include "RenderManager.h"
#include "ApplicationManager.h"
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>


RenderManager* RenderManager::instance = nullptr;
Shader* RenderManager::defaultShader = nullptr;
Material* RenderManager::defaultMaterial = nullptr;

float clearColor[4] = { 0.2f, 0.3f, 0.3f, 1.0f };

// Create static instance
// ToDo: Setup the singleton manager pattern as a base class.
RenderManager* RenderManager::CreateManager()
{
	if (instance != nullptr)
	{
		printf("DUPLICATE SINGLETON DETECTED");
		return NULL;
	}
	instance = new RenderManager();
	instance->Init();
	return instance;
}

// Init instance
int RenderManager::Init()
{
	CompileShaders();

	view = new glm::mat4(1.0f);
	projection = new glm::mat4(1.0f);
	*projection = glm::perspective(glm::radians(45.0f),
		(float)ApplicationManager::config->screenWidth / (float)ApplicationManager::config->screenHeight,
		0.1f, 100.0f);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	isInitialized = true;
	return 0;
}
void RenderManager::CompileShaders()
{
	defaultShader = new Shader("default.vs", "default.fs");
	defaultShader->use();
	defaultShader->setColor("MainColor", 1.0f, 0.0f, 1.0f, 1.0f); // Pink
	
	defaultMaterial = new Material(defaultShader);
	defaultMaterial->Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

	//ToDo: Pre-compile all shaders that might be used in the scene?

}

void RenderManager::Render()
{
	// Render back drop
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get Camera matrix information for use in next render
	// Camera has "view" and "projection" information.

	//  Loop through and render all renderables
	for (size_t i = 0; i < currentRenderables.size(); i++)
	{
		RenderObject(currentRenderables[i]);
	}
}
void RenderManager::RenderObject(RenderableObject* renderable)
{
	if (renderable->enabled == false)
	{
		// Don't render anything
		return;
	}

	// ToDo: Optimize draw calls by rendering all objects that use the same shader at once.
	
	// Start the shader
	if (currentShaderID != renderable->material->shader->ID)
	{
		//std::cout << "Swapping material" << std::endl;
		renderable->material->shader->use();
		currentShaderID = renderable->material->shader->ID;
	}
	//ToDo: Track current material and draw all objects that use it
	renderable->material->Load();

	// Draw the object
	renderable->Draw();
}


void RenderManager::FreeAllResources()
{
	//ToDo: Loop through all renderables and free their resources
	for (size_t i = 0; i < currentRenderables.size(); i++)
	{
		FreeObjectResources(currentRenderables[i]);
	}
}

void RenderManager::FreeObjectResources(RenderableObject* renderable)
{
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &(renderable->VAO));
	glDeleteBuffers(1, &(renderable->VBO));
	glDeleteBuffers(1, &(renderable->EBO));
}


void RenderManager::AddRenderable(RenderableObject* renderable)
{
	currentRenderables.push_back(renderable);
}
void RenderManager::RemoveRenderable(RenderableObject* renderable)
{
	// If vector contains it, remove it.
	// https://stackoverflow.com/questions/39912/how-do-i-remove-an-item-from-a-stl-vector-with-a-certain-value
	auto n = std::find(currentRenderables.begin(), currentRenderables.end(), renderable);
	if (n != currentRenderables.end())
	{
		// swap the one to be removed with the last element
		// and remove the item at the end of the container
		// to prevent moving all items after '5' by one
		std::swap(*n, currentRenderables.back());
		currentRenderables.pop_back();
	}
}