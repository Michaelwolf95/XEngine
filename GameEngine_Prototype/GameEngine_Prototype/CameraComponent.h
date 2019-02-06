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

};

