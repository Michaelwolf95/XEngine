#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
//#include <glm/gtx/transform.hpp>
using namespace glm;

Transform::Transform() : Component::Component()
{
	model = glm::mat4(1.0f);
}

Transform::~Transform() {}

void Transform::SetParent(Transform * _parent)
{
	// ToDo: Make sure the object isn't a child.
	parent = _parent;
}


void Transform::Start()
{

}
void Transform::Update()
{

}

glm::mat4 Transform::getMatrix4x4()
{
	return model;
}

glm::vec3 Transform::getPosition()
{
	return glm::vec3(model[3]);
}

void Transform::setPosition(glm::vec3 pos)
{
	//ToDo: Optimize this
	model[3].x = pos.x;
	model[3].y = pos.y;
	model[3].z = pos.z;
	//model[3].w = pos.w;
	//model = translate(mat4(1.0f), pos);
}
// NONE OF THESE DO ANYTHING YET!!!! =====

glm::vec4 Transform::getRotation()
{
	return glm::vec4(model[4]);
}

void Transform::setRotation(glm::vec4 rot)
{
	//model = glm::rotate(mat4(1.0f), rot);
}

glm::vec3 Transform::getLocalScale()
{
	return glm::vec3(model[0]);
}

void Transform::setLocalScale(glm::vec3 scale)
{
	/*model[2].x = scale.x;
	model[2].y = scale.y;
	model[2].z = scale.z;*/
	//model = glm::scale(scale);
}