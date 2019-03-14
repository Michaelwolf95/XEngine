#ifndef MESH_H
#define MESH_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Component.h"
#include "Material.h"
//#include "MeshRenderer.h"
class MeshRenderer;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

//using namespace std;

/// Based on LearnOpenGL.com section on Meshes

// stride = 14
struct Vertex {
	// position vector
	glm::vec3 Position;
	// normal vector
	glm::vec3 Normal;
	// texture coordinate vector
	glm::vec2 TexCoords;
	// tangent vector
	glm::vec3 Tangent;
	// bitangent vector 
	glm::vec3 Bitangent;
};



class Mesh
{
	public:
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::string name;
		
		// Constructor
		Mesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		
		// Deconstructor
		~Mesh();

		void Setup();
};


#endif // !1