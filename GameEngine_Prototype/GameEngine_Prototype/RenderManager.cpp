#include "RenderManager.h"
#include "ApplicationManager.h"
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SceneManager.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "PrimitiveModels.h"

Shader* RenderManager::defaultShader = nullptr;
Material* RenderManager::defaultMaterial = nullptr;
Shader* RenderManager::colorDrawShader = nullptr;
Shader* RenderManager::gizmoSpriteShader = nullptr;
Shader* RenderManager::defaultTextShader = nullptr;

//TODO: Store this on the Camera.
glm::vec4 defaultClearColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f );

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

	defaultView = glm::mat4(1.0f);
	defaultView = glm::rotate((defaultView), glm::radians(180.0f), glm::vec3(0, 1, 0));
	defaultProjection = glm::mat4(1.0f);
	defaultProjection = glm::perspective(glm::radians(45.0f),
		(float)ApplicationManager::config->screenWidth / (float)ApplicationManager::config->screenHeight,
		0.1f, 100.0f);

	// Create default primitive meshes.
	// TODO: Finish this.
	/*std::vector<float> cubeVerts(CUBE_VERTS, CUBE_VERTS + (sizeof(CUBE_VERTS) / (sizeof(float))));
	std::vector<unsigned int> cubeInd(CUBE_INDICES, CUBE_INDICES + (sizeof(CUBE_INDICES) / (sizeof(unsigned int))));
	boxMesh = new Mesh("Cube", cubeVerts, cubeInd);*/

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	isInitialized = true;
	return 0;
}

void RenderManager::CompileShaders()
{
	std::cout << "RENDERMANAGER::CompileShaders\n";
	defaultShader = new Shader("model.vs", "model.fs");
	
	//defaultMaterial = new Material("Default Mat", "model.vs", "model.fs");
	//defaultMaterial = AssetManager::getInstance().materialLib.GetAsset("../Assets/Materials/Default_Mat.material");
	defaultMaterial = new Material("Default_Mat", "multilights.shader", "");

	//defaultMaterial = new Material(defaultShader);
	//defaultMaterial->Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	//defaultMaterial->LoadTexture("textures/container.jpg");

	colorDrawShader = new Shader("color.vs", "color.fs");

	gizmoSpriteShader = new Shader("billboardSprite.vs", "billboardSprite.fs");
	defaultTextShader = new Shader("text.vs", "text.fs");

	//ToDo: Pre-compile all shaders that might be used in the scene?
}

glm::mat4 RenderManager::getView()
{
	//OutputDebugStringW(L"Getting View...\n");
	//std::cout << "Getting View..." << std::endl;
	if (currentCamera != nullptr && currentCamera != NULL)
	{
		//std::cout << typeid(currentCamera).name() << std::endl;
		glm::mat4 view = currentCamera->getView();
		return view;
	}
	return defaultView;
}

glm::mat4 RenderManager::getProjection()
{
	if (currentCamera != nullptr)
	{
		return currentCamera->getProjection();
	}
	return defaultProjection;
}

Camera* RenderManager::getCurrentCamera()
{
	return currentCamera;
}

void RenderManager::setCurrentCamera(Camera* cam)
{
	//std::cout << "Setting Current Cam: " << cam << std::endl;
	if (currentCamera != cam)
	{
		currentCamera = cam;
	}
}

void RenderManager::Render()
{
	// Render back drop
	glm::vec4 clearColor = (currentCamera != nullptr) ? currentCamera->clearColor : defaultClearColor;
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get Camera matrix information for use in next render
	// Camera has "view" and "projection" information.

	//  Loop through and render all renderables
	for (size_t i = 0; i < currentRenderables.size(); i++)
	{
		if(currentRenderables[i] != nullptr)
			RenderObject(currentRenderables[i]);
	}

	// TODO: Make it a global option of whether we draw gizmos or not.
	// TODO: Make drawing gizmos an event subsriber based model - not a callback.
	// Draw Gizmos
	if (SceneManager::getInstance().GetActiveScene() != nullptr && SceneManager::getInstance().GetActiveScene()->isLoaded)
	{
		for (GameObject_ptr go : SceneManager::getInstance().GetActiveScene()->rootGameObjects)
		{
			for (Component_ptr c : go->components)
			{
				c->OnDrawGizmos();
			}
		}
	}
}
void RenderManager::RenderObject(RenderableObject* renderable)
{
	if (renderable->render_enabled == false) // TODO: if enabled then draw. get rid of everything else
	{
		// Don't render anything
		return;
	}

	// TODO: Optimize draw calls by rendering all objects that use the same shader at once.
	
	// Start the shader
	//if (currentShaderID != renderable->material->shader->ID) // DISABLED CHECK FOR NOW
	{
		//std::cout << "Swapping material" << std::endl;
		//renderable->material->shader->use();
		//currentShaderID = renderable->material->shader->ID;
	}
	//TODO: Track current material and draw all objects that use it
	//renderable->material->Load();

	//std::cout << renderable << std::endl;
	// Draw the object
	renderable->Draw();
}

//// moved to MeshRenderer
//void RenderManager::FreeAllResources()
//{
//	//ToDo: Loop through all renderables and free their resources
//	for (size_t i = 0; i < currentRenderables.size(); i++)
//	{
//		FreeObjectResources(currentRenderables[i]);
//	}
//}
//
//void RenderManager::FreeObjectResources(RenderableObject* renderable)
//{
//	// optional: de-allocate all resources once they've outlived their purpose:
//	// ------------------------------------------------------------------------
//	glDeleteVertexArrays(1, &(renderable->VAO));
//	glDeleteBuffers(1, &(renderable->VBO));
//	glDeleteBuffers(1, &(renderable->EBO));
//}

void RenderManager::FindCameraInScene(Scene* scene)
{
	// Init Camera for RenderManager
	CameraComponent* camera = nullptr;
	for (GameObject_ptr go : scene->rootGameObjects)
	{
		// Finds the first object of the type CameraComponent
		// Just checks roots for now. - change to search all later.
		if (go->FindComponent(typeid(CameraComponent), (void**)&camera)) // Pointer to a pointer!
		{
			setCurrentCamera((CameraComponent*)camera);
			break;
		}
	}
}

void RenderManager::AddRenderable(RenderableObject* renderable)
{
	// If vector does not contain it, add it.
	auto n = std::find(currentRenderables.begin(), currentRenderables.end(), renderable);
	if (n == currentRenderables.end())
	{
		currentRenderables.push_back(renderable);
	}
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
	//std::cout << color.r << std::endl;
	//std::cout << color.g << std::endl;
	//std::cout << color.b << std::endl;
	colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);

	glm::vec3 point3 = glm::vec3(point.x, point.y, 0.2);
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
	//glEnableVertexAttribArray(0);

	glPointSize(size);
	glDrawArrays(GL_POINTS, 0, 1);

	//glDisableVertexAttribArray(0);
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

	glm::vec3 point3 = glm::vec3(point1.x, point1.y, 0.2);
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
	//glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void RenderManager::DrawWorldSpaceBox(glm::vec3 center, glm::vec3 extents, glm::vec4 color, int lineSize)
{
	glm::mat4 model(1.0);
	model = glm::translate(model, center);
	model = glm::scale(model, extents);
	DrawWorldSpaceBox(model, color, lineSize);
}
void RenderManager::DrawWorldSpaceBox(glm::mat4 model, glm::vec4 color, int lineSize)
{
	RenderManager::getInstance().currentShaderID = colorDrawShader->ID;
	glClear(GL_DEPTH_BUFFER_BIT); // Clears the depth buffer so we can draw on top.

	glUseProgram(0); // Reset the current shader. Makes sure that the data from previous call isn't reused.
	colorDrawShader->use();
	colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	colorDrawShader->setMat4("model", model);
	colorDrawShader->setMat4("view", view);
	colorDrawShader->setMat4("projection", projection);

	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_CUBE_VERTS, CUBE_VERTS, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Drawing
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(lineSize);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

}

void RenderManager::DrawWorldSpaceSphere(glm::vec3 center, glm::vec3 scale, float radius, glm::vec4 color, int lineSize)
{
	RenderManager::getInstance().currentShaderID = colorDrawShader->ID;
	glClear(GL_DEPTH_BUFFER_BIT); // Clears the depth buffer so we can draw on top.

	glUseProgram(0); // Reset the current shader. Makes sure that the data from previous call isn't reused.
	colorDrawShader->use();
	colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);

	glm::mat4 model(1.0);
	model = glm::translate(model, center);
	scale *= radius;
	model = glm::scale(model, scale);
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	colorDrawShader->setMat4("model", model);
	colorDrawShader->setMat4("view", view);
	colorDrawShader->setMat4("projection", projection);

	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_SPHERE_VERTS, SPHERE_VERTS, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Drawing
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(lineSize);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, (NUM_SPHERE_VERTS)/3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
