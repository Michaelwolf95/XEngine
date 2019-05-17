#pragma once
#include <glm/glm.hpp>

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