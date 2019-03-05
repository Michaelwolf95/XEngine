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
	glDeleteVertexArrays(1, &this->quadVAO);
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
	//return (gameObject->transform->getMatrix4x4());
}

void CameraComponent::Start() {}
void CameraComponent::Update() {}

void CameraComponent::OnDrawGizmos()
{
	return;
	/*
	//return;
	// Prepare transformations
	this->shader->use();
	glm::mat4 model = glm::mat4(1.0);
	//model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
	//model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	glm::mat4 projection = RenderManager::getInstance().getProjection();
	glm::mat4 view = RenderManager::getInstance().getView();
	glm::vec3 pos = this->gameObject->transform->getPosition();
	glm::vec4 BillboardPos_worldspace(pos.x, pos.y, pos.z, 1.0f);
	glm::vec4 BillboardPos_screenspace = projection * view * BillboardPos_worldspace;
	BillboardPos_screenspace /= BillboardPos_screenspace.w;

	//glm::mat4 model = glm::mat4(1.0);
	model[3].x = BillboardPos_screenspace.x;
	model[3].y = BillboardPos_screenspace.y;
	model[3].z = BillboardPos_screenspace.z;
	model[0][0] = 0.25f;
	model[1][1] = 0.25f;
	model[2][2] = 0.25f;
	//this->gameObject->transform->getMatrix4x4()

	//model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
	this->shader->setMat4("projection", projection);
	this->shader->setMat4("view", view);
	this->shader->setMat4("model", model);

	// Render textured quad
	this->shader->setVec4("MainColor", glm::vec4(1.0,1.0,1.0, 1.0));

	//glActiveTexture(GL_TEXTURE0);
	//texture.Bind();
	//glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);
	*/
	// Vertex shader
	unsigned int programID = this->shader->ID;
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");
	GLuint BillboardPosID = glGetUniformLocation(programID, "BillboardPos");
	GLuint BillboardSizeID = glGetUniformLocation(programID, "BillboardSize");
	//GLuint LifeLevelID = glGetUniformLocation(programID, "LifeLevel");

	this->shader->use();
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 ProjectionMatrix = RenderManager::getInstance().getProjection();
	glm::mat4 ViewMatrix = RenderManager::getInstance().getView();
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	glBindVertexArray(this->quadVAO);

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	//glUniform1i(TextureID, 0);
	// This is the only interesting part of the tutorial.
		// This is equivalent to mlutiplying (1,0,0) and (0,1,0) by inverse(ViewMatrix).
		// ViewMatrix is orthogonal (it was made this way), 
		// so its inverse is also its transpose, 
		// and transposing a matrix is "free" (inversing is slooow)
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniform3f(BillboardPosID, 0.0f, 0.5f, 0.0f); // The billboard will be just above the cube
	glUniform2f(BillboardSizeID, 1.0f, 0.125f);     // and 1m*12cm, because it matches its 256*32 resolution =)

	//// Generate some fake life level and send it to glsl
	//float LifeLevel = sin(currentTime)*0.1f + 0.7f;
	//glUniform1f(LifeLevelID, LifeLevel);

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadVAO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);


	// Draw the billboard !
	// This draws a triangle_strip which looks like a quad.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	
}

void CameraComponent::initGizmoRenderData()
{
	shader = RenderManager::defaultSpriteShader;
	// Configure VAO/VBO
	//GLuint VBO;
	//GLfloat vertices[] = {
	//	// Pos      // Tex
	//	0.0f, 1.0f, 0.0f, 1.0f,
	//	1.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 0.0f,

	//	0.0f, 1.0f, 0.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f, 1.0f,
	//	1.0f, 0.0f, 1.0f, 0.0f
	//};

	//glGenVertexArrays(1, &this->quadVAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindVertexArray(this->quadVAO);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//GLuint Texture = loadDDS("ExampleBillboard.DDS");

	//glBindVertexArray(this->quadVAO);

	// The VBO containing the 4 vertices of the particles.
	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};
	GLuint VBO;
	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

}
