#include "imgui.h"
#include <iostream>
#include <string>
#include "XEngine.h"
#include "Component.h"
#include "GameObject.h"
namespace XEngine
{
	namespace GUI
	{
		//IMGUI_API void InputTextField(std::string& str, char* label = "##edit");
		IMGUI_IMPL_API void InputTextField(std::string& str, const char* label)
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
		// Helper to display a little (?) mark which shows a tooltip when hovered.
		IMGUI_IMPL_API void HelpMarker(const char* desc)
		{
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(desc);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		IMGUI_IMPL_API void GameObjectReference(GameObject*& go, std::string label)
		{
			ImGui::Text(label.c_str());
			ImGui::SameLine();
			if (go != nullptr)
			{
				ImGui::Button(go->name.c_str());
			}
			else
			{
				ImGui::Button("(None)");
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_DRAG"))
				{
					IM_ASSERT(payload->DataSize == sizeof(GameObject_ptr));
					GameObject_ptr payload_n = *(const GameObject_ptr*)payload->Data;

					go = payload_n.get();
				}
				ImGui::EndDragDropTarget();
			}
		}

		IMGUI_IMPL_API void ComponentReference(const std::type_info & typeInfo, Component*& compRef, std::string label)
		{
			ImGui::Text(label.c_str());
			ImGui::SameLine();
			if (compRef != nullptr)
			{
				ImGui::Button((compRef)->gameObject->name.c_str());
			}
			else
			{
				std::string emptyLabel = "(" + Component::registry()[typeInfo].name + ")";
				ImGui::Button(emptyLabel.c_str());
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_DRAG"))
				{
					IM_ASSERT(payload->DataSize == sizeof(GameObject_ptr));
					GameObject_ptr payload_n = *(const GameObject_ptr*)payload->Data;

					//go = payload_n.get();

					payload_n.get()->FindComponent(typeInfo, (void**)&compRef);
				}
				ImGui::EndDragDropTarget();
			}

		}
		IMGUI_IMPL_API void ComponentReference(const std::type_info & typeInfo, Component_ptr & compRef, std::string label)
		{
			ImGui::Text(label.c_str());
			ImGui::SameLine();
			if (compRef.get() != nullptr)
			{
				if (compRef->gameObject == nullptr)
				{
					compRef.reset();
					return;
				}
				ImGui::Button((compRef)->gameObject->name.c_str());
			}
			else
			{
				//std::cout << compRef.get() << std::endl;
				std::string emptyLabel = "(" + Component::registry()[typeInfo].name + ")";
				ImGui::Button(emptyLabel.c_str());
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_DRAG"))
				{
					IM_ASSERT(payload->DataSize == sizeof(GameObject_ptr));
					GameObject_ptr payload_n = *(const GameObject_ptr*)payload->Data;

					//go = payload_n.get();

					payload_n.get()->FindComponent(typeInfo, (Component_ptr*)&compRef);

					/*if (compRef.get() != nullptr)
						std::cout << compRef->gameObject->name << std::endl;*/
				}
				ImGui::EndDragDropTarget();
			}
		}

		IMGUI_IMPL_API bool FileReference(std::string & pathRef, std::string extension, const char * label)
		{
			std::vector<std::string> exts;
			exts.push_back(extension);
			return FileReference(pathRef, exts, label);
		}

		IMGUI_IMPL_API bool FileReference(std::string & pathRef, std::vector<std::string> extensions, const char * label)
		{
			bool assigned = false;
			//GUI::InputTextField(pathRef, label);
			ImGui::InputText(label, &pathRef);
			const ImGuiPayload* payload = ImGui::GetDragDropPayload();
			//if (payload != nullptr && payload->IsDataType("FILE_DRAG"))
			{
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
					{
						IM_ASSERT(payload->DataSize == 128);
						const char* payload_n = (const char*)payload->Data;

						std::string filePath(payload_n);
						std::string ext = (filePath.substr(filePath.find_last_of(".")));
						bool valid = false;
						for (size_t i = 0; i < extensions.size(); i++)
						{
							if (valid = (ext == extensions[i]))
								break;
						}
						if (valid)
						{
							std::replace(filePath.begin(), filePath.end(), '\\', '/');
							pathRef.assign(filePath);
						}
						assigned = true;
					}
					ImGui::EndDragDropTarget();
				}
			}
			return assigned;
		}

		IMGUI_IMPL_API void MaterialReference(Material *& matRef, std::string & pathRef, std::string label)
		{
			bool changed = FileReference(pathRef, ".material", "Material Path");
			if (changed)
			{
				std::replace(pathRef.begin(), pathRef.end(), '\\', '/');
				Material* mat = AssetManager::getInstance().materialLib.GetAsset(pathRef);
				if (mat != nullptr)
				{
					matRef = mat;
				}
			}
			matRef->DrawInspector();
		}
	}
}