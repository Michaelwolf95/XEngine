// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "TestComponent.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		std::cout << "ATTACH" << std::endl;
		XEngine::number = 2;



		break;
    case DLL_THREAD_ATTACH:
		std::cout << "THREAD ATTACH" << std::endl;
		std::cout << "DLL: " << XEngine::number << std::endl;
		break;
    case DLL_THREAD_DETACH:
		std::cout << "THREAD DETACH" << std::endl;
		break;
    case DLL_PROCESS_DETACH:
		std::cout << "DETACH" << std::endl;
        break;
    }
	

	//Registrar<T> T::registrar(ComponentTypeInfo(std::string(K), []() {return (Component_ptr)(new T()); }));
	//Component::registry()[typeid(*selectedGameObject->components[i])].name;

	//Component::registry()[typeid(TestComponent)] = ComponentTypeInfo(std::string("TestComponent"), []() {return (Component_ptr)(new TestComponent()); });

	//std::vector<ComponentTypeInfo> componentTypes;
	//for (std::pair<std::type_index, ComponentTypeInfo> element : Component::registry())
	//{
	//	std::cout << element.first.name() << ": " << element.second.name << std::endl;
	//	componentTypes.push_back(element.second); // TODO: fix bug that causes engine to crash when simple model component is chosen
	//}

    return TRUE;
}

