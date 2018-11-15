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


void RenderManager::DrawScreenPoint(glm::vec2 point, glm::vec4 color, int size)
{
	GLfloat p[]{ point.x, point.y };
	//GLfloat c[]{ color.r, color.g, color.b, color.a };
	glClear(GL_DEPTH_BUFFER_BIT); // Clears the depth buffer so we can draw on top.

	colorDrawShader->use();
	colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);

	glm::mat4 view = RenderManager::getInstance().getView();
	glm::mat4 projection = RenderManager::getInstance().getProjection();
	glm::mat4 model(1.0);
	colorDrawShader->setMat4("model", model);
	colorDrawShader->setMat4("view", view);
	colorDrawShader->setMat4("projection", projection);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_POINT_SMOOTH); // Remove this wrap for square points.
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glPointSize(size);
	//glColorPointer(1, GL_FLOAT, 0, c);
	glVertexPointer(2, GL_FLOAT, 0, new GLfloat[point.x, point.y]);

	//glBindVertexArray(2, GL_FLOAT, 0, new float[point.x, point.y]);
	glDrawArrays(GL_POINTS, 0, 1);

	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisable(GL_POINT_SMOOTH);
}
void RenderManager::DrawWorldPoint(glm::vec3 worldPoint, glm::vec4 color, int size)
{
	
}


/*
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 600); // essentially set coordinate system
glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
glLoadIdentity();


//glDrawArrays(GL_POINTS, 0, 12);
//glEnable(GL_PROGRAM_POINT_SIZE);
//gl_PointSize = 10.0;

//glBegin(GL_LINES);
//glVertex2f(10, 10);
//glVertex2f(20, 20);
//glEnd();

////glBegin(GL_POINTS);
////glColor3f(0.3, 0.3, 0.3);
////glPointSize(5.0f);  // wat
////glVertex2i(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
////glEnd();
//glEnable(GL_PROGRAM_POINT_SIZE);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
//gluPerspective( 65.0, (double)1024/(double)768, 1.0, 60.0 );
glOrtho(0, ApplicationManager::getInstance().config->screenWidth,
	ApplicationManager::getInstance().config->screenHeight, 0, -1, 1);// 100, -100);
//glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();
glPointSize(50);
glBegin(GL_POINTS);
//vec4 point = view * projection * (*model) * vec4(0.0, 0.0, 0.0, 1.0);
//glVertex3f(point.x, point.y, point.z);
//glVertex2d(point.x, point.y);
//glVertex2f(0.0, 0.0);
//glVertex3f(0.0, 0.0, 0.0);
glColor4f(1,1,1,1);
//glVertex3f(ApplicationManager::getInstance().config->screenWidth/2,
//	ApplicationManager::getInstance().config->screenHeight / 2,
//	0.0);
glVertex2i(100, 100);
//glDrawArrays(GL_POINTS, 0, 1);
glClear(GL_COLOR_BUFFER_BIT);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0, ApplicationManager::getInstance().config->screenWidth,
	ApplicationManager::getInstance().config->screenHeight, 0, -1, 1);
glEnd();
glBegin(GL_POINTS);
  glVertex2i(100, 100);
glEnd();
//glBegin(GL_LINES);
//glVertex3f(0.0f, 0.0f, 0.0f);
//glVertex3f(50.0f, 50.0f, 50.0f);
//glEnd();

//glLineWidth(2.5);
//glColor3f(1.0, 0.0, 0.0);
//glBegin(GL_LINES);
//glVertex3f(0.0, 0.0, 0.0);
//glVertex3f(15, 0, 0);
//glEnd();
//glPointSize(1.0); // Default
*/