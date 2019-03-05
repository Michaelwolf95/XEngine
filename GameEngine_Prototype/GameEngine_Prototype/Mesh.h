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

using namespace std;

/// Based on LearnOpenGL.com section on Meshes

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

struct Texture {
	// texture id
	unsigned int id;
	// texture type
	string type;
	// texture path
	string path;
};

class Mesh
{
	public:
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		// Constructor
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		
		// Deconstructor
		~Mesh();

		void Setup();

};


#endif // !1