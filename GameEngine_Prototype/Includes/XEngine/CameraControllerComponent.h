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
	inline float isMoveSpeedModified(bool _moveSpeedModified, float _moveSpeedModifier);


	void DrawInspector();
	CameraComponent* findObjectCameraComponent();

	CameraControllerComponent * DeepCopy() override;

private:
	// Camera Rotation
	float fieldOfView = 45.0f;
	float xRotSpeed = 5.0f;
	float yRotSpeed = 5.0f;
	float zRotSpeed = 2.5f;
	bool inversionX = false;
	bool inversionY = false;
	bool inversionZ = false;
	bool inversionZoom = false;
	bool moveSpeedModified = false;

	// Camera Translation
	float moveFowardSpeed = 2.0f;
	float moveBackwardSpeed = 2.0f;
	float moveLeftSpeed = 2.0f;
	float moveRightSpeed = 2.0f;
	float moveUpSpeed = 2.0f;
	float moveDownSpeed = 2.0f;
	bool inversionMoveX = false;
	bool inversionMoveY = false;
	bool inversionMoveZ = false;
	float moveSpeedModifierX = 5.0f; // run, walk, etc.
	float moveSpeedModifierY = 5.0f;
	float moveSpeedModifierZ = 5.0f;

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
		ar & BOOST_SERIALIZATION_NVP(xRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(yRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(zRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(inversionX);
		ar & BOOST_SERIALIZATION_NVP(inversionY);
		ar & BOOST_SERIALIZATION_NVP(inversionZ);
		ar & BOOST_SERIALIZATION_NVP(inversionZoom);
		ar & BOOST_SERIALIZATION_NVP(moveFowardSpeed);
		ar & BOOST_SERIALIZATION_NVP(moveBackwardSpeed);
		ar & BOOST_SERIALIZATION_NVP(moveLeftSpeed);
		ar & BOOST_SERIALIZATION_NVP(moveRightSpeed);
		ar & BOOST_SERIALIZATION_NVP(moveUpSpeed);
		ar & BOOST_SERIALIZATION_NVP(moveDownSpeed);
		ar & BOOST_SERIALIZATION_NVP(inversionMoveX);
		ar & BOOST_SERIALIZATION_NVP(inversionMoveY);
		ar & BOOST_SERIALIZATION_NVP(inversionMoveZ);
		ar & BOOST_SERIALIZATION_NVP(moveSpeedModifierX);
		ar & BOOST_SERIALIZATION_NVP(moveSpeedModifierY);
		ar & BOOST_SERIALIZATION_NVP(moveSpeedModifierZ);
	}
};

