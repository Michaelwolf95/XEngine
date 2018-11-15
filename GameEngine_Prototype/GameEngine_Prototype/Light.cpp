#include "Light.h"
#include "RenderManager.h"

Light::Light()
{
	RenderManager::getInstance().AddLight(this);
}

Light::~Light()
{
	RenderManager::getInstance().RemoveLight(this);
}
