#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
//using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();
	virtual glm::mat4 getProjection() = 0;
	virtual glm::mat4 __stdcall getView() = 0;
};

//typedef std::shared_ptr<Camera> Camera_ptr;
