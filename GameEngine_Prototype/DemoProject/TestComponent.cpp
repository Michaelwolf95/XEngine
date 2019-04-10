#include "stdafx.h"
#include "XEngine.h"
#include "TestComponent.h"

BOOST_CLASS_EXPORT_GUID(TestComponent, "TestComponent")

Registrar<TestComponent> TestComponent::registrar((ComponentTypeInfo&)ComponentTypeInfo(std::string("TestComponent"), []() {return (Component_ptr)(new TestComponent()); }));

TestComponent::TestComponent()
{
	//ComponentTypeInfo(std::string("TestComponent"), []() {return (Component_ptr)(new TestComponent()); });
}

TestComponent::~TestComponent()
{
}

void TestComponent::Start()
{
}

void TestComponent::Update()
{
}