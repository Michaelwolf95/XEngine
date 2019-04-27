#include "ExampleComponent.h"
#include "GameObject.h"
#include "Component.h"
REGISTER_COMPONENT(ExampleComponent, "ExampleComponent")

ExampleComponent::ExampleComponent() {}
ExampleComponent::~ExampleComponent() {}

void ExampleComponent::Start()
{

}

void ExampleComponent::Update()
{
}

void ExampleComponent::DrawInspector()
{
	ImGui::Text("Hi Gabe!");
	ImGui::InputText("GameObjPointer", &(*this->GameObjPtr)->name[0], 48);
	
	/*
	
	if (payload != nullptr && payload->IsDataType("FILE_DRAG"))
	{
		ImGui::Text("<----- CHANGE MODEL ----->");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
			{
				IM_ASSERT(payload->DataSize == 128);
				const char* payload_n = (const char*)payload->Data;

				std::string fileName(payload_n);
				if (fileName.substr(fileName.find_last_of(".")) == ".obj")
				{
					std::cout << "Dropping MODEL!" << std::endl;

					// temporary fix, replace any backward slash with a forward slash
					std::replace(fileName.begin(), fileName.end(), '\\', '/');

					gameObject->name = fileName;
				}
			}
			ImGui::EndDragDropTarget();
		}
	}*/
	const ImGuiPayload* payload = ImGui::GetDragDropPayload();
	if (payload != nullptr && payload->IsDataType("GAMEOBJECT_DRAG"))
	{
		ImGui::Text("<----- GameObject Drap ----->");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_DRAG"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject*));
				GameObject* payload_n = (GameObject*)payload->Data;
				std::cout << "Dropping " << payload_n->name << " on empty." << std::endl;
				
				GameObject** GameObjPtr = &payload_n;

			}
			ImGui::EndDragDropTarget();
		}
		ImGui::Unindent();
	}
}