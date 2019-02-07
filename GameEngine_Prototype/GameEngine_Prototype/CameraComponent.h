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
		std::cout << "Serializing Derived Component." << std::endl;
		// save/load base class information
		//ar & boost::serialization::base_object<Component>(*this);
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		std::cout << "\tType: ." << std::endl;
	}
};
// FIRST CLASS IS DEFINING?
//BOOST_CLASS_EXPORT_GUID(CameraComponent, "CameraComponent")


