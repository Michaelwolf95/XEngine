#include "ComponentTemplate.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;

// Register to be created and serialized.
REGISTER_COMPONENT(ComponentTemplate, "ComponentTemplate")

ComponentTemplate::ComponentTemplate() {}
ComponentTemplate::~ComponentTemplate() {}

// Start is called on the objects first update.
void ComponentTemplate::Start()
{

}

// Update is called once per frame.
void ComponentTemplate::Update()
{

}

// Draw the inspector for your custom component.
void ComponentTemplate::DrawInspector()
{

}

