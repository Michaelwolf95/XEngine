#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/transform.hpp>
#include "RenderManager.h"
#include "DebugUtility.h"
using namespace glm;

BOOST_CLASS_EXPORT_GUID(Transform, "Transform")
Registrar<Transform> Transform::registrar("Transform"); //ComponentTypeInfo(std::string("Transform"), []() {return (Component*)(new Transform()); }));
//REGISTER_COMPONENT(Transform, "Transform")

//http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
// Order: Yaw, Roll, Pitch
static void toEulerAngles(const glm::quat& q, float& pitch, float& yaw, float& roll)
{
	double test = ((double)q.x*(double)q.y) + ((double)q.z*(double)q.w);
	//double test = (double)q.x*(double)q.w + (double)q.y*(double)q.z;
	if (test > 0.4999) { // singularity at north pole
		yaw = 2 * atan2f(q.x, q.w);
		roll = glm::pi<float>() / 2;
		pitch = 0;
		std::cout << "Singularity North: " << test << ", " << q.w << std::endl;
		std::cout << "\tResolved Rot:  (" << pitch << ", " << yaw << ", " << roll << ")" << std::endl;
		return;
	}
	if (test < -0.4999) { // singularity at south pole
		yaw = -2 * atan2f(q.x, q.w);
		roll = -glm::pi<float>() / 2;
		pitch = 0;
		std::cout << "Singularity South: " << test << ", " << q.w << std::endl;
		std::cout << "\tResolved Rot:  (" << pitch << ", " << yaw << ", " << roll << ")" << std::endl;
		return;
	}

	double sqx = ((double)q.x)*((double)q.x);
	double sqy = ((double)q.y)*(double)q.y;
	double sqz = ((double)q.z)*(double)q.z;
	yaw = atan2f(2 * q.y*q.w - 2 * q.x*q.z, 1 - 2 * sqy - 2 * sqz);
	roll = asin(2 * ((double)q.x*(double)q.y + (double)q.z*(double)q.w));
	pitch = atan2f(2 * q.x*q.w - 2 * q.y*q.z, 1 - 2 * sqx - 2 * sqz);
}
//https://answers.unity.com/questions/1324251/quaternion-to-euler-conversion-singularities.html
static glm::vec3 toEulerAngles2(glm::quat q1)
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

static void toEulerAngles3(const glm::quat& q, float& pitch, float& yaw, float& roll)
{
	//double test = (double)q.x*(double)q.w + (double)q.y*(double)q.z;
	//double test = ((double)q.w*(double)q.x) + ((double)q.y*(double)q.z);
	//double test = ((double)q.x*(double)q.y) + ((double)q.z*(double)q.w);
	double test = ((double)q.w*(double)q.y) + ((double)q.z*(double)q.x);
	if (test > 0.4999) { // singularity at north pole
		roll = 2 * atan2f(q.x, q.w);
		yaw = glm::pi<float>() / 2;
		pitch = 0;
		std::cout << "Singularity North: " << test << ", " << q.w << std::endl;
		std::cout << "\tResolved Rot:  (" << pitch << ", " << yaw << ", " << roll << ")" << std::endl;
		return;
	}
	if (test < -0.4999) { // singularity at south pole
		roll = -2 * atan2f(q.x, q.w);
		yaw = -glm::pi<float>() / 2;
		pitch = 0;
		std::cout << "Singularity South: " << test << ", " << q.w << std::endl;
		std::cout << "\tResolved Rot:  (" << pitch << ", " << yaw << ", " << roll << ")" << std::endl;
		return;
	}

	double sqx = ((double)q.x)*((double)q.x);
	double sqy = ((double)q.y)*(double)q.y;
	double sqz = ((double)q.z)*(double)q.z;
	pitch = atan2f(2 *(q.w*q.x + q.y*q.z), 1 - (2 * (sqx + sqy)));
	yaw = asin(2 * (q.w*q.y + q.z*q.x));
	roll = atan2f(2 * (q.w*q.z + q.x*q.y), 1 - (2 * (sqy + sqz)));

	if (abs(pitch)< 0.00001) pitch = 0;
	if (abs(yaw)  < 0.00001) yaw = 0;
	if (abs(roll) < 0.00001) roll = 0;
}

// Derived from math using https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
static void toEulerAngles4(const glm::quat& _q, float& pitch, float& yaw, float& roll)
{
	quat q(_q.w, _q.x, _q.y, _q.z);

	if (abs(q.w) < 0.0000001) q.w = 0;
	if (abs(q.x) < 0.0000001) q.x = 0;
	if (abs(q.y) < 0.0000001) q.y = 0;
	if (abs(q.z) < 0.0000001) q.z = 0;

	double test = (double)((q.w*q.y) - (q.z*q.x));
	//double test = (double)q.x*(double)q.w + (double)q.y*(double)q.z;
	if (test > 0.4999) { // singularity at north pole
		roll = 2 * atan2f(q.x, q.w);
		yaw = glm::pi<float>() / 2;
		pitch = 0;
		//std::cout << "Singularity North: " << test << ", " << q.w << std::endl;
		//std::cout << "\tResolved Rot:  (" << pitch << ", " << yaw << ", " << roll << ")" << std::endl;
		//return;
	}
	else if (test < -0.4999) { // singularity at south pole
		roll = -2 * atan2f(q.x, q.w);
		yaw = -glm::pi<float>() / 2;
		pitch = 0;
		//std::cout << "Singularity South: " << test << ", " << q.w << std::endl;
		//std::cout << "\tResolved Rot:  (" << pitch << ", " << yaw << ", " << roll << ")" << std::endl;
		//return;
	}
	else
	{
		double sqx = ((double)q.x)*((double)q.x);
		double sqy = ((double)q.y)*(double)q.y;
		double sqz = ((double)q.z)*(double)q.z;

		float r_y = (2 * (q.w*q.z + q.y*q.x));
		float r_x = (1 - (2 * (sqy + sqz)));
		// ZYX?
		roll = atan2f( r_y, r_x);
		yaw = asin(2 * (q.w*q.y - q.z*q.x));
		pitch = atan2f((2 * (q.w*q.x + q.z*q.y)), (1 - (2 * (sqy + sqx))));
		
	}

	float pi = glm::pi<float>();
	//if (abs(q.y) > abs(q.w))
	if (abs(q.y) > sin(pi / 4))
	{
		std::cout << "SHOULD FLIP: " << std::endl;
		//q *= -1; //q.y *= -1;

		yaw = (pi / 2) + ((pi / 2) - yaw); // only positive case...
		roll = roll + pi;
		pitch = pitch + pi;

	}

	// Prevent 2*pi and -2*pi values.
	// Doesnt actually solve the issue..
	float tau = 2 * glm::pi<float>();
	if (pitch >= tau) pitch -= tau;
	if (pitch <= -tau) pitch += tau;
	if (yaw >= tau) yaw -= tau;
	if (yaw <= -tau) yaw += tau;
	if (roll >= tau) roll -= tau;
	if (roll <= -tau) roll += tau;


	if (abs(pitch)< 0.000001) pitch = 0;
	if (abs(yaw)  < 0.000001) yaw = 0;
	if (abs(roll) < 0.000001) roll = 0;
}

//https://gist.github.com/aeroson/043001ca12fe29ee911e
//http://stackoverflow.com/questions/12088610/conversion-between-euler-quaternion-like-in-unity3d-engine
static void toEulerAngles5(const glm::quat& _q, float& pitch, float& yaw, float& roll)
{
	float sqw = _q.w * _q.w;
	float sqx = _q.x * _q.x;
	float sqy = _q.y * _q.y;
	float sqz = _q.z * _q.z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = _q.x * _q.w - _q.y * _q.z;

	if (test > 0.4995f * unit)
	{ // singularity at north pole
		yaw = 2.0f * atan2(_q.y, _q.x);
		pitch = glm::pi<float>() / 2;
		roll = 0;
	}
	if (test < -0.4995f * unit)
	{ // singularity at south pole
		yaw = -2.0f * atan2(_q.y, _q.x);
		pitch = -glm::pi<float>() / 2;
		roll = 0;
	}
	//glm::quat q(_q.w, _q.z, _q.x, _q.y);
	glm::quat q(_q.w, _q.z, _q.x, _q.y);
	//glm::quat q(-_q.w, _q.x, _q.y, _q.z);
	yaw = (float)atan2(2.0f * q.x * q.w + 2.0f * q.y * q.z, 1 - 2.0f * (q.z * q.z + q.w * q.w));     // Yaw
	pitch = (float)asin(2.0f * (q.x * q.z - q.w * q.y));                             // Pitch
	roll = (float)atan2(2.0f * q.x * q.y + 2.0f * q.z * q.w, 1 - 2.0f * (q.y * q.y + q.z * q.z));      // Roll
}

// TODO: Track the different transformation matrices seperately to save on calculations.
// TODO: Convert Rotation matrix to just a quaternion?
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
	glm::quat rq = quat_cast(rotMat);
	//std::cout << "_getRotQuat:(" << rq.w << ", " << rq.x << ", " << rq.y << ", " << rq.z << ")" << std::endl;
	return rq;
	//return quat_cast(rotMat);
}

// Returns the current local euler rotation in DEGREES.
glm::vec3 Transform::getLocalRotationEuler()
{
	quat qRot = getLocalRotation();
	vec3 rot = glm::eulerAngles(qRot);
	//TODO: Rotate using ZXY or YXZ. (Middle axis should be X)
	//vec3 rot;
	//toEulerAngles5(getLocalRotation(), rot.x, rot.y, rot.z);
	rot.x = glm::degrees(rot.x);
	rot.y = glm::degrees(rot.y);
	rot.z = glm::degrees(rot.z);
	return rot;
	/*
	//vec3 rot;
	//toEulerAngles4(getLocalRotation(), rot.x, rot.y, rot.z);
	
	float pi = glm::pi<float>();
	//if (abs(q.y) > abs(q.w))
	if (abs(qRot.y) > sin(pi / 4)) // Not always correct?
	{
		//std::cout << "SHOULD FLIP: " << std::endl;

		rot.y = (pi / 2) + ((pi / 2) - rot.y); // only positive case...
		rot.z = rot.z + pi;
		rot.x = rot.x + pi;

	}
	float tau = 2 * glm::pi<float>();
	if (rot.x >= tau) rot.x -= tau;
	if (rot.x <= -tau) rot.x += tau;
	if (rot.y >= tau) rot.y -= tau;
	if (rot.y <= -tau) rot.y += tau;
	if (rot.z >= tau) rot.z -= tau;
	if (rot.z <= -tau) rot.z += tau;

	rot[0] = glm::degrees(rot[0]);
	rot[1] = glm::degrees(rot[1]);
	rot[2] = glm::degrees(rot[2]);
	return rot;
	*/
}

void Transform::setLocalRotation(glm::quat rot)
{
	//std::cout << "_setQuat:   (" << rot.w << ", " << rot.x << ", " << rot.y << ", " << rot.z << ")" << std::endl;
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
	//	if (rot[i] < -360)
	//	{
	//		int numOver = ((int)rot[i]) / -360;
	//		rot[i] = -(rot[i] + (360 * numOver));
	//	}
	//}
	rot.x = glm::radians(rot.x); 
	//if (rot.x - glm::pi<float>() < 0.0001) rot.x = glm::pi<float>();
	rot.y = glm::radians(rot.y);
	rot.z = glm::radians(rot.z);
	glm::quat rotQuat = glm::normalize(glm::quat(rot));
	
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
	rotation.x = glm::radians(rotation.x);
	rotation.y = glm::radians(rotation.y);
	rotation.z = glm::radians(rotation.z);
	if (rotation.x != 0)
	{
		//model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		rotateMatrix = glm::rotate(rotateMatrix, rotation.x, glm::vec3(1, 0, 0));
	}
	if (rotation.y != 0)
	{
		//model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		rotateMatrix = glm::rotate(rotateMatrix, rotation.y, glm::vec3(0, 1, 0));
	}
	if (rotation.z != 0)
	{
		//model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		rotateMatrix = glm::rotate(rotateMatrix, rotation.z, glm::vec3(0, 0, 1));
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
	EngineDebug::PrintMatrix(model);
	//for (size_t i = 0; i < 4; i++) // Flip i nad j
	//{
	//	std::cout << ((i == 0) ? "[ " : "| ");
	//	for (size_t j = 0; j < 4; j++)
	//	{
	//		std::cout << model[j][i] << ((j == 3) ? "" : "\t");
	//	}
	//	std::cout << ((i == 3) ? " ]\n" : " |\n");
	//}
	
}

void Transform::UpdateMatrix()
{
	// M = T*R*S ... or S*R*T? ... or S*T*R??
	//http://headerphile.com/uncategorized/opengl-matrix-operations/
	//model = scaleMatrix * translateMatrix * rotateMatrix;
	model =  translateMatrix * rotateMatrix * scaleMatrix;

	// TEMP
	localPosition = getPosition();
	localRotation = getLocalRotation();
	localScale = getLocalScale();

	//std::cout << "scale:   (" << localScale.x << ", " << localScale.y << ", " << localScale.z << ")" << std::endl;
}

void Transform::OnDrawGizmos()
{
	//gameObject->transform->DrawGizmo();
}
// Note: this is NOT "OnDrawGizmos".
void Transform::DrawGizmo()
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


void Transform::TestEulerRotation(float x, float y, float z)
{
	std::cout << "=====TESTING EULER ROTATION:" << std::endl;
	// RESET ROTATION.
	gameObject->transform->setLocalRotation(glm::quat(1, 0, 0, 0));

	// SET ROTATION. (Output steps)
	glm::vec3 newRot = glm::vec3(x, y, z);
	std::cout << "setRotDeg:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	gameObject->transform->setLocalRotationEuler(newRot);

	newRot.x = glm::radians(newRot.x);
	newRot.y = glm::radians(newRot.y);
	newRot.z = glm::radians(newRot.z);
	std::cout << "setRotRad:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	glm::quat rq = glm::quat(newRot);
	//rq = glm::normalize(rq);
	std::cout << "setRotQuat: (" << rq.w << ", " << rq.x << ", " << rq.y << ", " << rq.z  << ")" << std::endl;

	//EngineDebug::PrintMatrix(getRotationMatrix());

	/*toEulerAngle(rq, newRot.x, newRot.y, newRot.z);
	std::cout << "radConvBack:(" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	newRot.x = glm::degrees(newRot.x);
	newRot.y = glm::degrees(newRot.y);
	newRot.z = glm::degrees(newRot.z);
	std::cout << "degConvBack:(" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;*/
	
	std::cout << std::endl;

	// PRINT RESULTS
	newRot = gameObject->transform->getLocalRotationEuler();
	std::cout << "newRotDeg:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	newRot.x = glm::radians(newRot.x);
	newRot.y = glm::radians(newRot.y);
	newRot.z = glm::radians(newRot.z);
	std::cout << "newRotRad:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	
	glm::quat newRotQuat = gameObject->transform->getLocalRotation();
	std::cout << "newRotQuat: (" << newRotQuat.w << ", " << newRotQuat.x << ", " << newRotQuat.y << ", " << newRotQuat.z << ")" << std::endl;

	//// ROTATE VERSION
	//std::cout << "Testing Using Rotate:" << std::endl;
	//gameObject->transform->setLocalRotation(glm::quat(1, 0, 0, 0));
	//gameObject->transform->Rotate(glm::vec3(x, y, z));

	//newRot = gameObject->transform->getLocalRotationEuler();
	//std::cout << "newRotDeg:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	//newRot.x = glm::radians(newRot.x);
	//newRot.y = glm::radians(newRot.y);
	//newRot.z = glm::radians(newRot.z);
	//std::cout << "newRotRad:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	//newRotQuat = gameObject->transform->getLocalRotation();
	//std::cout << "newRotQuat: (" << newRotQuat.w << ", " << newRotQuat.x << ", " << newRotQuat.y << ", " << newRotQuat.z << ")" << std::endl;


	//EngineDebug::PrintMatrix(glm::mat4_cast(newRotQuat));

	//glm::quat convQuat(newRot);
	//convQuat = glm::normalize(convQuat);
	//std::cout << "convertQuat:(" << convQuat.w << ", " << convQuat.x << ", " << convQuat.y << ", " << convQuat.z << ")" << std::endl;

	//glm::vec3 newRotEuler;
	//toEulerAngles(newRotQuat, newRotEuler.x, newRotEuler.y, newRotEuler.z);// = glm::eulerAngles(newRotQuat);
	//std::cout << "newRotRad: (" << newRotEuler.x << ", " << newRotEuler.y << ", " << newRotEuler.z << ")" << std::endl;

	// PRINT EXPECTED RESULTS

	// CHECK FOR ERROR
	
	std::cout << "=====" << std::endl;
	return;
	/*
	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	std::cout << "Rot:  (" << rot.x << ", " << rot.y << ", " << rot.z << ")" << std::endl;
	glm::quat rotQuat = gameObject->transform->getLocalRotation();
	std::cout << "RotQ: (" << rotQuat.x << ", " << rotQuat.y << ", " << rotQuat.z << ", " << rotQuat.w << ")" << std::endl;
	std::cout << std::endl;


	std::cout << "setRotDeg:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	gameObject->transform->setLocalRotationEuler(newRot);

	//glm::vec3 newRot = glm::vec3(rot.x + 15, rot.y + 91, rot.z + 15);
	newRot.x = glm::radians(newRot.x);
	newRot.y = glm::radians(newRot.y);
	newRot.z = glm::radians(newRot.z);
	std::cout << "setRotRad:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;


	//glm::quat qPitch = glm::angleAxis(newRot.x, glm::vec3(1, 0, 0));
	//glm::quat qYaw = glm::angleAxis(newRot.y, glm::vec3(0, 1, 0));
	//glm::quat qRoll = glm::angleAxis(newRot.z, glm::vec3(0, 0, 1));
	//glm::quat rq = qRoll * qYaw* qPitch;//qYaw * qRoll * qPitch;//qRoll * qPitch * qYaw; qRoll * qYaw * qPitch;
	//									 //qYaw * qPitch * qRoll;
	//									//qPitch * qRoll * qYaw; //qPitch * qYaw * qRoll;
	glm::quat rq = glm::quat(newRot);
	//rq = glm::quat(glm::vec3(newRot.z, newRot.y, newRot.x));

	//std::cout << "setRotQuat: (" << rq.x << ", " << rq.y << ", " << rq.z << ", " << rq.w << ")" << std::endl;
	//glm::quat rq = glm::quat(newRot);
	//glm::quat rq;
	rq = glm::normalize(rq);
	std::cout << "setRotQuat: (" << rq.x << ", " << rq.y << ", " << rq.z << ", " << rq.w << ")" << std::endl;
	toEulerAngle(rq, newRot.x, newRot.y, newRot.z);
	std::cout << "radConvBack:(" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;

	std::cout << std::endl;
	std::cout << "CONVERTING BACK" << std::endl;
	auto mat = glm::mat4_cast(rq);
	rq = quat_cast(mat);
	//rq = -rq;
	std::cout << "setRotQuat2:(" << rq.x << ", " << rq.y << ", " << rq.z << ", " << rq.w << ")" << std::endl;
	//newRot = glm::eulerAngles(rq); // Same result... right now
	toEulerAngle(rq, newRot.x, newRot.y, newRot.z);
	std::cout << "setRotRad2: (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	newRot[0] = glm::degrees(newRot[0]);
	newRot[1] = glm::degrees(newRot[1]);
	newRot[2] = glm::degrees(newRot[2]);
	std::cout << "setRotDeg2: (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	std::cout << std::endl;


	newRot = gameObject->transform->getLocalRotationEuler();
	std::cout << "newRotDeg: (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	glm::quat newRotQuat = gameObject->transform->getLocalRotation();
	std::cout << "newRotQuat:(" << newRotQuat.x << ", " << newRotQuat.y << ", " << newRotQuat.z << ", " << newRotQuat.w << ")" << std::endl;
	glm::vec3 newRotEuler;
	toEulerAngle(newRotQuat, newRotEuler.x, newRotEuler.y, newRotEuler.z);// = glm::eulerAngles(newRotQuat);
	std::cout << "newRotRad: (" << newRotEuler.x << ", " << newRotEuler.y << ", " << newRotEuler.z << ")" << std::endl;
	std::cout << "newRotDeg2:(" << glm::degrees(newRotEuler.x) << ", " << glm::degrees(newRotEuler.y) << ", " << glm::degrees(newRotEuler.z) << ")" << std::endl;
	std::cout << std::endl;


	glm::quat rotQuatTrans = glm::quat(newRotEuler);
	std::cout << "nRQ_Trans:(" << rotQuatTrans.x << ", " << rotQuatTrans.y << ", " << rotQuatTrans.z << ", " << rotQuatTrans.w << ")" << std::endl;
	float det = glm::determinant(glm::mat4_cast(rotQuat));
	std::cout << "Det:       " << det << std::endl;
	float detT = glm::determinant(glm::mat4_cast(rotQuatTrans));
	std::cout << "Det_Trans: " << detT << std::endl;

	glm::vec3 eRot;
	toEulerAngle(newRotQuat, eRot.x, eRot.y, eRot.z);

	std::cout << "newRotRadx: (" << eRot.x << ", " << eRot.y << ", " << eRot.z << ")" << std::endl;
	std::cout << "newRotDegx:(" << glm::degrees(eRot.x) << ", " << glm::degrees(eRot.y) << ", " << glm::degrees(eRot.z) << ")" << std::endl;
	std::cout << std::endl;

	glm::quat conjRotQuat = glm::conjugate(newRotQuat);
	std::cout << "conjRotQuat:(" << conjRotQuat.x << ", " << conjRotQuat.y << ", " << conjRotQuat.z << ", " << conjRotQuat.w << ")" << std::endl;
	glm::vec3 conjRotEuler = glm::eulerAngles(conjRotQuat);
	std::cout << "conjRotRad: (" << conjRotEuler.x << ", " << conjRotEuler.y << ", " << conjRotEuler.z << ")" << std::endl;
	std::cout << "conjRotDeg:(" << glm::degrees(conjRotEuler.x) << ", " << glm::degrees(conjRotEuler.y) << ", " << glm::degrees(conjRotEuler.z) << ")" << std::endl;
	*/
}