#pragma once
#include "imgui.h"
#include <iostream>
#include <string>
#include <type_traits>
//#include <shared_ptr.hpp>
#include "Component.h"
#include "GameObject.h"
#include "Material.h"
namespace XEngine
{
	namespace GUI
	{
		//IMGUI_API void InputTextField(std::string& str, char* label = "##edit");
		IMGUI_IMPL_API void InputTextField(std::string& str, const char* label = "##edit");
		// Helper to display a little (?) mark which shows a tooltip when hovered.
		IMGUI_IMPL_API void HelpMarker(const char* desc);


		IMGUI_IMPL_API void GameObjectReference(GameObject*& go, std::string label);

		IMGUI_IMPL_API void GameObjectReference(GameObject_ptr& go, std::string label);

		IMGUI_IMPL_API void GameObjectReference(std::weak_ptr<GameObject>& go, std::string label);

		IMGUI_IMPL_API void ComponentReference(const std::type_info & typeInfo, Component*& compRef, std::string label);

		IMGUI_IMPL_API void ComponentReference(const std::type_info & typeInfo, Component_ptr& compRef, std::string label);

		//IMGUI_IMPL_API void ComponentReference(const std::type_info & typeInfo, std::weak_ptr<Component>& compRef, std::string label);

		IMGUI_IMPL_API bool FileReference(std::string& pathRef, std::string extension, const char* label = "##edit");
		IMGUI_IMPL_API bool FileReference(std::string& pathRef, std::vector<std::string> extensions, const char* label = "##edit");


		IMGUI_IMPL_API void MaterialReference(Material*& matRef, std::string & pathRef, std::string label);

		template <typename T>
		void ComponentReference(T*& compRef, std::string label)
		{
			static_assert(std::is_base_of<Component, T>::value, "Type T must derive from Component");
			ComponentReference(typeid(T), ((Component*&)compRef), label);
		}
		template <typename T>
		void ComponentReference(std::shared_ptr<T>& compRef, std::string label)
		{
			static_assert(std::is_base_of<Component, T>::value, "Type T must derive from Component");
			ComponentReference(typeid(T), ((Component_ptr&)compRef), label);
		}
		template <typename T>
		void ComponentReference(std::weak_ptr<T>& compRef, std::string label)
		{
			static_assert(std::is_base_of<Component, T>::value, "Type T must derive from Component");
			std::shared_ptr<T> sp = compRef.lock();
			ComponentReference(typeid(T), ((Component_ptr&)sp), label);
			compRef = sp;
		}
	}
}