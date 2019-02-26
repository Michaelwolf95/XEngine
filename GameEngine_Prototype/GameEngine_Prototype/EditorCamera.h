#pragma once
#include "Component.h"
#include "Camera.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
class EditorCamera : public Component, public Camera
{
public:
	glm::mat4 projection = glm::mat4(1.0f);
	EditorCamera();
	~EditorCamera();
	glm::mat4 getProjection() override;
	glm::mat4 getView() override;
	void Start() override;
	void Update() override;

private:
	bool isBeingUsed = false;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
};

