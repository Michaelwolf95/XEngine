#pragma once
#include "Component.h"
#include "Camera.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

class CameraComponent : public Component, public Camera
{
public:
	static Registrar<CameraComponent> registrar;
	mat4 projection = mat4(1.0f);
	CameraComponent();
	~CameraComponent();
	mat4 getProjection() override;
	mat4 getView() override;
	void Start() override;
	void Update() override;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
};
// FIRST CLASS IS DEFINING?
//BOOST_CLASS_EXPORT_GUID(CameraComponent, "CameraComponent")


