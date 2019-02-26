#include "SceneEditor.h"
#include "Input.h"
#include "SceneManager.h"
#include "Shader.h"
#include "SimpleModelComponent.h"
#include "PrimitiveModels.h"
#include "TestMoverComponent.h"

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
	if (Input::GetKey(GLFW_KEY_LEFT_CONTROL) || Input::GetKey(GLFW_KEY_RIGHT_CONTROL))
	{
		if (Input::GetKeyDown(GLFW_KEY_S))
		{
			// Might need to add some sort of "wait until it finished saving" functionality.
			SceneManager::getInstance().SaveActiveScene();
		}
		else if (Input::GetKeyDown(GLFW_KEY_G))
		{
			std::cout << "Creating new Cube" << std::endl;

			Scene* scene =  SceneManager::getInstance().GetActiveScene();
			GameObject* go = scene->CreateGameObject("New Cube");
			// Create Box Material
			Shader* modelShader = new Shader("model.vs", "model.fs");
			Material* modelMaterial = new Material(modelShader);
			modelMaterial->LoadTexture("textures/container.jpg");
			SimpleModelComponent* testModel = new SimpleModelComponent(CUBE_VERTS, 36, 5,
				CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
			testModel->Setup();

			go->AddComponent(testModel);
			go->AddComponent(new TestMoverComponent());
		}
		/*else if (Input::GetKeyDown(GLFW_KEY_S))
		{

		}*/
	}
}
