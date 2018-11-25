#include "RenderManager.h"
#include "ApplicationManager.h"
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>


Shader* RenderManager::defaultShader = nullptr;
Material* RenderManager::defaultMaterial = nullptr;
Shader* RenderManager::colorDrawShader = nullptr;

glm::vec4 clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f );

// Create static instance
RenderManager* RenderManager::CreateManager()
{
	RenderManager* instance = &RenderManager::getInstance();
	instance->Init();
	return instance;
}

// Init instance
int RenderManager::Init()
{
	CompileShaders();

	currentCamera = nullptr;

	defaultView = new glm::mat4(1.0f);
	defaultProjection = new glm::mat4(1.0f);
	*defaultProjection = glm::perspective(glm::radians(45.0f),
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

	colorDrawShader = new Shader("color.vs", "color.fs");
	colorDrawShader->use();
	colorDrawShader->setColor("MainColor", 1.0f, 1.0f, 1.0f, 1.0f); // White

	//ToDo: Pre-compile all shaders that might be used in the scene?

}

glm::mat4 RenderManager::getView()
{
	if (currentCamera != nullptr)
	{
		return currentCamera->getView();
	}
	return *defaultView;
}

glm::mat4 RenderManager::getProjection()
{
	if (currentCamera != nullptr)
	{
		return currentCamera->getProjection();
	}
	return *defaultProjection;
}

Camera * RenderManager::getCurrentCamera()
{
	return currentCamera;
}

void RenderManager::setCurrentCamera(Camera * cam)
{
	std::cout << "Setting Current Cam: " << cam << std::endl;
	if (currentCamera != cam)
	{
		currentCamera = cam;
	}
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

void RenderManager::AddLight(Light* light)
{
	lights.push_back(light);
}
void RenderManager::RemoveLight(Light* light)
{
	// If vector contains it, remove it.
	auto n = std::find(lights.begin(), lights.end(), light);
	if (n != lights.end())
	{
		std::swap(*n, lights.back());
		lights.pop_back();
	}
}


void RenderManager::DrawScreenSpacePoint(glm::vec2 point, glm::vec4 color, int size)
{
	RenderManager::getInstance().currentShaderID = colorDrawShader->ID;
	glClear(GL_DEPTH_BUFFER_BIT); // Clears the depth buffer so we can draw on top.
	glUseProgram(0); // Reset the current shader. Makes sure that the data from previous call isn't reused.
	colorDrawShader->use();
	colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);

	glm::vec3 point3 = vec3(point.x, point.y, 0.2);
	glm::mat4 model(1.0);
	model = glm::translate(model, point3);
	glm::mat4 view(1.0);
	glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	colorDrawShader->setMat4("model", model);
	colorDrawShader->setMat4("view", view);
	colorDrawShader->setMat4("projection", projection);

	GLfloat p[]{ 0.0, 0.0 };
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glPointSize(size);
	glVertexPointer(2, GL_FLOAT, 0, p);
	glDrawArrays(GL_POINTS, 0, 1);

	glDeleteVertexArrays(1, &VAO);
}

void RenderManager::DrawWorldSpacePoint(glm::vec3 worldPoint, glm::vec4 color, int size)
{
	RenderManager::getInstance().currentShaderID = colorDrawShader->ID;
	glClear(GL_DEPTH_BUFFER_BIT); // Clears the depth buffer so we can draw on top.
	glUseProgram(0); // Reset the current shader. Makes sure that the data from previous call isn't reused.
	colorDrawShader->use();
	colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);

	glm::mat4 model(1.0);
	model = glm::translate(model, worldPoint);
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	colorDrawShader->setMat4("model", model);
	colorDrawShader->setMat4("view", view);
	colorDrawShader->setMat4("projection", projection);

	GLfloat p[]{ 0.0, 0.0, 0.0 };
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glPointSize(size);
	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
}

void RenderManager::DrawScreenSpaceLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color, int size)
{
	RenderManager::getInstance().currentShaderID = colorDrawShader->ID;
	glClear(GL_DEPTH_BUFFER_BIT); // Clears the depth buffer so we can draw on top.
	glUseProgram(0); // Reset the current shader. Makes sure that the data from previous call isn't reused.
	colorDrawShader->use();
	colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);

	glm::vec3 point3 = vec3(point1.x, point1.y, 0.2);
	glm::mat4 model(1.0);
	model = glm::translate(model, point3);
	glm::mat4 view(1.0);
	glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	colorDrawShader->setMat4("model", model);
	colorDrawShader->setMat4("view", view);
	colorDrawShader->setMat4("projection", projection);

	glm::vec2 diff = point2 - point1;
	GLfloat p[]{ 
		0.0, 0.0, 0.0,
		diff.x, diff.y, 0.0 };
	GLfloat i[]{ 0, 1, 0 };
	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p), p, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glLineWidth(size);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void RenderManager::DrawWorldSpaceLine(glm::vec3 point1, glm::vec3 point2, glm::vec4 color, int size)
{
	RenderManager::getInstance().currentShaderID = colorDrawShader->ID;
	glClear(GL_DEPTH_BUFFER_BIT); // Clears the depth buffer so we can draw on top.

	glUseProgram(0); // Reset the current shader. Makes sure that the data from previous call isn't reused.
	colorDrawShader->use();
	colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);

	glm::mat4 model(1.0);
	model = glm::translate(model, point1);
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	colorDrawShader->setMat4("model", model);
	colorDrawShader->setMat4("view", view);
	colorDrawShader->setMat4("projection", projection);

	glm::vec3 diff = point2 - point1;
	GLfloat p[]
	{ 
		0.0, 0.0, 0.0, 
		diff.x, diff.y, diff.z
	};
	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p), p, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glLineWidth(size);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
