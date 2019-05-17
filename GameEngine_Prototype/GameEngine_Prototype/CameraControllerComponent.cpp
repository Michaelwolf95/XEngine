#include "CameraControllerComponent.h"
#define DELTA_TIME XEngine::GameTime::getInstance().deltaTime
#define FORWARD gameObject->transform->getForwardDirection()
#define UP gameObject->transform->getUpDirection()

using namespace XEngine;

REGISTER_COMPONENT(CameraControllerComponent, "CameraControllerComponent")

CameraControllerComponent::CameraControllerComponent(float _fov, float _zoomSpeed,
	float _xRotSpeed, float _yRotSpeed)
{
	fieldOfView = _fov;
	zoomSpeed = _zoomSpeed;
	xRotSpeed = _xRotSpeed;
	yRotSpeed = _yRotSpeed;
}

CameraControllerComponent::~CameraControllerComponent() {}

void CameraControllerComponent::Start() {}

void CameraControllerComponent::Update()
{
	if (cameraComponent == nullptr)
	{
		findObjectCameraComponent();
	}
	rotateCamera();
	zoomCamera();
	moveCamera();
}

inline void CameraControllerComponent::rotateCamera()
{
	if (Input::GetKeyUp(GLFW_KEY_P)) Input::ToggleCursor(); // temporary

	float deltaXRot = xRotSpeed * isInverted(inversionX) 
		* XEngine::GameTime::getInstance().deltaTime * Input::GetDeltaPosX();
	float deltaYRot = yRotSpeed * isInverted(inversionY) 
		* XEngine::GameTime::getInstance().deltaTime * Input::GetDeltaPosY();

	// gets current local rotation
	glm::vec3 localRot = cameraComponent->gameObject->transform->getLocalRotationEuler();

	// update current rotation
	localRot = glm::vec3(localRot.x + deltaYRot, localRot.y + deltaXRot, localRot.z);

	// set camera's location rotational position
	gameObject->transform->setLocalRotationEuler(localRot);
}

inline void CameraControllerComponent::zoomCamera()
{
	//std::cout << "zoomCamera\n";
	if (Input::GetScrollOffsetY() != 0.0)
	{
		fieldOfView += zoomSpeed * isInverted(inversionZoom) * Input::GetScrollOffsetY();
		if (fieldOfView > 180.0f) fieldOfView = 180.0f;
		if (fieldOfView < 0.0f) fieldOfView = 0.0f;
		cameraComponent->projection = glm::perspective(glm::radians(fieldOfView), SCREEN_RATIO, 0.1f, 100.0f);
	}
}

inline void CameraControllerComponent::moveCamera()
{
	if (Input::GetKey(GLFW_KEY_LEFT_SHIFT))
	{
		moveSpeedModified = true;
	}
	else if (moveSpeedModified)
	{
		moveSpeedModified = false;
	}
	if (Input::GetKey(GLFW_KEY_W))
	{
		gameObject->transform->Translate(moveFowardSpeed * isInverted(inversionMoveZ) 
			* isMoveSpeedModified(moveSpeedModified, moveSpeedModifierZ) * GameTime::deltaTime * FORWARD);
	}
	if (Input::GetKey(GLFW_KEY_S))
	{
		gameObject->transform->Translate(moveBackwardSpeed * isInverted(inversionMoveZ)
			* isMoveSpeedModified(moveSpeedModified, moveSpeedModifierZ) * GameTime::deltaTime * -FORWARD);
	}
	if (Input::GetKey(GLFW_KEY_A))
	{
		gameObject->transform->Translate(moveLeftSpeed * isInverted(inversionMoveX)
			* isMoveSpeedModified(moveSpeedModified, moveSpeedModifierX) * GameTime::deltaTime * glm::cross(UP, FORWARD));
	}
	if (Input::GetKey(GLFW_KEY_D))
	{
		gameObject->transform->Translate(moveRightSpeed * isInverted(inversionMoveX)
			* isMoveSpeedModified(moveSpeedModified, moveSpeedModifierX) * GameTime::deltaTime * glm::cross(UP, -FORWARD));
	}
	if (Input::GetKey(GLFW_KEY_SPACE))
	{
		gameObject->transform->Translate(moveRightSpeed * isInverted(inversionMoveX)
			* isMoveSpeedModified(moveSpeedModified, moveSpeedModifierY) * GameTime::deltaTime * UP);
	}
	if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
	{
		gameObject->transform->Translate(moveRightSpeed * isInverted(inversionMoveX)
			* isMoveSpeedModified(moveSpeedModified, moveSpeedModifierY) * GameTime::deltaTime * -UP);
	}
}

int CameraControllerComponent::isInverted(bool _isAxisInverted)
{
	return _isAxisInverted ? -1 : 1;
}

float CameraControllerComponent::isMoveSpeedModified(bool _moveSpeedModified, float _moveSpeedModifier) // using parameter to use with any axis
{
	return _moveSpeedModified ? _moveSpeedModifier : 1.0f;
}

void CameraControllerComponent::DrawInspector()
{
	
	//ImGui::InputFloat("fieldOfView "), &fieldOfView);
	ImGui::PushID(this);
	ImGui::SliderFloat("fieldOfView", &fieldOfView, 10.0f, 180.0f);
	ImGui::SliderFloat("zoomSpeed", &zoomSpeed, 0.001f, 179.999f);
	ImGui::SameLine();
	ImGui::Checkbox("Invert Zoom", &inversionZoom);
	ImGui::SliderFloat("X RotSpeed", &xRotSpeed, 0.1f, 20.0f, "%.1f"); //TODO: allow min-max values to be modified
	ImGui::SameLine();
	ImGui::Checkbox("Invert X Axis", &inversionX);
	ImGui::SliderFloat("Y RotSpeed", &yRotSpeed, 0.1f, 20.0f, "%.1f");
	ImGui::SameLine();
	ImGui::Checkbox("Invert Y Axis", &inversionY);
	ImGui::SliderFloat("Z RotSpeed", &zRotSpeed, 0.1f, 20.0f, "%.1f");
	ImGui::SameLine();
	ImGui::Checkbox("Invert Z Axis", &inversionZ);
	ImGui::SliderFloat("Move Speed Mod X", &moveSpeedModifierX, 0.1f, 20.0f, "%.1f");
	ImGui::SliderFloat("Move Speed Mod Y", &moveSpeedModifierY, 0.1f, 20.0f, "%.1f");
	ImGui::SliderFloat("Move Speed Mod Z", &moveSpeedModifierZ, 0.1f, 20.0f, "%.1f");
	ImGui::PopID();
}

CameraComponent * CameraControllerComponent::findObjectCameraComponent()
{
	cameraComponent = nullptr;

	// Finds the first component of the type CameraComponent
	gameObject->FindComponent(typeid(CameraComponent), (void**)&cameraComponent); // Pointer to a pointer!

	cameraComponent ? std::cout << "Camera Component found!\n" : std::cout << "Camera Component not found...\n";
	
	//if (!cameraComponent)
	//{
	//	std::cout << "Adding Camera Component to object!\n";
	//	std::shared_ptr<CameraComponent> camComponent(new CameraComponent());
	//	gameObject->AddComponent(camComponent);
	//	cameraComponent = camComponent.get();
	//} // doesn't work...

	return cameraComponent;;
}
