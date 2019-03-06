#include "CameraComponent.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RenderManager.h"
#include "AssetManager.h"

REGISTER_COMPONENT(CameraComponent, "CameraComponent")

CameraComponent::CameraComponent() 
{
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f),
		(float)ApplicationManager::config->screenWidth / (float)ApplicationManager::config->screenHeight,
		0.1f, 100.0f);
	initGizmoRenderData();
}

CameraComponent::~CameraComponent() 
{
	if (RenderManager::getInstance().getCurrentCamera() == this)
	{
		RenderManager::getInstance().setCurrentCamera(nullptr);
	}
	// Delete Gizmo
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteVertexArrays(1, &this->VBO);
}

glm::mat4 CameraComponent::getProjection()
{
	return projection;
}

glm::mat4 __stdcall CameraComponent::getView()
{
	//OutputDebugStringW(L"Getting CameraComponent View...\n");
	//std::cout << "Getting CameraComponent View..." << std::endl;
	glm::vec3 cameraPos = gameObject->transform->getPosition();
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + gameObject->transform->getForwardDirection(), gameObject->transform->getUpDirection());

	return view;
}
//
//glm::vec3 CameraComponent::getPosition()
//{
//	return gameObject->transform->getPosition();
//}

void CameraComponent::Start() {}
void CameraComponent::Update() {}



void CameraComponent::initGizmoRenderData()
{
	shader = RenderManager::defaultSpriteShader;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	AssetManager::getInstance().LoadTextureAsset("Editor/Gizmos/CameraGizmo.png", &textureID, 4); //4 for alpha
	//AssetManager::getInstance().LoadTexture("../Editor/Gizmos/TestTexture.jpg", &textureID, 3);

	// The VBO containing the 4 vertices of the particles.
	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};
	//GLuint billboard_vertex_buffer;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void CameraComponent::OnDrawGizmos()
{
	// Vertex shader
	unsigned int programID = this->shader->ID;
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");
	GLuint BillboardPosID = glGetUniformLocation(programID, "BillboardPos");
	GLuint BillboardSizeID = glGetUniformLocation(programID, "BillboardSize");
	GLuint TexturePropID = glGetUniformLocation(programID, "TextureSampler");

	glm::vec3 pos = this->gameObject->transform->getPosition();
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 ProjectionMatrix = RenderManager::getInstance().getProjection();
	glm::mat4 ViewMatrix = RenderManager::getInstance().getView();
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->shader->use();

	glm::vec4 color(1.0f, 1.0f, 1.0f, 0.8f);
	shader->setColor("MainColor", color.r, color.g, color.b, color.a);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TexturePropID, 0);


	// This is the only interesting part of the tutorial.
		// This is equivalent to mlutiplying (1,0,0) and (0,1,0) by inverse(ViewMatrix).
		// ViewMatrix is orthogonal (it was made this way), 
		// so its inverse is also its transpose, 
		// and transposing a matrix is "free" (inversing is slooow)
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniform3f(BillboardPosID, pos.x, pos.y, pos.z); // The billboard will be just above the cube
	glUniform2f(BillboardSizeID, 0.1f, 0.1f);     // and 1m*12cm, because it matches its 256*32 resolution =)

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	
	glBindVertexArray(VAO);

	// Draw the billboard !
	// This draws a triangle_strip which looks like a quad.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);

	//glDisableVertexAttribArray(0);
	glDisable(GL_BLEND);
	
}
