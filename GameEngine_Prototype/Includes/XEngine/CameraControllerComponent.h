#pragma once
#include "CameraComponent.h"
#include <glm/glm.hpp>
#include "Input.h"
#include "GameTime.h"
#include "GameObject.h"

class ENGINE_API CameraControllerComponent : public Component
{
public:
	static Registrar<CameraControllerComponent> registrar;
	CameraControllerComponent(float _fov = 45.0f, float _zoomSpeed = 1.0f, 
		float _xRotSpeed = 5.0f, float _yRotSpeed = 5.0f);
	~CameraControllerComponent();
	void Start();
	void Update();
	inline void rotateCamera();
	inline void zoomCamera();
	inline void moveCamera();
	inline int isInverted(bool _isAxisInverted);

	void DrawInspector();
	//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	CameraComponent* findObjectCameraComponent();

private:
	// Camera Rotation
	float fieldOfView = 45.0f;
	//float panSpeed = 0.1f;
	float xRotSpeed = 5.0f;
	float yRotSpeed = 5.0f;
	float zRotSpeed = 2.5f;
	bool inversionX = false;
	bool inversionY = false;
	bool inversionZ = false;
	bool inversionZoom = false;

	// Camera Translation
	float moveFowardSpeed = 1.0f;
	float moveBackwardSpeed = 1.0f;
	float moveLeftSpeed = 1.0f;
	float moveRightSpeed = 1.0f;
	float moveUpSpeed = 1.0f;
	float moveDownSpeed = 1.0f;
	bool inversionMoveX = false;
	bool inversionMoveY = false;
	bool inversionMoveZ = false;

	// Camera features
	float zoomSpeed = 0.0001f;

	CameraComponent* cameraComponent = nullptr;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(fieldOfView);
		ar & BOOST_SERIALIZATION_NVP(zoomSpeed);
		//ar & BOOST_SERIALIZATION_NVP(panSpeed);
		ar & BOOST_SERIALIZATION_NVP(xRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(yRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(zRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(inversionX);
		ar & BOOST_SERIALIZATION_NVP(inversionY);
		ar & BOOST_SERIALIZATION_NVP(inversionZ);
		ar & BOOST_SERIALIZATION_NVP(inversionZoom);
	}
};

