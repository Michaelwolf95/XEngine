#include "SceneEditor.h"
#include "Input.h"
#include "SceneManager.h"

BOOST_CLASS_EXPORT_GUID(SceneEditor, "SceneEditor")

SceneEditor::SceneEditor()
{
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::Start()
{
	std::cout << "STARTING SCENE EDITOR" << std::endl;
}

void SceneEditor::Update()
{
	if (Input::GetKeyDown(GLFW_KEY_S) && (Input::GetKey(GLFW_KEY_LEFT_CONTROL) || Input::GetKey(GLFW_KEY_RIGHT_CONTROL)))
	{
		// Might need to add some sort of "wait until it finished saving" functionality.
		SceneManager::getInstance().SaveActiveScene();
	}
}
