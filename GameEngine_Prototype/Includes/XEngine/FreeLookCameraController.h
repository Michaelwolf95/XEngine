#pragma once
#include "Component.h"
#include <glm/glm.hpp>
class ENGINE_API FreeLookCameraController : public Component
{
public:
	static Registrar<FreeLookCameraController> registrar;
	float zoomSpeed = 15.0f;
	float panSpeed = 0.1f;
	float xRotSpeed = 5.0f;
	float yRotSpeed = 5.0f;
	//float fov = 45.0f;
	FreeLookCameraController();
	~FreeLookCameraController();
	void Start() override;
	void Update() override;
	void DrawInspector() override;
private:
	unsigned int camMode;
	glm::vec2 clickPos;
	glm::vec2 lastDragPos;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(zoomSpeed);
		ar & BOOST_SERIALIZATION_NVP(panSpeed);
		ar & BOOST_SERIALIZATION_NVP(xRotSpeed);
		ar & BOOST_SERIALIZATION_NVP(yRotSpeed);
	}
};

