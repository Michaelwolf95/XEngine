#include "FPCamera.h"

FPCamera::FPCamera()
{
	lastPos = newPos;
	newPos = ::Input::GetMousePos();
	
	// add sensitivity

	glm::vec3 localRot = gameObject->transform->getLocalRotationEuler();

	// 
}


FPCamera::~FPCamera()
{
}

void FPCamera::Update()
{
	
}
