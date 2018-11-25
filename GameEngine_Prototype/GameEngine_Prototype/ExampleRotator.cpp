#include "ExampleRotator.h"
#include "GameObject.h"
#include "Time.h"
//#include <GLFW/glfw3.h>
#include "ApplicationManager.h"
#include "DebugUtility.h"

//http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
static void toEulerAngle(const glm::quat& q, float& pitch, float& yaw, float& roll)
{
	float test = q.x*q.y + q.z*q.w;
	if (test > 0.499) { // singularity at north pole
		yaw = 2 * atan2(q.x, q.w);
		roll = glm::pi<double>() / 2;
		pitch = 0;
		std::cout << "Singularity North." << std::endl;
		return;
	}
	if (test < -0.499) { // singularity at south pole
		yaw = -2 * atan2(q.x, q.w);
		roll = -glm::pi<double>() / 2;
		pitch = 0;
		std::cout << "Singularity South." << std::endl;
		return;
	}

	double sqx = q.x*q.x;
	double sqy = q.y*q.y;
	double sqz = q.z*q.z;
	yaw = atan2(2 * q.y*q.w - 2 * q.x*q.z, 1 - 2 * sqy - 2 * sqz);
	roll = asin(2 * test);
	pitch = atan2(2 * q.x*q.w - 2 * q.y*q.z, 1 - 2 * sqx - 2 * sqz);
}

ExampleRotator::ExampleRotator() {}
ExampleRotator::~ExampleRotator() {}

void ExampleRotator::Start()
{
	//this->rotationSpeed = 9999;

	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();
	std::cout << "Rot:  (" << rot.x << ", " << rot.y << ", " << rot.z << ")" << std::endl;
	glm::quat rotQuat = gameObject->transform->getLocalRotation();
	std::cout << "RotQ: (" << rotQuat.x << ", " << rotQuat.y << ", " << rotQuat.z << ", " << rotQuat.w << ")" << std::endl;
	std::cout << std::endl;

	//glm::vec3 newRot = glm::vec3(rot.x + 45, rot.y+91, rot.z);
	//glm::vec3 newRot = glm::vec3(0, 45, 0);
	glm::vec3 newRot = glm::vec3(0, 91, 0);
	//glm::vec3 newRot = glm::vec3(45, 91, 0);
	//glm::vec3 newRot = glm::vec3(-135, 89, 180);

	std::cout << "setRotDeg:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	gameObject->transform->setLocalRotationEuler(newRot);


	//glm::vec3 newRot = glm::vec3(rot.x + 15, rot.y + 91, rot.z + 15);
	newRot.x = glm::radians(newRot.x);
	newRot.y = glm::radians(newRot.y);
	newRot.z = glm::radians(newRot.z);
	std::cout << "setRotRad:  (" << newRot.x << ", " << newRot.y << ", " << newRot.z << ")" << std::endl;
	

	glm::quat qPitch = glm::angleAxis(newRot.x, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(newRot.y, glm::vec3(0, 1, 0));
	glm::quat qRoll = glm::angleAxis(newRot.z, glm::vec3(0, 0, 1));
	glm::quat rq = qRoll * qYaw* qPitch;//qYaw * qRoll * qPitch;//qRoll * qPitch * qYaw; qRoll * qYaw * qPitch;
										 //qYaw * qPitch * qRoll;
										//qPitch * qRoll * qYaw; //qPitch * qYaw * qRoll;
	rq = glm::quat(newRot);
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
}

void ExampleRotator::Update()
{
	//return;
	float deltaY = rotationSpeed * Time::deltaTime;

	float horizontal = 0;
	//Input::GetButtonHold(KEY_CODE) // return bool
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_LEFT) == GLFW_PRESS)
		horizontal += -1;
	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_RIGHT) == GLFW_PRESS)
		horizontal += 1;

	deltaY *= horizontal;

	glm::vec3 rot = gameObject->transform->getLocalRotationEuler();

	// Currently broken API
	glm::vec3 newRot = glm::vec3(rot.x , rot.y + deltaY, rot.z);
	gameObject->transform->setLocalRotationEuler(newRot);

	//glm::quat q = gameObject->transform->getLocalRotation();
	//q = 

	// Rotate using glm Rotate. This one DOESN'T break.
	//gameObject->transform->Rotate(glm::vec3(0, deltaY, 0));

	//glm::quat qRot = gameObject->transform->getLocalRotation();
	//glm::mat4 rotMat = mat4_cast(qRot);
	//glm::quat qRot2 = quat_cast(rotMat);
	//gameObject->transform->setLocalRotation(qRot2);

	if (glfwGetKey(ApplicationManager::APP_WINDOW, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!isPressed)
		{
			gameObject->transform->printTransformMatrix();
			glm::vec3 rotDeg = gameObject->transform->getLocalRotationEuler();
			std::cout << "Rot:  (" << rotDeg.x << ", " << rotDeg.y << ", " << rotDeg.z << ")" << std::endl;
			glm::quat rotQuat = gameObject->transform->getLocalRotation();
			std::cout << "RotQ: (" << rotQuat.x << ", " << rotQuat.y << ", " << rotQuat.z << ", " << rotQuat.w << ")" << std::endl;
			glm::vec3 scale = gameObject->transform->getLocalScale();
			std::cout << "Scale:(" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
			
			//EngineDebug::PrintMatrix(gameObject->transform->getRotationMatrix());
			isPressed = true;
		}
	}
	else if (isPressed)
	{
		isPressed = false;
	}
}
