#pragma once
#include "Component.h"
#include "Camera.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
//https://embeddedartistry.com/blog/2016/10/18/embedded-c-sharedfromthis
class EditorCamera : public Component, public Camera, public std::enable_shared_from_this<Camera>
{
public:
	glm::mat4 projection = glm::mat4(1.0f);
	EditorCamera();
	~EditorCamera();
	glm::mat4 getProjection() override;
	glm::mat4 __stdcall getView() override;
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

