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
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "RenderManager.h"
#include "DebugUtility.h"

#include "SceneManager.h"

//using namespace glm;

BOOST_CLASS_EXPORT_GUID(Transform, "Transform")

// TODO: Track the different transformation matrices seperately to save on calculations.
// TODO: Convert Rotation matrix to just a quaternion?
// That is - don't track the whole model. Translation, scale, and rotation seperately.

Transform::Transform() : Component::Component() {}
Transform::~Transform() {}

void Transform::Start() {}
void Transform::Update() {}

#pragma region HIERARCHY

Transform * Transform::GetParent() { return parent; }
std::vector<Transform*> Transform::GetChildren() { return children; }
unsigned int Transform::GetChildCount() { return children.size(); }

void Transform::SetParent(Transform * _parent)
{
	if(parent == _parent) return;
	// Make sure the object isn't already a child.
	if (this->parent != nullptr)
	{
		auto n = std::find(this->parent->children.begin(), this->parent->children.end(), this);
		if (n != this->parent->children.end())
		{
			std::swap(*n, this->parent->children.back());
			this->parent->children.pop_back();
		}
	}

	glm::mat4 globalChild = getMatrix4x4();
	glm::mat4 newModel(1.0);

	parent = _parent;
	if (_parent != nullptr)
	{
		parent->children.push_back(this);

		// Global Child = Global Parent * Local Child
		// GP^-1 *GC = GP^-1 * GP * LC
		// GP^-1 * GC = LC
		// Local Child = Inverse Global Parent * Global Child
		newModel = glm::inverse(parent->getMatrix4x4()) * globalChild;
	}
	else
	{
		// No Parent.
		// Set as root object -> (Handled in scene OnHierarchyUpdate)

		// With no parent, global space is local space.
		newModel = globalChild;
	}

	// Recalculate localSpace vars on re-parent.
	setLocalMatrix4x4(newModel);

	// Update gameobject to handle hierarchy activations.
	this->gameObject->HandleHierarchyChanged();

	// TEMP WORK AROUND.
	// TODO: Resolve dependency on SceneManager
	Scene_ptr scene = SceneManager::getInstance().GetActiveScene();
	if (scene != nullptr)
	{
		scene->OnHierarchyUpdate();
	}
}

#pragma endregion

#pragma region TRANSFORM_ACCESS

// MATRIX ACCESS =======================================================================================================
//
void Transform::UpdateMatrix()
{
	// M = T*R*S
	//http://headerphile.com/uncategorized/opengl-matrix-operations/
	model = translateMatrix * rotateMatrix * scaleMatrix;

	// Calculate the simple values from their matrices.
	localPosition = _calcLocalPositionFromMatrix();
	localRotation = _calcLocalRotationFromMatrix();
	localScale = _calcLocalScaleFromMatrix();
}
void Transform::RecalculateMatrices(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
{
	translateMatrix[3].x = pos.x;
	translateMatrix[3].y = pos.y;
	translateMatrix[3].z = pos.z;
	rotateMatrix = glm::mat4_cast(rot);
	scaleMatrix[0][0] = scale.x;
	scaleMatrix[1][1] = scale.y;
	scaleMatrix[2][2] = scale.z;
	UpdateMatrix();
}
glm::vec3 Transform::_calcLocalPositionFromMatrix()
{
	return glm::vec3(model[3]);
}
glm::quat Transform::_calcLocalRotationFromMatrix()
{
	glm::mat4 rotMat = getRotationMatrix();
	glm::quat rq = quat_cast(rotMat);
	return rq;
}
glm::vec3 Transform::_calcLocalScaleFromMatrix()
{
	return glm::vec3(scaleMatrix[0][0], scaleMatrix[1][1], scaleMatrix[2][2]);
}

glm::mat4& Transform::getModelRef()
{
	return model;
}

glm::mat4 Transform::getMatrix4x4()
{
	//Global Child = Global Parent * Local Child
	if (this->parent != nullptr)
		return this->parent->getMatrix4x4() * model;
	else
		return model;
}

void Transform::setLocalMatrix4x4(glm::mat4 newModel)
{
	glm::vec3 scale;
	glm::quat rot;
	glm::vec3 pos;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(newModel, scale, rot, pos, skew, perspective);
	rot = glm::conjugate(rot);
	RecalculateMatrices(pos, rot, scale);
}

glm::mat4 Transform::getTranslationMatrix() { return translateMatrix; }
glm::mat4 Transform::getRotationMatrix(){ return rotateMatrix; }
glm::mat4 Transform::getScaleMatrix() { return scaleMatrix; }

// POSITION ACCESS =====================================================================================================
//
glm::vec3 Transform::getPosition()
{
	if (this->parent != nullptr)
	{
		glm::vec3 parentPos = this->parent->getPosition();
		glm::vec3 parentScale = this->parent->getScale();
		return glm::vec3(
			parentPos.x + (this->localPosition.x * parentScale.x),
			parentPos.y + (this->localPosition.y* parentScale.y),
			parentPos.z + (this->localPosition.z* parentScale.z));
	}
	else
		return getLocalPosition();
}

glm::vec3 Transform::getLocalPosition(){ return localPosition; }

void Transform::setLocalPosition(float x, float y, float z) { setLocalPosition(glm::vec3(x, y, z)); }
void Transform::setLocalPosition(glm::vec3 pos)
{
	translateMatrix[3].x = pos.x;
	translateMatrix[3].y = pos.y;
	translateMatrix[3].z = pos.z;
	UpdateMatrix();
}

void Transform::setPosition(float x, float y, float z) { setPosition(glm::vec3(x, y, z)); }
// Set World space position.
void Transform::setPosition(glm::vec3 pos)
{
	if (parent != nullptr)
	{
		glm::mat4 globalChild = getMatrix4x4();
		globalChild[3].x = pos.x;
		globalChild[3].y = pos.y;
		globalChild[3].z = pos.z;
		glm::mat4 newModel = glm::inverse(parent->getMatrix4x4()) * globalChild;
		setLocalMatrix4x4(newModel);
	}
	else
	{
		setLocalPosition(pos);
	}
}

// ROTATION ACCESS =====================================================================================================
//

glm::quat Transform::getLocalRotation() { return localRotation; }

glm::quat Transform::getRotation()
{
	if (this->parent != nullptr)
	{
		glm::quat parentRot = this->parent->getRotation();
		glm::quat localRot = getLocalRotation();
		return localRot * parentRot;
	}
	else
		return getLocalRotation();
}

void Transform::setLocalRotation(glm::quat rot)
{
	rotateMatrix = glm::mat4_cast(rot);
	UpdateMatrix();
	return;
}

// Returns the current local euler rotation in DEGREES.
glm::vec3 Transform::getLocalRotationEuler()
{
	glm::vec3 rot;
	//glm::extractEulerAngleZXY(rotateMatrix, rot.x, rot.y, rot.z);
	glm::extractEulerAngleYXZ(rotateMatrix, rot.y, rot.x, rot.z);

	rot.x = glm::degrees((abs(rot.x) < 0.00001) ? 0 : rot.x);
	rot.y = glm::degrees((abs(rot.y) < 0.00001) ? 0 : rot.y);
	rot.z = glm::degrees((abs(rot.z) < 0.00001) ? 0 : rot.z);
	return rot;
}

// Set rotation in terms of euler DEGREES.
void Transform::setLocalRotationEuler(float x, float y, float z) { setLocalRotationEuler(glm::vec3(x, y, z)); }

// Set rotation in terms of euler DEGREES.
void Transform::setLocalRotationEuler(glm::vec3 rot)
{
	rot.x = glm::radians(rot.x); 
	rot.y = glm::radians(rot.y);
	rot.z = glm::radians(rot.z);

	rotateMatrix = glm::eulerAngleYXZ(rot.y, rot.x, rot.z); // TODO: Why are y and x reversed?
	UpdateMatrix();
	return;
}

void Transform::setRotation(glm::quat rot)
{
	//TODO: CONVERT TO WORLD SPACE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	setLocalRotation(rot);
}

// SCALE ACCESS ========================================================================================================
//
glm::vec3 Transform::getLocalScale() { return localScale; }

// Gets scale in world space.
glm::vec3 Transform::getScale()
{
	if (this->parent != nullptr)
	{
		glm::vec3 parentScale = this->parent->getScale();
		glm::vec3 localScale = getLocalScale();
		return glm::vec3(parentScale.x * localScale.x, parentScale.y * localScale.y, parentScale.z * localScale.z);
	}
	else
		return getLocalScale();
}

void Transform::setLocalScale(float x, float y, float z) { setLocalScale(glm::vec3(x, y, z)); }

void Transform::setLocalScale(glm::vec3 scale)
{
	scaleMatrix[0][0] = scale.x;
	scaleMatrix[1][1] = scale.y;
	scaleMatrix[2][2] = scale.z;
	UpdateMatrix();
}

#pragma endregion

#pragma region GLM_MANIPULATIONS

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
	
	rotateMatrix *= glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
	//rotateMatrix *= glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
	UpdateMatrix();
}

void Transform::Scale(glm::vec3 scale)
{
	//model = glm::scale(model, scale);
	scaleMatrix = glm::scale(scaleMatrix, scale);
	UpdateMatrix();
}

// NOT CURRENTLY WORKING
void Transform::LookAt(glm::vec3 lookPos, glm::vec3 up)
{
	glm::vec3 pos = getPosition();
	//model = glm::lookAt(pos, lookPos, up);
	glm::quat result = glm::quatLookAt(glm::normalize(lookPos - pos), up);

	//std::cout << "lookQuat:   (" << result.w << ", " << result.x << ", " << result.y << ", " << result.z << ")" << std::endl;
	//rotateMatrix = glm::lookAt(pos, lookPos, up);
	setLocalRotation(result);
	//glm::lookAtLH
	UpdateMatrix();
}

#pragma endregion

#pragma region DIRECTIONS

// https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
// right   = glm::vec3(matrix[0][0], matrix[1][0], matrix[2][0]);
// up      = glm::vec3(matrix[0][1], matrix[1][1], matrix[2][1]);
// forward = glm::vec3(matrix[0][2], matrix[1][2], matrix[2][2]);

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

#pragma endregion

#pragma region EDITOR_UTILITY

// Note: this is NOT "OnDrawGizmos".
void Transform::DrawGizmo()
{
	glm::vec3 pos = getPosition();
	glm::mat4 rotMat = getRotationMatrix();
	//RenderManager::DrawWorldSpacePoint(pos, vec4(1, 1, 1, 1), 5);
	glm::vec3 right = glm::normalize(getRightDirection());
	glm::vec3 up = glm::normalize(getUpDirection());
	glm::vec3 forward = glm::normalize(getForwardDirection());
	float L = 0.5;
	float sL = 0.1;
	RenderManager::DrawWorldSpaceLine(pos, pos + right * L, glm::vec4(1, 0, 0, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + right * L, glm::vec4(1, 0, 0, 1), 5); // Head

	RenderManager::DrawWorldSpaceLine(pos, pos + up * L, glm::vec4(0, 1, 0, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + up * L, glm::vec4(0, 1, 0, 1), 5);

	RenderManager::DrawWorldSpaceLine(pos, pos + forward * L, glm::vec4(0, 0, 1, 1), 3);
	RenderManager::DrawWorldSpacePoint(pos + forward * L, glm::vec4(0, 0, 1, 1), 5);

	// Gray Square
	RenderManager::DrawWorldSpaceLine(pos + right * sL, pos + (right + forward)*sL, glm::vec4(0.5, 0.5, 0.5, 1), 3);
	RenderManager::DrawWorldSpaceLine(pos + forward * sL, pos + (right + forward)*sL, glm::vec4(0.5, 0.5, 0.5, 1), 3);

}

// Draws the inspector for the editor.
void Transform::DrawInspector()
{
	ImGui::PushID(this);
	glm::vec3 pos = this->getLocalPosition();
	ImGui::InputFloat3("Position", (float*)&pos);
	if (pos != this->getLocalPosition())
		this->setLocalPosition(pos);
	
	glm::vec3 rot = this->getLocalRotationEuler();
	ImGui::InputFloat3("Rotation", (float*)&rot);
	if (rot != this->getLocalRotationEuler())
		this->setLocalRotationEuler(rot);

	float minScale = 0.001f;
	float maxScale = 10.0f;
	glm::vec3 scale = this->getLocalScale();

	isScaleSlider ? ImGui::SliderFloat3("Scale", (float*)&scale, minScale, maxScale)
		: ImGui::InputFloat3("Scale", (float*)&scale);

	if (!scale.x) scale.x = 0.001f;
	if (!scale.y) scale.y = 0.001f;
	if (!scale.z) scale.z = 0.001f;
	ImGui::Checkbox("Lock Ratio", &scaleRatioLock);
	ImGui::SameLine();
	ImGui::Checkbox("Use Scale Slider", &isScaleSlider);
	float ratio;
	if (scaleRatioLock)
	{
		if ((scaleRatio.x - scale.x) != 0)
		{
			ratio = scale.x / scaleRatio.x;
			scale.y *= ratio;
			scale.z *= ratio;
		}
		else if ((scaleRatio.y - scale.y) != 0)
		{
			ratio = scale.y / scaleRatio.y;
			scale.x *= ratio;
			scale.z *= ratio;
		}
		else if ((scaleRatio.z - scale.z) != 0)
		{
			ratio = scale.z / scaleRatio.z;
			scale.x *= ratio;
			scale.y *= ratio;
		}
		else ratio = 1.0f;
	}
	if (scale != this->getLocalScale())
	{
		this->setLocalScale(scale);
	}
	scaleRatio = scale;
	ImGui::PopID();
}

// Prints the Transform Matrix to the console.
void Transform::printTransformMatrix()
{
	std::cout << gameObject->name << " Transform:\n";
	EngineDebug::PrintMatrix(model);
}

#pragma endregion


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