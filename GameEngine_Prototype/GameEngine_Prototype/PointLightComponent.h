#pragma once
#include "Component.h"
#include "Light.h"
#include "LightComponent.h"
#include <glm/glm.hpp>
class PointLightComponent : public LightComponent
{
public:
	static Registrar<PointLightComponent> registrar;
	static const int TYPE_ID = 1;

	// position is a function of transform component
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	PointLightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), 
		float _intensity = 1.0f);
	~PointLightComponent();

	void Start() override;
	void Update() override;

	float getConstant() override;
	float getLinear() override;
	float getQuadratic() override;
	glm::vec3 getDirection() override;
	int getTypeID() override;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LightComponent);
	}
};