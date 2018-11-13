#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "RenderableObject.h"
#include "Shader.h"
#include "Material.h"
/* The RenderManager is responsible for:
- Tracking all renderable objects
- Handling switching between shaders
- Drawing all rendered objects optimally
*/

class RenderManager
{
public:
	static RenderManager* instance;
	static Shader* defaultShader;
	static Material* defaultMaterial;

	unsigned int currentShaderID = 0;
	bool isInitialized = false;
	std::vector<RenderableObject*> currentRenderables;

	// Create static instance
	// ToDo: Setup the singleton manager pattern as a base class.
	static RenderManager* CreateManager();

	// Init instance
	int Init();

	void Render();
	void RenderObject(RenderableObject* renderable);

	void FreeAllResources();
	void FreeObjectResources(RenderableObject* renderable);


	void AddRenderable(RenderableObject * renderable);

	void RemoveRenderable(RenderableObject * renderable);

private:
	void CompileShaders();
	
	//std::vector<RenderableObject*> GetCurrentRenderables();
};
