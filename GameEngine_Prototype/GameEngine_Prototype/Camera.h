#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();
	virtual mat4 getProjection() = 0;
	virtual mat4 getView() = 0;
	virtual vec3 getPosition() = 0; // added by dennis
};

