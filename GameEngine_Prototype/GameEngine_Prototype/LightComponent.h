#pragma once
#include "Component.h"
#include "Light.h"
#include <glm/glm.hpp>
#include "GizmoSpriteDrawer.h"
class LightComponent : public Component, public Light
{
public:
	static Registrar<LightComponent> registrar;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;
	int TYPE_ID;
	
	glm::vec3 ambient = glm::vec3(0.05f);
	glm::vec3 diffuse = glm::vec3(0.8f);
	glm::vec3 specular = glm::vec3(1.0f);

	//GameObject *gameObject;
	LightComponent(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), float _intensity = 20.0f, int typeID = 0);
	//LightComponent();
	~LightComponent();
	virtual void Start() override;
	virtual void Update() override;
	virtual glm::vec3 getLightColor() override;
	virtual glm::vec3 getLightPos() override;
	virtual int getTypeID() override;
	float getIntensity();
	//GameObject* LightComponent::getGameObject();

	void OnDrawGizmos() override;
	void DrawInspector() override;
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
		//ar & BOOST_SERIALIZATION_NVP(ambient);
		//ar & BOOST_SERIALIZATION_NVP(diffuse);
		//ar & BOOST_SERIALIZATION_NVP(specular);
	}
};

