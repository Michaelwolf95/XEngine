#include "SceneEditor.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "Shader.h"
#include "SimpleModelComponent.h"
#include "PrimitiveModels.h"
#include "TestMoverComponent.h"
#include "ApplicationManager.h"

BOOST_CLASS_EXPORT_GUID(SceneEditor, "SceneEditor")

SceneEditor::SceneEditor()
{
	executeInEditMode = true;
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::Start()
{
	std::cout << "SCENE EDITOR: Press CTRL+SHIFT+E to Edit scene." << std::endl;
}

void SceneEditor::Update()
{
	// Swap edit mode
	if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT))
		&& (Input::GetKey(GLFW_KEY_LEFT_CONTROL) || Input::GetKey(GLFW_KEY_RIGHT_CONTROL))
		&& Input::GetKeyDown(GLFW_KEY_E))
	{
		if (ApplicationManager::getInstance().IsEditMode())
		{
			ApplicationManager::getInstance().SetEditMode(false);
			std::cout << "EXITING EDIT MODE =========================" << std::endl;
		}
		else
		{
			ApplicationManager::getInstance().SetEditMode(true);
			std::cout << "ENTERING EDIT MODE =========================" << std::endl;
			std::cout << "\tCTRL+E: Select Object to Edit" << std::endl;
			std::cout << "\tCTRL+S: Save Scene" << std::endl;
			std::cout << "\tCTRL+P: Print Scene" << std::endl;
			std::cout << "\tCTRL+I: Inspect Selected Object" << std::endl;
			std::cout << "\t[Q],[W],[E],[R]: Manipulate Selected Object." << std::endl;
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
			else if (Input::GetKeyDown(GLFW_KEY_E)) // "Edit" object - select an object to edit.
			{
				if (selected == nullptr)
				{
					Scene* scene = SceneManager::getInstance().GetActiveScene();
					//scene->PrintScene();

					// Set focus to console.
					SetFocus(GetConsoleWindow());
					BringWindowToTop(GetConsoleWindow());
					//glfwFocusWindow(glfwconsole)

					// Select GameObject by index.
					int selectIndex = -1;
					while (selectIndex < 0 && selectIndex > scene->rootGameObjects.size())
					{
						std::cout << "Select GameObject Index:" << std::endl;
						std::cin >> selectIndex;
						std::cout << std::endl;
						selected = scene->rootGameObjects[selectIndex];
					}
					std::cout << "Selected: " << selected->name << std::endl;

					// Set focus back to app.
					glfwFocusWindow(ApplicationManager::APP_WINDOW);
				}
				else
				{
					selected = nullptr;
					std::cout << "No longer selecting." << std::endl;
				}
			}
			else if(Input::GetKeyDown(GLFW_KEY_P)) // Print Scene
			{
				SceneManager::getInstance().GetActiveScene()->PrintScene();
			}
			else if (Input::GetKeyDown(GLFW_KEY_I)) // Inspect selected
			{
				if (selected == nullptr)
				{
					std::cout << "No Object to Inspect." << std::endl;
				}
				else
				{
					Scene* scene = SceneManager::getInstance().GetActiveScene();
					auto index = std::distance(scene->rootGameObjects.begin(), std::find(scene->rootGameObjects.begin(), scene->rootGameObjects.end(), selected));
					std::cout << "Inspecting Object: [" << index << "]: " << selected->name << std::endl;
					// Output current position.
					glm::vec3 pos = selected->transform->getPosition();
					std::cout << "Pos:  (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
					// Output current local scale.
					glm::vec3 scale = selected->transform->getLocalScale();
					std::cout << "Scale:(" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
					glm::quat rot = selected->transform->getLocalRotation();
					std::cout << "Rot:  (" << rot.x << ", " << rot.y << ", " << rot.z << ", " << rot.w << ")" << std::endl;
				}
			}
			else if (Input::GetKeyDown(GLFW_KEY_D)) // Delete selected
			{
				SceneManager::getInstance().GetActiveScene()->DeleteGameObject(selected);
				selected = nullptr;
			}
		}
		else // Ctrl not pressed.
		{
			SelectManipTool();
		}
	}


	// Update Manip Tool
	if (selected != nullptr)
	{
		ManipToolUpdate();
	}
}

void SceneEditor::OnDrawGizmos()
{
	if (selected != nullptr)
	{
		selected->transform->DrawGizmo();
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
		selected->transform->Translate(-glm::normalize(glm::cross(forward, up)) * deltaMove);
	if (Input::GetKey(GLFW_KEY_RIGHT))
		selected->transform->Translate(glm::normalize(glm::cross(forward, up)) * deltaMove);

	if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT)) == false)
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selected->transform->Translate(deltaMove * forward);
		if (Input::GetKey(GLFW_KEY_DOWN))
			selected->transform->Translate(-deltaMove * forward);
	}
	else // Pressing Shift
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selected->transform->Translate(up * deltaMove);
		if (Input::GetKey(GLFW_KEY_DOWN))
			selected->transform->Translate(-up * deltaMove);
	}
}

void SceneEditor::RotateTool()
{
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float rotSpeed = 25.0f;

	float deltaRot = rotSpeed * Time::deltaTime;

	if (Input::GetKey(GLFW_KEY_LEFT))
		selected->transform->Rotate(glm::vec3(0, deltaRot, 0));
	if (Input::GetKey(GLFW_KEY_RIGHT))
		selected->transform->Rotate(glm::vec3(0, -deltaRot, 0));

	if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT)) == false)
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selected->transform->Rotate(glm::vec3(deltaRot, 0, 0));
		if (Input::GetKey(GLFW_KEY_DOWN))
			selected->transform->Rotate(glm::vec3(-deltaRot, 0, 0));
	}
	else // Pressing Shift
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selected->transform->Rotate(glm::vec3(0, 0, deltaRot));
		if (Input::GetKey(GLFW_KEY_DOWN))
			selected->transform->Rotate(glm::vec3(0, 0, -deltaRot));
	}
}

void SceneEditor::ScaleTool()
{
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float scaleSpeed = 1.0f;

	float deltaScale = scaleSpeed * Time::deltaTime;

	if (Input::GetKey(GLFW_KEY_LEFT))
		selected->transform->setLocalScale(selected->transform->getLocalScale() + (-glm::normalize(glm::cross(forward, up))*deltaScale));
		//selected->transform->Scale(-glm::normalize(glm::cross(forward, up)) * deltaScale);
	if (Input::GetKey(GLFW_KEY_RIGHT))
		selected->transform->setLocalScale(selected->transform->getLocalScale() + (glm::cross(forward, up)* deltaScale));

	if ((Input::GetKey(GLFW_KEY_LEFT_SHIFT) || Input::GetKey(GLFW_KEY_RIGHT_SHIFT)) == false)
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selected->transform->setLocalScale(selected->transform->getLocalScale() + (deltaScale * forward));
		if (Input::GetKey(GLFW_KEY_DOWN))
			selected->transform->setLocalScale(selected->transform->getLocalScale() + (-deltaScale * forward));
	}
	else // Pressing Shift
	{
		if (Input::GetKey(GLFW_KEY_UP))
			selected->transform->setLocalScale(selected->transform->getLocalScale() + (-up * deltaScale));
		if (Input::GetKey(GLFW_KEY_DOWN))
			selected->transform->setLocalScale(selected->transform->getLocalScale() + (up * deltaScale));
	}
}
