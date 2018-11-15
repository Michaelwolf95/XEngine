#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtx/transform.hpp>
#include "DebugUtility.h"
using namespace glm;

Transform::Transform() : Component::Component()
{
	model = glm::mat4(1.0f);
}

Transform::~Transform() {}

void Transform::SetParent(Transform * _parent)
{
	// ToDo: Make sure the object isn't a child.
	parent = _parent;
}


void Transform::Start()
{

}
void Transform::Update()
{

}

glm::mat4 Transform::getMatrix4x4()
{
	return model;
}

glm::vec3 Transform::getPosition()
{
	return glm::vec3(model[3]);
}

void Transform::setPosition(glm::vec3 pos)
{
	//ToDo: Optimize this
	model[3].x = pos.x;
	model[3].y = pos.y;
	model[3].z = pos.z;
	//model[3].w = pos.w;
	//model = translate(mat4(1.0f), pos);
}
// NONE OF THESE DO ANYTHING YET!!!! =====

glm::quat Transform::getLocalRotation() // Local
{
	// This is the rotation matrix. It needs to be converted into a quaternion.
	mat4 rotMat = getRotationMatrix();

	return quat_cast(rotMat);
}

void Transform::setLocalRotation(glm::quat rot)
{
	//model = glm::rotate(mat4(1.0f), rot);
}

glm::vec3 Transform::getLocalScale()
{
	return glm::vec3(model[0][0], model[1][1], model[2][2]);
}

void Transform::setLocalScale(glm::vec3 scale)
{
	// M = T*R*S
	//printTransformMatrix();

	mat4 transMat = getTranslationMatrix();
	//std::cout << "Trans" <<std::endl;
	EngineDebug::PrintMatrix(transMat);
	mat4 rotMat = getRotationMatrix();
	//std::cout << "Rot" << std::endl;
	//EngineDebug::PrintMatrix(rotMat);

	mat4 scaleMat(1.0);
	scaleMat[0][0] = scale.x;
	scaleMat[1][1] = scale.y;
	scaleMat[2][2] = scale.z;
	//std::cout << "Scale" << std::endl;
	//EngineDebug::PrintMatrix(scaleMat);

	model = transMat * rotMat * scaleMat;
	//std::cout << std::endl;
	//printTransformMatrix();
}

glm::mat4 Transform::getTranslationMatrix()
{
	mat4 transMat(1.0);
	transMat[3][0] = model[3][0];
	transMat[3][1] = model[3][1];
	transMat[3][2] = model[3][2];
	EngineDebug::PrintMatrix(transMat);
	return transMat;
}

glm::mat4 Transform::getRotationMatrix()
{
	glm::mat4 rotMat;
	vec4 scaleVec = vec4(getLocalScale(), 0.0);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			rotMat[i][j] = scaleVec[i] * model[i][j];
		}
	}
	rotMat[3] = vec4(0.0, 0.0, 0.0, 1.0);
	return rotMat;
}

glm::mat4 Transform::getScaleMatrix()
{
	mat4 scaleMat(1.0);
	scaleMat[0][0] = model[0][0];
	scaleMat[1][1] = model[1][1];
	scaleMat[2][2] = model[2][2];
	return scaleMat;
}

/* https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
right   = glm::vec3(matrix[0][0], matrix[1][0], matrix[2][0]);
up      = glm::vec3(matrix[0][1], matrix[1][1], matrix[2][1]);
forward = glm::vec3(matrix[0][2], matrix[1][2], matrix[2][2]);
*/

glm::vec3 Transform::getRightDirection()
{
	return glm::vec3(model[0][0], model[1][0], model[2][0]);
}
glm::vec3 Transform::getUpDirection()
{
	return glm::vec3(model[0][1], model[1][1], model[2][1]);

}
glm::vec3 Transform::getForwardDirection()
{
	return glm::vec3(model[0][2], model[1][2], model[2][2]);
}

void Transform::printTransformMatrix()
{
	//mat4 tModel = glm::transpose(model);
	std::cout << gameObject->name <<" Transform:\n";
	for (size_t i = 0; i < 4; i++) // Flip i nad j
	{
		std::cout << ((i == 0) ? "[ " : "| ");
		for (size_t j = 0; j < 4; j++)
		{
			std::cout << model[j][i] << ((j == 3) ? "" : "\t");
		}
		std::cout << ((i == 3) ? " ]\n" : " |\n");
	}
	
}