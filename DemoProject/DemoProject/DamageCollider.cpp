#include "DamageCollider.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;
#include "Rigidbody.h"
#include "HealthManager.h"

// Register to be created and serialized.
REGISTER_COMPONENT(DamageCollider, "DamageCollider")

DamageCollider::DamageCollider() {}
DamageCollider::~DamageCollider() {}

// Start is called on the objects first update.
void DamageCollider::Start()
{
	RB_SUBSCRIBE_COLLISION_ENTER(DamageCollider)
}

// Update is called once per frame.
void DamageCollider::Update()
{

}

// Draw the inspector for your custom component.
void DamageCollider::DrawInspector()
{
	ImGui::InputInt("Damage", &damageValue);
	ImGui::Checkbox("Destroy Self On Collision", &destroySelfAfterCollision);
	ImGui::Checkbox("Filter By Name", &filterByTargetName);
	if (filterByTargetName)
	{
		ImGui::Indent();
		for (size_t i = 0; i < targetNames.size(); i++)
		{
			
			ImGui::InputText(("Name[" + std::to_string(i) + "]").c_str(), &targetNames[i]);
		}
		ImGui::Unindent();
		static std::string tempName = "";
		ImGui::InputText("New", &tempName);
		if (ImGui::Button("Add"))
		{
			if (tempName.length() > 0)
			{
				targetNames.push_back(tempName);
				tempName = "";
			}
		}
		ImGui::SameLine();
		if (targetNames.size() > 0)
		{
			if (ImGui::Button("Delete"))
			{

				targetNames.pop_back();
			}
		}
		
	}
}

void DamageCollider::OnCollisionEnter(XEngine::CollisionInfo info)
{
	if (info.otherRigidbody)
	{
		HealthManager* hp = nullptr;
		if (info.otherRigidbody->gameObject->FindComponent<HealthManager>(hp))
		{
			if (hp->gameObject->transform->GetRoot() != this->gameObject->transform->GetRoot())
			{
				if (filterByTargetName)
				{
					bool inFilterList = false;
					for (size_t i = 0; i < targetNames.size(); i++)
					{
						if (hp->gameObject->name == targetNames[i])
						{
							inFilterList = true;
							break;
						}
					}
					if (!inFilterList) return;
				}

				hp->applyDamage(damageValue);

				if (destroySelfAfterCollision)
				{
					this->gameObject->Delete();
				}
			}
		}
	}
}

