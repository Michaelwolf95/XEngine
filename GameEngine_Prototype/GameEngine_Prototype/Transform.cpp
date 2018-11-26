#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/transform.hpp>
#include "RenderManager.h"
#include "DebugUtility.h"
using namespace glm;

//http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
// Order: Yaw, Roll, Pitch
static void toEulerAngle(const glm::quat& q, float& pitch, float& yaw, float& roll)
{
	double test = ((double)q.x*(double)q.y) + ((double)q.z*(double)q.w);
	//double test = (double)q.x*(double)q.w + (double)q.y*(double)q.z;
	if (test > 0.4999) { // singularity at north pole
		yaw = 2 * atan2(q.x, q.w);
		roll = glm::pi<float>() / 2;
		pitch = 0;
		std::cout << "Singularity North: " << test << ", " << q.w << std::endl;
		std::cout << "Rot:  (" << pitch << ", " << yaw << ", " << roll << ")" << std::endl;
		return;
	}
	if (test < -0.4999) { // singularity at south pole
		yaw = -2 * atan2(q.x, q.w);
		roll = -glm::pi<float>() / 2;
		pitch = 0;
		std::cout << "Singularity South: " << test << ", " << q.w << std::endl;
		std::cout << "Rot:  (" << pitch << ", " << yaw << ", " << roll << ")" << std::endl;
		return;
	}

	double sqx = ((double)q.x)*((double)q.x);
	double sqy = ((double)q.y)*(double)q.y;
	double sqz = ((double)q.z)*(double)q.z;
	yaw = atan2(2 * q.y*q.w - 2 * q.x*q.z, 1 - 2 * sqy - 2 * sqz);
	roll = asin(2 * ((double)q.x*(double)q.y + (double)q.z*(double)q.w));
	pitch = atan2(2 * q.x*q.w - 2 * q.y*q.z, 1 - 2 * sqx - 2 * sqz);
}
//https://answers.unity.com/questions/1324251/quaternion-to-euler-conversion-singularities.html
static glm::vec3 toEulerAngles(glm::quat q1)
{
	float sqw = q1.w * q1.w;
	float sqx = q1.x * q1.x;
	float sqy = q1.y * q1.y;
	float sqz = q1.z * q1.z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = q1.x * q1.w - q1.y * q1.z;
	glm::vec3 v;

	if (test > 0.4995f * unit)
	{ // singularity at north pole
		v.y = 2.0f * atan2(q1.y, q1.x);
		v.x = glm::pi<float>() / 2.0;
		v.z = 0;
		return v;
	}
	if (test < -0.4995f * unit)
	{ // singularity at south pole
		v.y = -2.0f * atan2(q1.y, q1.x);
		v.x = -glm::pi<float>() / 2;
		v.z = 0;
		return v;
	}

	glm::quat q(q1.w, q1.z, q1.x, q1.y);
	v.z = (float)atan2(2.0f * q.x * q.y + 2.0f * q.z * q.w, 1 - 2.0f * (q.y * q.y + q.z * q.z));     // Roll
	v.y = (float)atan2(2.0f * q.x * q.w + 2.0f * q.y * q.z, 1 - 2.0f * (q.z * q.z + q.w * q.w));     // Yaw
	v.x = (float)asin(2.0f * (q.x * q.z - q.w * q.y));    // Pitch     
	return v;
}

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

void Transform::setLocalPosition(glm::vec3 pos)
{
	translateMatrix[3].x = pos.x;
	translateMatrix[3].y = pos.y;
	translateMatrix[3].z = pos.z;
	UpdateMatrix();
}

void Transform::setLocalPosition(float x, float y, float z)
{
	setLocalPosition(glm::vec3(x, y, z));
}

glm::quat Transform::getLocalRotation() // Local
{
	// This is the rotation matrix. It needs to be converted into a quaternion.
	mat4 rotMat = getRotationMatrix();
	return quat_cast(rotMat);
}

// Returns the current local euler rotation in DEGREES.
glm::vec3 Transform::getLocalRotationEuler()
{
	vec3 rot;// = glm::eulerAngles(getLocalRotation());
	toEulerAngle(glm::normalize(getLocalRotation()), rot.x, rot.y, rot.z);
	//vec3 rot = toEulerAngles(getLocalRotation());
	rot[0] = glm::degrees(rot[0]);
	rot[1] = glm::degrees(rot[1]);
	rot[2] = glm::degrees(rot[2]);
	return rot;
}

void Transform::setLocalRotation(glm::quat rot)
{
	rotateMatrix = glm::mat4_cast(rot);
	UpdateMatrix();
	return;
}

// Set rotation in terms of euler DEGREES.
void Transform::setLocalRotationEuler(glm::vec3 rot)
{
	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (rot[i] > 360)
	//	{
	//		int numOver = ((int)rot[i]) / 360;
	//		rot[i] = -(rot[i] - (360 * numOver));
	//	}
	//}
	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (rot[i] < -360)
	//	{
	//		int numOver = ((int)rot[i]) / -360;
	//		rot[i] = -(rot[i] + (360 * numOver));
	//	}
	//}
	rot.x = glm::radians(rot.x); 
	rot.y = glm::radians(rot.y);
	rot.z = glm::radians(rot.z);
	glm::quat rotQuat = glm::quat(rot);

	setLocalRotation(rotQuat);
}

void Transform::setLocalRotationEuler(float x, float y, float z)
{
	setLocalRotationEuler(glm::vec3(x, y, z));
}

glm::vec3 Transform::getLocalScale()
{
	return glm::vec3(scaleMatrix[0][0], scaleMatrix[1][1], scaleMatrix[2][2]);
}

void Transform::setLocalScale(glm::vec3 scale)
{
	scaleMatrix[0][0] = scale.x;
	scaleMatrix[1][1] = scale.y;
	scaleMatrix[2][2] = scale.z;

	UpdateMatrix();
}

void Transform::setLocalScale(float x, float y, float z)
{
	setLocalScale(glm::vec3(x, y, z));
}

void Transform::Translate(glm::vec3 translation)
{
	translateMatrix = glm::translate(translateMatrix, translation);
	UpdateMatrix();
}

void Transform::Rotate(glm::vec3 rotation)
{
	if (rotation.x != 0)
	{
		//model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	}
	if (rotation.y != 0)
	{
		//model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	}
	if (rotation.z != 0)
	{
		//model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	}
	UpdateMatrix();
}

void Transform::Scale(glm::vec3 scale)
{
	//model = glm::scale(model, scale);
	scaleMatrix = glm::scale(scaleMatrix, scale);
	UpdateMatrix();
}

void Transform::LookAt(glm::vec3 lookPos, glm::vec3 up)
{
	glm::vec3 pos = getPosition();
	//model = glm::lookAt(pos, lookPos, up);
	rotateMatrix = glm::lookAt(pos, lookPos, up);
	UpdateMatrix();
}

glm::mat4 Transform::getTranslationMatrix()
{
	return translateMatrix;
}

glm::mat4 Transform::getRotationMatrix()
{
	return rotateMatrix;
}

glm::mat4 Transform::getScaleMatrix()
{
	return scaleMatrix;
}

/* https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
right   = glm::vec3(matrix[0][0], matrix[1][0], matrix[2][0]);
up      = glm::vec3(matrix[0][1], matrix[1][1], matrix[2][1]);
forward = glm::vec3(matrix[0][2], matrix[1][2], matrix[2][2]);
*/

glm::vec3 Transform::getRightDirection()
{
	glm::mat4 mat = glm::inverse(rotateMatrix);
	return glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
}
glm::vec3 Transform::getUpDirection()
{
	glm::mat4 mat = glm::inverse(rotateMatrix);
	return glm::vec3(mat[0][1], mat[1][1], mat[2][1]);

}
glm::vec3 Transform::getForwardDirection()
{
	glm::mat4 mat = glm::inverse(rotateMatrix);
	return glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
}

void Transform::printTransformMatrix()
{
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

void Transform::UpdateMatrix()
{
	// M = T*R*S ... or S*R*T? ... or S*T*R??
	//http://headerphile.com/uncategorized/opengl-matrix-operations/
	model = scaleMatrix * translateMatrix * rotateMatrix;
	//model =  translateMatrix * rotateMatrix * scaleMatrix;
}

void Transform::OnDrawGizmo()
{
	glm::vec3 pos = getPosition();
	glm::mat4 rotMat = getRotationMatrix();
	//RenderManager::DrawWorldSpacePoint(pos, vec4(1, 1, 1, 1), 5);
	vec3 right = glm::normalize(getRightDirection());
	vec3 up = glm::normalize(getUpDirection());
	vec3 forward = glm::normalize(getForwardDirection());
	float L = 0.5;
	float sL = 0.1;
	RenderManager::DrawWorldSpaceLine(pos, pos + right*L, vec4(1, 0, 0, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + right * L, vec4(1, 0, 0, 1), 5); // Head

	RenderManager::DrawWorldSpaceLine(pos, pos + up * L, vec4(0, 1, 0, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + up * L, vec4(0, 1, 0, 1), 5);

	RenderManager::DrawWorldSpaceLine(pos, pos + forward * L, vec4(0, 0, 1, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + forward * L, vec4(0, 0, 1, 1), 5);

	// Gray Square
	RenderManager::DrawWorldSpaceLine(pos + right*sL, pos + (right+forward)*sL, vec4(0.5, 0.5, 0.5, 1), 3);
	RenderManager::DrawWorldSpaceLine(pos + forward*sL, pos + (right + forward)*sL, vec4(0.5, 0.5, 0.5, 1), 3);

}


