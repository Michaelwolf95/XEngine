#pragma once
#include "LibraryExport.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Material.h"

class ENGINE_API RenderableObject
{	
public:
	bool render_enabled = false;

	virtual void Draw() = 0;
	virtual void Setup() = 0;
};