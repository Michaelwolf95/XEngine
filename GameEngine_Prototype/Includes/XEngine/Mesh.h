#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Component.h"
#include "Material.h"
#include "Vertex.h"
//#include "MeshRenderer.h"
class MeshRenderer;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

//using namespace std;

/// Based on LearnOpenGL.com section on Meshes



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