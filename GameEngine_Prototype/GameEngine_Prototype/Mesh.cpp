#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Shader.h"
#include "Material.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

/// Based on LearnOpenGL.com section on Meshes

// Constructor
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Material* material)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures; 
	this->material = material;

	Setup();
}

// Deconstructor
Mesh::~Mesh()
{
}

void Mesh::Setup()
{
	// create buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// pass pointer to struct, translate to glm vec2/3, which translate to 2/3 floats that translate to byte array
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set vertex attributes pointers
	// position vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// normal vertex
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// coordinate vector
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// tangent vector
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	// bitangent vector
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
}

void Mesh::Draw()
{
	// texture variables
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	// binding textures
	for (unsigned int i = 0; i < textures.size(); i++) 
	{
		// get texture before binding
		glActiveTexture(GL_TEXTURE0 + 1);

		string number;
		string name = textures[i].type;

		// transfer unsigned to stream
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		// set texture unit
		glUniform1i(glGetUniformLocation(material->shader->ID, (name + number).c_str()), i);

		// bind texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// default once configured
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Start()
{
}

void Mesh::Update()
{
}


