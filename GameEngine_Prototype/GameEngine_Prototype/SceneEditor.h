#pragma once
#include "Component.h"

enum ManipToolMode
{
	None =	0,
	Move =	1,
	Rotate= 2,
	Scale =	3
};
class SceneEditor : public Component
{
public:
	SceneEditor();
	~SceneEditor();
	void Start() override;
	void Update() override;
	void OnDrawGizmos() override;
	GameObject* selected = nullptr;
	ManipToolMode manipTool = ManipToolMode::None;
	void SelectManipTool();
	void ManipToolUpdate();
	void MoveTool();
	void RotateTool();
	void ScaleTool();
	void AddComponentMenu();
private:
	friend class boost::serialization::access;
	//friend std::ostream & operator<<(std::ostream &os, const Component &comp);
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
	}
};

