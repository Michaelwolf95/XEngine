#pragma once
#include "imgui.h"
#include <string>
namespace ImGui
{
	//IMGUI_API void InputTextField(std::string& str, char* label = "##edit");
	IMGUI_IMPL_API inline void InputTextField(std::string& str, char* label = "##edit")
	{
		//ImGui::Text("Edit name:");
		char buf[32];
		strcpy_s(buf, &str[0]);
		ImGui::InputText(label, buf, 32);
		if (buf != str)
		{
			//std::cout << buf << std::endl;
			str.clear();
			str = std::string(buf);
		}
		//ImGui::InputText("##edit", &selectedGameObject->name[0], 32, 0, 
		//ImGui::InputText("##edit", inName, 32, 0,
		//	([](ImGuiInputTextCallbackData* data)->int{
		//	if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
		//	{
		//		std::string bufStr(data->Buf);
		//		//if(bufStr != selectedGameObject->name)
		//		//data->b
		//	}
		//	return 0;
		//}));
	}
}