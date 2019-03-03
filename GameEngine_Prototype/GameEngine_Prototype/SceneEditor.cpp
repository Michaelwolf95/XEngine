#include "SceneEditor.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "Shader.h"
#include "SimpleModelComponent.h"
#include "PrimitiveModels.h"
#include "TestMoverComponent.h"
#include "ApplicationManager.h"

//BOOST_CLASS_EXPORT_GUID(SceneEditor, "SceneEditor")

SceneEditor * SceneEditor::CreateManager()
{
	SceneEditor* instance = &SceneEditor::getInstance();
	instance->Init();
	return instance;
}

int SceneEditor::Init()
{
	GameObject go();
	isInitialized = true;
	return 0;
}

SceneEditor::SceneEditor()
{
	//executeInEditMode = true;
	std::cout << "SCENE EDITOR: Press CTRL+SHIFT+E to Edit scene." << std::endl;
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::StartEditMode()
{
	//std::cout << "SCENE EDITOR: Press CTRL+SHIFT+E to Edit scene." << std::endl;
	if (ApplicationManager::getInstance().IsEditMode() == false)
	{
		ApplicationManager::getInstance().SetEditMode(true);
		SceneManager::getInstance().ReloadSceneFromFile();
		
		std::cout << "ENTERING EDIT MODE =========================" << std::endl;
		std::cout << "\tCTRL+E: Select Object to Edit" << std::endl;
		std::cout << "\tCTRL+S: Save Scene" << std::endl;
		std::cout << "\tCTRL+P: Print Scene" << std::endl;
		std::cout << "\tCTRL+I: Inspect Selected Object" << std::endl;
		std::cout << "\t[Q],[W],[E],[R]: Manipulate Selected Object." << std::endl;

		if (selectedGameObject == nullptr)
		{
			if (SceneManager::getInstance().GetActiveScene()->rootGameObjects.size() >= 2)
			{
				selectedGameObject = SceneManager::getInstance().GetActiveScene()->rootGameObjects[1];
				std::cout << "Auto-Selected GameObject[1]: " << selectedGameObject->name << std::endl;
			}
		}
	}
}

void SceneEditor::ExitEditMode()
{
	if (ApplicationManager::getInstance().IsEditMode())
	{
		std::cout << "EXITING EDIT MODE =========================" << std::endl;
		ApplicationManager::getInstance().SetEditMode(false);
		SceneManager::getInstance().ReloadSceneFromFile();
	}
}

void SceneEditor::UpdateEditor()
{
	// Swap edit mode
	if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT))
		&& (Input::GetKey(GLFW_KEY_LEFT_CONTROL) || Input::GetKey(GLFW_KEY_RIGHT_CONTROL))
		&& Input::GetKeyDown(GLFW_KEY_E))
	{
		if (ApplicationManager::getInstance().IsEditMode())
		{
			ExitEditMode();
		}
		else
		{
			StartEditMode();
		}
		return;
	}

	if (ApplicationManager::getInstance().IsEditMode())
	{
		if (Input::GetKey(GLFW_KEY_LEFT_CONTROL) || Input::GetKey(GLFW_KEY_RIGHT_CONTROL))
		{
			if (Input::GetKeyDown(GLFW_KEY_S))
			{
				// Might need to add some sort of "wait until it finished saving" functionality.
				SceneManager::getInstance().SaveActiveScene();
			}
			else if (Input::GetKeyDown(GLFW_KEY_N)) // New GameObject
			{
				std::cout << "Creating New GameObject" << std::endl;

				Scene_ptr scene =  SceneManager::getInstance().GetActiveScene();
				GameObject_ptr go = scene->CreateGameObject("New GameObject");

				//// Create Box Material
				//Shader* modelShader = new Shader("model.vs", "model.fs");
				//Material* modelMaterial = new Material(modelShader);
				//modelMaterial->LoadTexture("textures/container.jpg");
				//SimpleModelComponent* testModel = new SimpleModelComponent(CUBE_VERTS, 36, 5,
				//	CUBE_INDICES, sizeof(CUBE_INDICES) / sizeof(unsigned int), modelMaterial);
				//testModel->Setup();
				//go->AddComponent(testModel);

				selectedGameObject = go;
				//go->AddComponent(new TestMoverComponent());
			}
			else if (Input::GetKeyDown(GLFW_KEY_E)) // "Edit" object - select an object to edit.
			{
				if (selectedGameObject == nullptr)
				{
					Scene_ptr scene = SceneManager::getInstance().GetActiveScene();
					//scene->PrintScene();

					// Set focus to console.
					SetFocus(GetConsoleWindow());
					BringWindowToTop(GetConsoleWindow());
					//glfwFocusWindow(glfwconsole)

					// Select GameObject by index.
					int selectIndex = -1;
					while (selectIndex < 0 && selectIndex > scene->rootGameObjects.size())
					{
						std::cout << "Select GameObject Index: ";// << std::endl;
						std::cin >> selectIndex;
						//std::cout << std::endl;
						selectedGameObject = scene->rootGameObjects[selectIndex];
					}
					std::cout << "Selected: " << selectedGameObject->name << std::endl;

					// Set focus back to app.
					glfwFocusWindow(ApplicationManager::APP_WINDOW);
				}
				else
				{
					selectedGameObject = nullptr;
					std::cout << "No longer selecting." << std::endl;
				}
			}
			else if(Input::GetKeyDown(GLFW_KEY_P)) // Print Scene
			{
				SceneManager::getInstance().GetActiveScene()->PrintScene();
			}
			else if (Input::GetKeyDown(GLFW_KEY_I)) // Inspect selected
			{
				if (selectedGameObject == nullptr)
				{
					std::cout << "No Object to Inspect." << std::endl;
				}
				else
				{
					Scene_ptr scene = SceneManager::getInstance().GetActiveScene();
					auto index = std::distance(scene->rootGameObjects.begin(), std::find(scene->rootGameObjects.begin(), scene->rootGameObjects.end(), selectedGameObject));
					std::cout << "Inspecting Object: [" << index << "]: " << selectedGameObject->name << std::endl;
					// Output current position.
					glm::vec3 pos = selectedGameObject->transform->getPosition();
					std::cout << "Pos:  (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
					// Output current local scale.
					glm::vec3 scale = selectedGameObject->transform->getLocalScale();
					std::cout << "Scale:(" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
					glm::quat rot = selectedGameObject->transform->getLocalRotation();
					std::cout << "Rot:  (" << rot.x << ", " << rot.y << ", " << rot.z << ", " << rot.w << ")" << std::endl;
				}
			}
			else if (Input::GetKeyDown(GLFW_KEY_D)) // Delete selected
			{
				if (selectedGameObject == nullptr)
				{
					std::cout << "No Object to Delete." << std::endl;
					return;
				}
				SceneManager::getInstance().GetActiveScene()->DeleteGameObject(selectedGameObject);
				selectedGameObject = nullptr;
			}
			else if (Input::GetKeyDown(GLFW_KEY_A))
			{
				if (selectedGameObject == nullptr)
				{
					std::cout << "No Object to Add Component." << std::endl;
					return;
				}
				std::cout << "Add Component:" << std::endl;
				AddComponentMenu();
			}
			else if (Input::GetKeyDown(GLFW_KEY_R))
			{
				if (selectedGameObject == nullptr)
				{
					std::cout << "No Object to Rename" << std::endl;
					return;
				}
				std::cout << "Renaming: " << selectedGameObject->name << std::endl;
				//std::string newName;
				std::cout << "Name: ";
				std::cin >> selectedGameObject->name;
				std::cout << std::endl;
			}


		}
		else // Ctrl not pressed.
		{
			SelectManipTool();
		}

		// Update Manip Tool
		if (selectedGameObject != nullptr)
		{
			ManipToolUpdate();
		}
	}
}

void SceneEditor::DrawEditorGizmos()
{
	if (selectedGameObject != nullptr)
	{
		selectedGameObject->transform->DrawGizmo();
	}
}

void SceneEditor::SelectManipTool()
{
	if (Input::GetKeyDown(GLFW_KEY_Q))
	{
		manipTool = ManipToolMode::None;
		std::cout << "ManipTool: None." << std::endl;
	}
	else if (Input::GetKeyDown(GLFW_KEY_W))
	{
		manipTool = ManipToolMode::Move;
		std::cout << "ManipTool: Move." << std::endl;
	}
	else if (Input::GetKeyDown(GLFW_KEY_E))
	{
		manipTool = ManipToolMode::Rotate;
		std::cout << "ManipTool: Rotate." << std::endl;
	}
	else if (Input::GetKeyDown(GLFW_KEY_R))
	{
		manipTool = ManipToolMode::Scale;
		std::cout << "ManipTool: Scale." << std::endl;
	}
}

void SceneEditor::ManipToolUpdate()
{
	switch (manipTool)
	{
	case ManipToolMode::None:

		break;
	case ManipToolMode::Move:
		MoveTool();
		break;
	case ManipToolMode::Rotate:
		RotateTool();
		break;
	case ManipToolMode::Scale:
		ScaleTool();
		break;
	default:
		break;
	}
}

void SceneEditor::MoveTool()
{
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float moveSpeed = 2.5f;

	float deltaMove = moveSpeed * Time::deltaTime;

	if (Input::GetKey(GLFW_KEY_LEFT))
		selectedGameObject->transform->Translate(-glm::normalize(glm::cross(forward, up)) * deltaMove);
	if (Input::GetKey(GLFW_KEY_RIGHT))
		selectedGameObject->transform->Translate(glm::normalize(glm::cross(forward, up)) * deltaMove);

	if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT)) == false)
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selectedGameObject->transform->Translate(deltaMove * forward);
		if (Input::GetKey(GLFW_KEY_DOWN))
			selectedGameObject->transform->Translate(-deltaMove * forward);
	}
	else // Pressing Shift
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selectedGameObject->transform->Translate(up * deltaMove);
		if (Input::GetKey(GLFW_KEY_DOWN))
			selectedGameObject->transform->Translate(-up * deltaMove);
	}
}

void SceneEditor::RotateTool()
{
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float rotSpeed = 25.0f;

	float deltaRot = rotSpeed * Time::deltaTime;

	if (Input::GetKey(GLFW_KEY_LEFT))
		selectedGameObject->transform->Rotate(glm::vec3(0, deltaRot, 0));
	if (Input::GetKey(GLFW_KEY_RIGHT))
		selectedGameObject->transform->Rotate(glm::vec3(0, -deltaRot, 0));

	if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT)) == false)
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selectedGameObject->transform->Rotate(glm::vec3(deltaRot, 0, 0));
		if (Input::GetKey(GLFW_KEY_DOWN))
			selectedGameObject->transform->Rotate(glm::vec3(-deltaRot, 0, 0));
	}
	else // Pressing Shift
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selectedGameObject->transform->Rotate(glm::vec3(0, 0, -deltaRot));
		if (Input::GetKey(GLFW_KEY_DOWN))
			selectedGameObject->transform->Rotate(glm::vec3(0, 0, deltaRot));
	}
}

void SceneEditor::ScaleTool()
{
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float scaleSpeed = 1.0f;

	float deltaScale = scaleSpeed * Time::deltaTime;

	if (Input::GetKey(GLFW_KEY_LEFT))
		selectedGameObject->transform->setLocalScale(selectedGameObject->transform->getLocalScale() + (-glm::normalize(glm::cross(forward, up))*deltaScale));
		//selected->transform->Scale(-glm::normalize(glm::cross(forward, up)) * deltaScale);
	if (Input::GetKey(GLFW_KEY_RIGHT))
		selectedGameObject->transform->setLocalScale(selectedGameObject->transform->getLocalScale() + (glm::cross(forward, up)* deltaScale));

	if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT)) == false)
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selectedGameObject->transform->setLocalScale(selectedGameObject->transform->getLocalScale() + (deltaScale * forward));
		if (Input::GetKey(GLFW_KEY_DOWN))
			selectedGameObject->transform->setLocalScale(selectedGameObject->transform->getLocalScale() + (-deltaScale * forward));
	}
	else // Pressing Shift
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selectedGameObject->transform->setLocalScale(selectedGameObject->transform->getLocalScale() + (up * deltaScale));
		if (Input::GetKey(GLFW_KEY_DOWN))
			selectedGameObject->transform->setLocalScale(selectedGameObject->transform->getLocalScale() + (-up * deltaScale));
	}
}

void SceneEditor::AddComponentMenu()
{
	std::vector<ComponentTypeInfo> componentTypes;
	for (std::pair<std::type_index, ComponentTypeInfo> element : Component::registry()) 
	{
		//std::cout << element.first.name() << ": " << element.second.name << std::endl;
		componentTypes.push_back(element.second);
	}
	for (size_t i = 0; i < componentTypes.size(); i++)
	{
		std::cout << "["<< i << "]: " << componentTypes[i].name << std::endl;
	}

	// Set focus to console.
	SetFocus(GetConsoleWindow());
	BringWindowToTop(GetConsoleWindow());

	// Select GameObject by index.
	int selectIndex = -1;
	while (selectIndex < 0 && selectIndex > componentTypes.size())
	{
		std::cout << "Select Component Index: ";
		std::cin >> selectIndex;
	}
	selectedGameObject->AddComponent(componentTypes[selectIndex].Constructor());
	std::cout << "Added new '" << componentTypes[selectIndex].name << "' to " << selectedGameObject->name << std::endl;

	// Set focus back to app.
	glfwFocusWindow(ApplicationManager::APP_WINDOW);
}
