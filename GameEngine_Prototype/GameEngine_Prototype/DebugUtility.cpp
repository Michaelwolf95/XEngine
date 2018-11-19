#include "DebugUtility.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>

namespace EngineDebug
{
	void PrintMatrix(glm::mat4 matrix)
	{
		// Flip i nad j for transpose... which is what we normally use.
		for (size_t i = 0; i < 4; i++)
		{
			std::cout << ((i == 0) ? "[ " : "| ");
			for (size_t j = 0; j < 4; j++)
			{
				std::cout << matrix[j][i] << ((j == 3) ? "" : "\t");
			}
			std::cout << ((i == 3) ? " ]\n" : " |\n");
		}
	}
}