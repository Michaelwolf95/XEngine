#pragma once
#include "Component.h"
#include "Light.h"
#include "LightComponent.h"
#include <glm/glm.hpp>

#define VAR_NAME(Var) (#Var)

class DLLExport GlobalLightComponent : public LightComponent
{
public:
	static Registrar<GlobalLightComponent> registrar;
	static const LightType TYPE_ID = LightType::GlobalLight;

	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
	GlobalLightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), 
		float _intensity = 1.0f, glm::vec3 _direction = glm::vec3(0.0f, -1.0f, 0.0f));

	void Start() override;
	void Update() override;

	float getConstant() override;
	float getLinear() override;
	float getQuadratic() override;
	glm::vec3 getDirection() override;
	int getTypeID() override;
	void draw(Shader* shader, int &counter) override;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LightComponent);
		ar & BOOST_SERIALIZATION_NVP(direction);
	}
};
