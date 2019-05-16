#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Shader.h"
#include "Material.h"
#include "RenderableObject.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "GameObject.h"
//class MeshRenderer ;
#include "MeshRenderer.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

//using namespace std;

/// Based on LearnOpenGL.com section on Meshes

// Constructor
Mesh::Mesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->name = name;
	this->vertices = vertices;
	this->indices = indices;

	Setup();
}

// Deconstructor
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Setup()
{
	//std::cout << "Mesh Setup." << std::endl;
	// create buffersmaterial	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// pass pointer to struct, translate to glm vec2/3, which translate to 2/3 floats that translate to byte array
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	//std::cout << "size of all vertices: " << vertices.size() * sizeof(Vertex) << std::endl;


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set vertex attributes pointers
	// position vertex
	//std::cout << "size of vertex: " << sizeof(Vertex) << std::endl;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//std::cout << "Offset: " << 0 << std::endl;
	glEnableVertexAttribArray(0);

	// normal vertex
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//std::cout << "Offset Normal: " << offsetof(Vertex, Normal) << std::endl;
	glEnableVertexAttribArray(1);

	// coordinate vector
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	//std::cout << "Offset texCoords: " << offsetof(Vertex, TexCoords) << std::endl;
	glEnableVertexAttribArray(2);






	// tangent vector
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	//std::cout << "Offset Normal: " << offsetof(Vertex, Normal) << std::endl;
	glEnableVertexAttribArray(3);

	// bitangent vector
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	//std::cout << "Offset Normal: " << offsetof(Vertex, Normal) << std::endl;
	glEnableVertexAttribArray(4);

	glBindVertexArray(0); // !VAO
}





