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
	std::string UNIFORM_NAME;
	
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;

	//GameObject *gameObject;
	LightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), float _intensity = 20.0f, int typeID = 0);
	~LightComponent();
	virtual void Start() override;
	virtual void Update() override;
	glm::vec3 getLightColor() override;
	glm::vec3 getLightPos() override;
	float getIntensity() override;
	virtual float getConstant() override;
	virtual float getLinear() override;
	virtual float getQuadratic() override;
	virtual glm::vec3 getDirection() override;
	virtual int getTypeID() override = 0;
	virtual void DrawInspector() override;
	virtual void OnDrawGizmos() override;
	virtual void draw(Shader* shader, int &counter) override;
	virtual const std::string* getUniformName();

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