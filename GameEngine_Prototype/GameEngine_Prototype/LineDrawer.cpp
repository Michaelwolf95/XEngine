#include "LineDrawer.h"
#include "RenderManager.h"
#include "ApplicationManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

LineDrawer::LineDrawer()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

LineDrawer::~LineDrawer() {}


// Render Manager calls this, then draws all used ones during render.
void LineDrawer::SetToDraw(glm::vec3 point1, glm::vec3 point2, glm::vec4 _color, unsigned int _size)
{
	isUsed = true;
	SetPoints(point1, point2);
	color = _color;
	size = _size;
}

void LineDrawer::SetPoints(glm::vec3 point1, glm::vec3 point2)
{
	// SAVES MEMORY
	verts[0] = point1.x;
	verts[1] = point1.y;
	verts[2] = point1.z;
	verts[3] = point2.x;
	verts[4] = point2.y;
	verts[5] = point2.z;
}

void LineDrawer::Draw()
{
	glClear(GL_DEPTH_BUFFER_BIT); // Clears the depth buffer so we can draw on top.

	RenderManager::colorDrawShader->use();
	RenderManager::colorDrawShader->setColor("MainColor", color.r, color.g, color.b, color.a);

	//glm::vec3 point3 = vec3(point.x, point.y, 0.2);
	glm::mat4 model(1.0);
	//model = glm::translate(model, point3);
	glm::mat4 view(1.0);
	glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	RenderManager::colorDrawShader->setMat4("model", model);
	RenderManager::colorDrawShader->setMat4("view", view);
	RenderManager::colorDrawShader->setMat4("projection", projection);
	glLineWidth(size);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	isUsed = false;
}