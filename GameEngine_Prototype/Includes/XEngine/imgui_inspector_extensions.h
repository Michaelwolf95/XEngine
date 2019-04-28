#pragma once
#include "imgui.h"
#include <iostream>
#include <string>
#include "Component.h"
#include "GameObject.h"

namespace ImGui
{
	//IMGUI_API void InputTextField(std::string& str, char* label = "##edit");
	IMGUI_IMPL_API void InputTextField(std::string& str, const char* label = "##edit");
	// Helper to display a little (?) mark which shows a tooltip when hovered.
	IMGUI_IMPL_API void HelpMarker(const char* desc);


	IMGUI_IMPL_API void GameObjectReference(GameObject*& go, std::string label);
}