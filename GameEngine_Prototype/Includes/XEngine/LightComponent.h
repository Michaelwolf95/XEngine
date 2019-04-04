#pragma once
#include "Component.h"
#include "Light.h"
#include <glm/glm.hpp>
#include "GizmoSpriteDrawer.h"
#define VAR_NAME(Var) (#Var)

class LightComponent : public Component, public Light
{
public:
	static Registrar<LightComponent> registrar;
	
	glm::vec4 color;
	float intensity;

	//GameObject *gameObject;
	LightComponent(glm::vec4 _color = glm::vec4(1.0f), float _intensity = 1.0f);
	~LightComponent();
	virtual void Start() override;
	virtual void Update() override;
	//glm::vec3 getLightColor() override;
	//float getIntensity() override;
	//virtual float getConstant() override;
	//virtual float getLinear() override;
	//virtual float getQuadratic() override;
	glm::vec3 getLightPos() override;
	glm::vec3 getDirection() override;
	int getTypeID() override;
	const std::string* getUniformName() override;
	virtual void DrawInspector() override;
	virtual void OnDrawGizmos() override;
	virtual void Draw(Shader* shader, int &counter) override;

private:
	std::shared_ptr<GizmoSpriteDrawer> gizmoDrawer;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(intensity);
		ar & BOOST_SERIALIZATION_NVP(color);
	}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(LightComponent)
