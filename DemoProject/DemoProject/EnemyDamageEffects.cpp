#include "EnemyDamageEffects.h"
#include "XEngine.h"
#include "GameObject.h" 
#include "Component.h"
using namespace XEngine;
#include "HealthManager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/compatibility.hpp"
#include "imgui_inspector_extensions.h"

#include "AudioEngine.h"
#include "AudioManager.h"
#include "MeshRenderer.h"

// Register to be created and serialized.
REGISTER_COMPONENT(EnemyDamageEffects, "EnemyDamageEffects")
BOOST_CLASS_VERSION(EnemyDamageEffects, 2);

EnemyDamageEffects::EnemyDamageEffects() {}
EnemyDamageEffects::~EnemyDamageEffects() {}

// Start is called on the objects first update.
void EnemyDamageEffects::Start()
{
	HealthManager* hp = nullptr;
	if (this->gameObject->FindComponent< HealthManager>(hp))
	{
		hp->OnDeathEvent.connect(boost::bind(&EnemyDamageEffects::StartOnDeathEffect, this));
		hp->OnTakeDamageEvent.connect(boost::bind(&EnemyDamageEffects::DoOnTakeDamageEffect, this, _1));
	}

	AudioManager::getInstance().sound.LoadSound(takeDamageAudioPath, true, false, false);
	AudioManager::getInstance().sound.LoadSound(deathAudioPath, true, false, false);
}

// Update is called once per frame.
void EnemyDamageEffects::Update()
{
	if (deathEffectRunning)
	{
		deathEffectTimer += GameTime::deltaTime;

		glm::vec3 newScale = glm::lerp(startScale, glm::vec3(0, 0, 0), deathEffectTimer / effectTime);
		this->gameObject->transform->setLocalScale(newScale);

		if (deathEffectTimer >= effectTime)
		{
			deathEffectRunning = false;
			this->gameObject->Delete();
		}
	}
}

// Draw the inspector for your custom component.
void EnemyDamageEffects::DrawInspector()
{
	ImGui::InputFloat("Effect Time", &effectTime);
	GUI::FileReference(takeDamageAudioPath, ".mp3", "TakeDmg Sound");
	GUI::FileReference(deathAudioPath, ".mp3", "Death Sound");
	GUI::GameObjectReference(meshObject, "MeshObj");
}

void EnemyDamageEffects::OnEnable()
{
}

void EnemyDamageEffects::OnDisable()
{
}

void EnemyDamageEffects::StartOnDeathEffect()
{
	if (deathEffectRunning) return;
	std::cout << "Starting Death Effect.\n";
	startScale = this->gameObject->transform->getLocalScale();
	deathEffectRunning = true;

	AudioManager::getInstance().sound.PlaySounds(deathAudioPath, this->gameObject->transform->getPosition(), 5);
}

void EnemyDamageEffects::DoOnTakeDamageEffect(float damage)
{
	std::cout << this->gameObject->name << " Took " << damage << "Damage!\n";

	if (meshObject != nullptr)
	{
		XEngine::MeshRenderer* mr;
		if (meshObject->FindComponent<XEngine::MeshRenderer>(mr))
		{
			//mr->material->vec4Properties[0].setValue(glm::vec4(1, 0, 0, 1));
		}
	
	}

	AudioManager::getInstance().sound.PlaySounds(takeDamageAudioPath, this->gameObject->transform->getPosition(), 5);
}

