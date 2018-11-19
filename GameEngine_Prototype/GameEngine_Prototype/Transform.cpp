#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtx/transform.hpp>
#include "RenderManager.h"
#include "DebugUtility.h"
using namespace glm;

// TODO: Track the different transformation matrices seperately to save on calculations.
// That is - don't track the whole model. Translation, scale, and rotation seperately.
Transform::Transform() : Component::Component()
{
	model = glm::mat4(1.0f);
}

Transform::~Transform() {}

void Transform::SetParent(Transform * _parent)
{
	// TODO: Make sure the object isn't a child.
	parent = _parent;
}


void Transform::Start() {}
void Transform::Update() {}

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
	model[3].x = pos.x;
	model[3].y = pos.y;
	model[3].z = pos.z;
}

glm::quat Transform::getLocalRotation() // Local
{
	// This is the rotation matrix. It needs to be converted into a quaternion.
	//mat4 rotMat = glm::transpose(getRotationMatrix());
	mat4 rotMat = glm::inverse(getRotationMatrix());

	return quat_cast(rotMat);
}

// Returns the current local euler rotation in DEGREES.
glm::vec3 Transform::getLocalRotationEuler()
{
	vec3 rot = glm::eulerAngles(getLocalRotation());
	rot[0] = glm::degrees(rot[0]);
	rot[1] = glm::degrees(rot[1]);
	rot[2] = glm::degrees(rot[2]);
	return rot;
}

void Transform::setLocalRotation(glm::quat rot)
{
	// M = T*R*S
	mat4 newRotMat = glm::mat4_cast(rot);
	mat4 scaleMat = getScaleMatrix();
	mat4 transMat = getTranslationMatrix();

	model = (transMat * newRotMat) * scaleMat;
}

// Set rotation in terms of euler DEGREES.
void Transform::setLocalRotationEuler(glm::vec3 rot)
{
	for (size_t i = 0; i < 3; i++)
	{
		if (rot[i] > 360)
		{
			int numOver = ((int)rot[i]) / 360;
			rot[i] = rot[i] - (360 * numOver);
		}
	}
	rot[0] = glm::radians(rot[0]); 
	rot[1] = glm::radians(rot[1]);
	rot[2] = glm::radians(rot[2]);
	quat qRot(rot);
	setLocalRotation(qRot);
}

glm::vec3 Transform::getLocalScale()
{
	vec3 Scale;
	vec3 Skew;
	mat3 Row;
	vec3 Pdum3;

	// Now get scale and shear.
	for (length_t i = 0; i < 3; ++i)
		for (length_t j = 0; j < 3; ++j)
			Row[i][j] = model[i][j];

	// Compute X scale factor and normalize first row.
	Scale.x = length(Row[0]);// v3Length(Row[0]);

	Row[0] = detail::scale(Row[0], (float)(1));

	// Compute XY shear factor and make 2nd row orthogonal to 1st.
	Skew.z = dot(Row[0], Row[1]);
	Row[1] = detail::combine(Row[1], Row[0], (float)(1), -Skew.z);

	// Now, compute Y scale and normalize 2nd row.
	Scale.y = length(Row[1]);
	Row[1] = detail::scale(Row[1], (float)(1));
	Skew.z /= Scale.y;

	// Compute XZ and YZ shears, orthogonalize 3rd row.
	Skew.y = glm::dot(Row[0], Row[2]);
	Row[2] = detail::combine(Row[2], Row[0], (float)(1), -Skew.y);
	Skew.x = glm::dot(Row[1], Row[2]);
	Row[2] = detail::combine(Row[2], Row[1], (float)(1), -Skew.x);

	// Next, get Z scale and normalize 3rd row.
	Scale.z = length(Row[2]);
	Row[2] = detail::scale(Row[2], (float)(1));
	Skew.y /= Scale.z;
	Skew.x /= Scale.z;

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
	Pdum3 = glm::cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
	if (glm::dot(Row[0], Pdum3) < 0)
	{
		for (length_t i = 0; i < 3; i++)
		{
			Scale[i] *= (float)(-1);
			Row[i] *= (float)(-1);
		}
	}
	return Scale;
	//return glm::vec3(model[0][0], model[1][1], model[2][2]);
}

void Transform::setLocalScale(glm::vec3 scale)
{
	// M = T*R*S
	mat4 scaleMat(1.0);
	scaleMat[0][0] = scale.x;
	scaleMat[1][1] = scale.y;
	scaleMat[2][2] = scale.z;

	//model = transMat * rotMat * scaleMat;
	model = (model * inverse(getScaleMatrix())) * scaleMat;
}

glm::mat4 Transform::getTranslationMatrix()
{
	mat4 transMat(1.0);
	transMat[3][0] = model[3][0];
	transMat[3][1] = model[3][1];
	transMat[3][2] = model[3][2];
	//::PrintMatrix(transMat);
	return transMat;
}

glm::mat4 Transform::getRotationMatrix()
{
	// M = T*R*S
	// R = inv(T)*M*inv(S)
	glm::mat4 transMat = getTranslationMatrix();
	glm::mat4 scaleMat = getScaleMatrix();
	glm::mat4 rotMat = glm::inverse(transMat)*(model*glm::inverse(scaleMat));

	//return rotMat;
	return glm::transpose(rotMat);
}

glm::mat4 Transform::getScaleMatrix()
{
	mat4 scaleMat(1.0);
	
	vec3 Scale = getLocalScale();
	scaleMat[0][0] = Scale.x;
	scaleMat[1][1] = Scale.y;
	scaleMat[2][2] = Scale.z;

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

void Transform::lookAt(glm::vec3 lookPos, glm::vec3 up)
{
	glm::vec3 pos = getPosition();
	model = glm::lookAt(pos, lookPos, up);
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

void Transform::DrawGizmo()
{
	glm::vec3 pos = gameObject->transform->getPosition();
	glm::mat4 model = gameObject->transform->model;
	glm::mat4 rotMat = getRotationMatrix();
	//RenderManager::DrawWorldSpacePoint(pos, vec4(1, 1, 1, 1), 5);
	vec3 right = vec3(vec4(1, 0, 0, 0)*rotMat);
	vec3 up = vec3(vec4(0, 1, 0, 0)*rotMat);
	vec3 forward = vec3(vec4(0, 0, 1, 0)*rotMat);
	float L = 0.5;
	float sL = 0.1;
	RenderManager::DrawWorldSpaceLine(pos, pos + right*L, vec4(1, 0, 0, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + right * L, vec4(1, 0, 0, 1), 5); // Head

	RenderManager::DrawWorldSpaceLine(pos, pos + up * L, vec4(0, 1, 0, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + up * L, vec4(0, 1, 0, 1), 5);

	RenderManager::DrawWorldSpaceLine(pos, pos + forward * L, vec4(0, 0, 1, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + forward * L, vec4(0, 0, 1, 1), 5);

	// Squares
	RenderManager::DrawWorldSpaceLine(pos + right*sL, pos + vec3(vec4(sL, 0, sL, 0)*rotMat), vec4(0.5, 0.5, 0.5, 1), 3);
	RenderManager::DrawWorldSpaceLine(pos + forward*sL, pos + vec3(vec4(sL, 0, sL, 0)*rotMat), vec4(0.5, 0.5, 0.5, 1), 3);

}