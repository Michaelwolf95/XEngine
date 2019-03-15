#pragma once
#include "Component.h"
#include "Camera.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "GizmoSpriteDrawer.h"
//using namespace glm;

class CameraComponent : public Component, public Camera
{
public:
	static Registrar<CameraComponent> registrar;
	glm::mat4 projection = glm::mat4(1.0f);
	//glm::vec4 clearColor = glm::vec4(1.0f);
	CameraComponent();
	~CameraComponent();
	glm::mat4 getProjection() override;
	glm::mat4 __stdcall getView() override;
	void Start() override;
	void Update() override;
	void OnDrawGizmos() override;
	void DrawInspector() override;
private:
	// Render state
	std::shared_ptr<GizmoSpriteDrawer> gizmoDrawer;

	// Initializes and configures the quad's buffer and vertex attributes
	//void initGizmoRenderData();

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		// save/load base class information
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(clearColor);
	}
};
// FIRST CLASS IS DEFINING?
//BOOST_CLASS_EXPORT_GUID(CameraComponent, "CameraComponent")


