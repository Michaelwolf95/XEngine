#include "GizmoSpriteDrawer.h"
#include "ApplicationManager.h"

float GizmoSpriteDrawer::gizmoScale = 0.25f;
const GLfloat GizmoSpriteDrawer::spriteQuadVerices[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
};

GizmoSpriteDrawer::GizmoSpriteDrawer(const char* _texturePath)
{
	//std::cout << "Constructing Sprite Drawer ==============================" << std::endl;
	gizmoTexturePath = _texturePath;
	color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
	Setup();
}

GizmoSpriteDrawer::~GizmoSpriteDrawer() 
{
	//std::cout << "Deconstructing Sprite Drawer ==============================" << std::endl;
	// Delete Gizmo
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteVertexArrays(1, &this->VBO);
}

void GizmoSpriteDrawer::Setup()
{
	//std::cout << "Setting up Sprite Drawer: " << gizmoTexturePath << std::endl;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	AssetManager::getInstance().LoadTextureAsset(gizmoTexturePath.c_str(), &textureID, 4); //4 for alpha

	//GLuint billboard_vertex_buffer;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spriteQuadVerices), spriteQuadVerices, GL_DYNAMIC_DRAW);

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

void GizmoSpriteDrawer::Draw(glm::vec3 position)
{
	if (ApplicationManager::getInstance().IsEditMode() == false)
	{
		return;
	}
	//std::cout << "Drawing Sprite Gizmo" << std::endl;
	Shader* shader = RenderManager::defaultSpriteShader;

	// Vertex shader
	unsigned int programID = shader->ID;
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");
	GLuint BillboardPosID = glGetUniformLocation(programID, "BillboardPos");
	GLuint BillboardSizeID = glGetUniformLocation(programID, "BillboardSize");
	GLuint TexturePropID = glGetUniformLocation(programID, "TextureSampler");

	glm::vec3 pos = position;
	//glm::mat4 model = glm::mat4(1.0);
	//std::cout << "pos:   (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	glm::mat4 ProjectionMatrix = RenderManager::getInstance().getProjection();
	glm::mat4 ViewMatrix = RenderManager::getInstance().getView();
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->use();

	//glm::vec4 color(1.0f, 1.0f, 1.0f, 0.8f);
	//glm::vec4 color(1.0f, 1.0f, 1.0f, 0.8f);
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
	glUniform2f(BillboardSizeID, gizmoScale, gizmoScale);     // and 1m*12cm, because it matches its 256*32 resolution =)

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);


	glBindVertexArray(VAO);

	// Draw the billboard !
	// This draws a triangle_strip which looks like a quad.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);

	//glDisableVertexAttribArray(0);
	glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
}
