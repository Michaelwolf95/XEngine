#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "RenderableObject.h"

/* The RenderManager is responsible for:
- 
*/


class RenderManager
{
public:
	static RenderManager* instance;
	static int defaultShaderProgram;

	bool isInitialized = false;
	std::vector<RenderableObject*> currentRenderables;

	// Create static instance
	// ToDo: Setup the singleton manager pattern as a base class.
	static RenderManager* CreateManager();

	// Init instance
	int Init();

	int CompileShaders(const char *vertexShaderSource, const char *fragmentShaderSource);

	void Render();
	void RenderObject(RenderableObject* renderable);

	void FreeAllResources();
	void FreeObjectResources(RenderableObject* renderable);


	void AddRenderable(RenderableObject * renderable);

	void RemoveRenderable(RenderableObject * renderable);
	
	//std::vector<RenderableObject*> GetCurrentRenderables();
};
