#pragma once
#include "Drawer.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

// This is designed to be an optimal solution for drawing lines.
// However... its not as necessary as I thought when implementing it.
// Keeping this here 
class LineDrawer : public Drawer
{
public:
	GLfloat verts[6];
	glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	unsigned int size = 5;
	bool isUsed = false; // Used for the frame.
	LineDrawer();
	~LineDrawer();
	void Draw() override;
	void SetToDraw(glm::vec3 point1, glm::vec3 point2, glm::vec4 color, unsigned int size);
	void SetPoints(glm::vec3 point1, glm::vec3 point2);
};

