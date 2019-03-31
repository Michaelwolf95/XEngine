#include "RenderableObject.h"
#include "RenderManager.h"
#include "Vertex.h"

//RenderableObject::RenderableObject()
//{
//}

//RenderableObject::RenderableObject(Material * _material)
//{
//	std::cout << "Warning: RenderableObject attributes are at NULL\n";
//	this->vertices = NULL;
//	this->numVerts = 0;
//	this->vertexDataSize = 0;
//	this->indices = NULL;
//	this->numIndices = 0;
//
//	
//	render_enabled = true;
//	if (_material == nullptr)
//	{
//		std::cout << "Assigning default material\n";
//		material = RenderManager::defaultMaterial;
//	}
//	else
//	{
//		material = _material;
//	}
//
//	//this->Setup(); // NO VIRTUAL FUNCTIONS IN CONSTRUCTOR
//	std::cout << "Created Object with shader ID: " << material->shader->ID << std::endl;
//
//	RenderManager::getInstance().AddRenderable((RenderableObject*)this);
//}

//RenderableObject::RenderableObject(float* verts, unsigned int numV, unsigned int vertDataSize,
//	unsigned int* ind, unsigned int numInd, Material* _material)
//{
//	vertices = verts;
//	std::cout << "Vertices: " << vertices << std::endl;
//	numVerts = numV;
//	std::cout << "numVerts: " << numVerts << std::endl;
//	vertexDataSize = vertDataSize;
//	std::cout << "vertexDataSize: " << vertexDataSize << std::endl;
//	indices = ind;
//	std::cout << "indices: " << indices << std::endl;
//	numIndices = numInd;
//	std::cout << "numIndices: " << numIndices << std::endl;
//	render_enabled = true;
//	if (_material == nullptr)
//	{
//		material = RenderManager::defaultMaterial;
//	}
//	else
//	{
//		material = _material;
//	}
//
//	//this->Setup(); // NO VIRTUAL FUNCTIONS IN CONSTRUCTOR
//	std::cout << "Created Object with shader ID: " << material->shader->ID << std::endl;
//
//	RenderManager::getInstance().AddRenderable((RenderableObject*)this);
//}
//RenderableObject::~RenderableObject()
//{
//	// optional: de-allocate all resources once they've outlived their purpose:
//	// ------------------------------------------------------------------------
//	glDeleteVertexArrays(1, &(VAO));
//	glDeleteBuffers(1, &(VBO));
//	glDeleteBuffers(1, &(EBO));
//
//	//RenderManager::getInstance().RemoveRenderable((RenderableObject*)this);
//}

//void RenderableObject::Setup()
//{
//	std::cout << "***********Renderable Setup." << std::endl;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, vertexDataSize*numVerts*sizeof(float), vertices, GL_STATIC_DRAW);
//	std::cout << "size of all vertices: " << vertexDataSize * numVerts * sizeof(float) << std::endl;
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), indices, GL_STATIC_DRAW);
//
//	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
//	//glEnableVertexAttribArray(0);
//
//	//// next two glvertextattribpointer and enablevertex calls for normals and textures coords -- DM
//	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	//glEnableVertexAttribArray(1);
//
//	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	//glEnableVertexAttribArray(2);
//
//
//
//
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//	std::cout << "Offset: " << 0 << std::endl;
//	glEnableVertexAttribArray(0);
//
//	// normal vertex
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//	std::cout << "Offset Normal: " << offsetof(Vertex, Normal) << std::endl;
//	glEnableVertexAttribArray(1);
//
//	// coordinate vector
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//	std::cout << "Offset texCoords: " << offsetof(Vertex, TexCoords) << std::endl;
//	glEnableVertexAttribArray(2);
//
//	// tangent vector
//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//	//std::cout << "Offset Normal: " << offsetof(Vertex, Normal) << std::endl;
//	glEnableVertexAttribArray(3);
//
//	// bitangent vector
//	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
//	//std::cout << "Offset Normal: " << offsetof(Vertex, Normal) << std::endl;
//	glEnableVertexAttribArray(4);
//
//
//
//
//
//
//
//
//	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//	glBindVertexArray(0);
//
//	// uncomment this call to draw in wireframe polygons.
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//}
//
//void RenderableObject::Draw()
//{
//	//std::cout << "RenderableObject.Draw()" << std::endl;
//	if (render_enabled = true)
//	{
//		glBindVertexArray(VAO);
//		//glDrawArrays(GL_TRIANGLES, 0, 6);
//		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		//std::cout << "numVerts == " << numVerts << ", vertexDataSize == " << vertexDataSize << std::endl;
//		glDrawElements(GL_TRIANGLES, this->numVerts, GL_UNSIGNED_INT, 0);
//		//glBindVertexArray(0); 
//	}
//}
