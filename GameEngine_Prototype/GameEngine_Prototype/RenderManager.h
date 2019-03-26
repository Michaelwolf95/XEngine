#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "RenderableObject.h"
#include "Shader.h"
#include "Material.h"
#include "Singleton.h"
#include "Camera.h"
#include "Light.h"
#include "LineDrawer.h"
#include "Scene.h"

/* The RenderManager is responsible for:
- Tracking all renderable objects
- Handling switching between shaders
- Drawing all rendered objects optimally
*/
class RenderManager : public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;
public:
	static Shader* defaultShader;
	static Material* defaultMaterial;
	static Shader* colorDrawShader;
	static Shader* defaultSpriteShader;
	static Shader* defaultTextShader;

	unsigned int currentShaderID = 0;
	bool isInitialized = false;
	// Current view and projection matrices. These can be set by the camera.
	glm::mat4 defaultView;
	glm::mat4 defaultProjection;

	std::vector<RenderableObject*> currentRenderables;
	std::vector<Light*> lights;
	std::vector<LineDrawer*> lineDrawers;
	// Create static instance
	static RenderManager* CreateManager();

	// Init instance
	int Init();

	glm::mat4 getView();
	glm::mat4 getProjection();
	Camera* getCurrentCamera();
	void setCurrentCamera(Camera* cam);

	void Render();
	void RenderObject(RenderableObject* renderable);

	void AddRenderable(RenderableObject * renderable);
	void RemoveRenderable(RenderableObject * renderable);
	void AddLight(Light * light);
	void RemoveLight(Light * light);

	void FreeAllResources();
	void FreeObjectResources(RenderableObject* renderable);

	void FindCameraInScene(Scene* scene);

	static void DrawScreenSpacePoint(glm::vec2 point, glm::vec4 color, int size);
	static void DrawWorldSpacePoint(glm::vec3 worldPoint, glm::vec4 color, int size);
	static void DrawScreenSpaceLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color, int size);
	static void DrawWorldSpaceLine(glm::vec3 point1, glm::vec3 point2, glm::vec4 color, int size);
	static void DrawWorldSpaceBox(glm::vec3 center, glm::vec3 extents, glm::vec4 color, int size);

private:
	Camera* currentCamera;
	void CompileShaders();
};
