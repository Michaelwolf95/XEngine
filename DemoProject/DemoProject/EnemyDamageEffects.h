#pragma once
#include "Component.h"
#include "XEngineProject.h"
#include "XEngine.h"
using namespace XEngine;

class PROJECT_API EnemyDamageEffects : public Component
{
public:
	static Registrar<EnemyDamageEffects> registrar;
	EnemyDamageEffects();
	~EnemyDamageEffects();
	void Start() override;
	void Update() override;
	void DrawInspector() override;

	void OnEnable() override;
	void OnDisable() override;

	void StartOnDeathEffect();
	void DoOnTakeDamageEffect(float damage);

	float effectTime = 0.5f;

	std::string takeDamageAudioPath;
	std::string deathAudioPath;

	GameObject* meshObject;

private:
	bool deathEffectRunning = false;
	float deathEffectTimer = 0.0f;
	glm::vec3 startScale;

	//AudioComponent* audioComponent = nullptr;

	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		if (version > 0)
		{
			ar & BOOST_SERIALIZATION_NVP(effectTime);
			ar & BOOST_SERIALIZATION_NVP(takeDamageAudioPath);
			ar & BOOST_SERIALIZATION_NVP(deathAudioPath);
		}
		if (version > 1)
		{
			ar & BOOST_SERIALIZATION_NVP(meshObject);
		}
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		// Add custom parameters here.
		if (version > 0)
		{
			ar & BOOST_SERIALIZATION_NVP(effectTime);
			ar & BOOST_SERIALIZATION_NVP(takeDamageAudioPath);
			ar & BOOST_SERIALIZATION_NVP(deathAudioPath);
		}
		if (version > 1)
		{
			ar & BOOST_SERIALIZATION_NVP(meshObject);
		}
	}

};
