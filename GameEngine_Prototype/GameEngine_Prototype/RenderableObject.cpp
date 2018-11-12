#include "RenderableObject.h"
#include "RenderManager.h"

RenderableObject::RenderableObject(float* verts, unsigned int numV, unsigned int vertDataSize, 
	unsigned int* ind, unsigned int numInd, Shader* _shader)
{
	vertices = verts;
	numVerts = numV;
	vertexDataSize = vertDataSize;
	indices = ind;
	numIndices = numInd;
	enabled = true;
	if (_shader == nullptr)
	{
		shader = RenderManager::defaultShader;
	}
	else
	{
		shader = _shader;
	}

	//this->Setup(); // NO VIRTUAL FUNCTIONS IN CONSTRUCTOR
	std::cout << "Created Object with shader ID: " << shader->ID << std::endl;

	RenderManager::instance->AddRenderable((RenderableObject*)this);
}
RenderableObject::~RenderableObject()
{
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &(VAO));
	glDeleteBuffers(1, &(VBO));
	glDeleteBuffers(1, &(EBO));
}

void RenderableObject::Setup()
{
	std::cout << "Renderable Setup." << std::endl;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize*numVerts*sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RenderableObject::Draw()
{
	//std::cout << "RenderableObject.Draw()" << std::endl;
	if (enabled)
	{
		//shader->use();

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); 
	}
}
