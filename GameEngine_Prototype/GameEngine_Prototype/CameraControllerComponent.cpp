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
	cameraComponent->gameObject->transform->setLocalRotationEuler(localRot);
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
	if (Input::GetKey(GLFW_KEY_W))
	{
		gameObject->transform->Translate(moveFowardSpeed * isInverted(inversionMoveZ) 
			* GameTime::deltaTime * FORWARD);
	}
	if (Input::GetKey(GLFW_KEY_S))
	{
		gameObject->transform->Translate(moveBackwardSpeed * isInverted(inversionMoveZ)
			* GameTime::deltaTime * -FORWARD);
	}
	if (Input::GetKey(GLFW_KEY_A))
	{
		gameObject->transform->Translate(moveLeftSpeed * isInverted(inversionMoveX)
			* GameTime::deltaTime * glm::cross(UP, FORWARD));
	}
	if (Input::GetKey(GLFW_KEY_D))
	{
		gameObject->transform->Translate(moveRightSpeed * isInverted(inversionMoveX)
			* GameTime::deltaTime * glm::cross(UP, -FORWARD));
	}
	if (Input::GetKey(GLFW_KEY_SPACE))
	{
		gameObject->transform->Translate(moveRightSpeed * isInverted(inversionMoveX)
			* GameTime::deltaTime * UP);
	}
	if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
	{
		gameObject->transform->Translate(moveRightSpeed * isInverted(inversionMoveX)
			* GameTime::deltaTime * -UP);
	}
}

int CameraControllerComponent::isInverted(bool _isAxisInverted)
{
	return _isAxisInverted ? -1 : 1;
}

//void initializeView()
//{
//	if (firstMouse) // this bool variable is initially set to true
//	{
//		Controls::lastX = xpos;
//		Controls::lastY = ypos;
//		Controls::firstMouse = false;
//		Controls::yaw = -90.0f;
//	}
//}

//void CameraControllerComponent::moveCamera()
//{
//	glm::vec3 localPos = gameObject->transform->getLocalPosition();
//	Transform* t = gameObject->transform;
//
//	if (Input::GetKey(GLFW_KEY_W) == GLFW_PRESS)
//	{
//		//CameraControl::cam.position += cameraSpeed * glm::normalize(glm::cross(CameraControl::cam.upDirection, glm::cross(CameraControl::cam.viewDirection, CameraControl::cam.upDirection)));
//		//glm::vec3 localPos = gameObject->transform->getLocalPosition();
//		localPos += moveSpeed * XEngine::GameTime::deltaTime * glm::normalize(glm::cross(t->getUpDirection(),
//			glm::cross(t->getForwardDirection(), t->getUpDirection()))); // fly mode
//	}
//
//	if (Input::GetKey(GLFW_KEY_S) == GLFW_PRESS)
//	{
//		//localPos.z = localPos.z * XEngine::GameTime::deltaTime;
//		localPos -= moveSpeed * XEngine::GameTime::deltaTime * glm::normalize(glm::cross(t->getUpDirection(),
//			glm::cross(t->getForwardDirection(), t->getUpDirection())));
//	}
//
//	if (Input::GetKey(GLFW_KEY_A) == GLFW_PRESS)
//	{
//
//	}
//
//	if (Input::GetKey(GLFW_KEY_D) == GLFW_PRESS)
//	{
//
//	}
//
//	if (Input::GetKey(GLFW_KEY_Q) == GLFW_PRESS)
//	{
//
//	}
//
//	if (Input::GetKey(GLFW_KEY_E) == GLFW_PRESS)
//	{
//
//	}
//
//	t->setLocalPosition(localPos);
//
//}

void CameraControllerComponent::DrawInspector()
{
	
	//ImGui::InputFloat(LABEL("fieldOfView "), &fieldOfView);
	ImGui::SliderFloat(LABEL("fieldOfView "), &fieldOfView, 10.0f, 180.0f);
	ImGui::SliderFloat(LABEL("zoomSpeed "), &zoomSpeed, 0.001f, 179.999f);
	ImGui::SameLine();
	ImGui::Checkbox(LABEL("Invert Zoom "), &inversionZoom);
	ImGui::SliderFloat(LABEL("X RotSpeed "), &xRotSpeed, 0.1f, 20.0f, "%.1f"); //TODO: allow min-max values to be modified
	ImGui::SameLine();
	ImGui::Checkbox(LABEL("Invert X Axis "), &inversionX);
	ImGui::SliderFloat(LABEL("Y RotSpeed "), &yRotSpeed, 0.1f, 20.0f, "%.1f");
	ImGui::SameLine();
	ImGui::Checkbox(LABEL("Invert Y Axis "), &inversionY);
	ImGui::SliderFloat(LABEL("Z RotSpeed "), &zRotSpeed, 0.1f, 20.0f, "%.1f");
	ImGui::SameLine();
	ImGui::Checkbox(LABEL("Invert Z Axis "), &inversionZ);

}

//void CameraControllerComponent::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
//{
//	if (fieldOfView >= 1.0f && fieldOfView <= 45.0f) // TODO: remove hardcodes
//		fieldOfView -= yoffset;
//	if (fieldOfView <= 1.0f)
//		fieldOfView = 1.0f;
//	if (fieldOfView >= 45.0f)
//		fieldOfView = 45.0f;
//}

CameraComponent * CameraControllerComponent::findObjectCameraComponent()
{
	cameraComponent = nullptr;

	for (auto component : this->gameObject->components)
	{
		// Finds the first component of the type CameraComponent
		if (component->gameObject->FindComponent(typeid(CameraComponent), (void**)&cameraComponent)) // Pointer to a pointer!
		{
			std::cout << "Found Camera Component!\n";
			break;
		}
		else
		{
			std::cout << "Searching for game object's Camera Component...\n";
		}
	}

	if (!cameraComponent) std::cout << "Camera Component not found...\n";

	return cameraComponent;;
}
