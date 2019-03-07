#include "SceneEditor.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "Shader.h"
#include "SimpleModelComponent.h"
#include "PrimitiveModels.h"
#include "TestMoverComponent.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "MeshRenderer.h"
//TODO: Put 'libboost_filesystem-vc141-mt-gd-x32-1_68.lib' in Libraries folder so we can use this.
//#include <boost/filesystem.hpp>
#include <direct.h> // Alternative to boost filesystem. This limits us to Windows/Linux
// imGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

SceneEditor * SceneEditor::CreateManager()
{
	SceneEditor* instance = &SceneEditor::getInstance();
	instance->Init();
	return instance;
}

int SceneEditor::Init()
{
	LoadEditorConfig();
	isInitialized = true;

	editorCameraGameObject = new GameObject("EditorCamGo");
	//editorCamera()

	std::shared_ptr<EditorCamera> editCamPtr(new EditorCamera());
	editorCamera = editCamPtr.get();
	editorCameraGameObject->AddComponent(editCamPtr);
	//std::cout << "EditorCam Address: " << editorCamera << std::endl;

	GLFWwindow* window = ApplicationManager::APP_WINDOW;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	const char* glsl_version = "#version 130";

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return 0;
}

SceneEditor::SceneEditor()
{
	//executeInEditMode = true;
	std::cout << "SCENE EDITOR: Press CTRL+SHIFT+E to Edit scene." << std::endl;
}

SceneEditor::~SceneEditor()
{
	//OutputDebugStringW(L"Trying to Save EditorConfig.\n");
	SaveEditorConfig();
}

void SceneEditor::ShutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void SceneEditor::EditorPreRender()
{
	ImGui::Render();
}

void SceneEditor::EditorPostRender()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SceneEditor::getInstance().DrawEditorGizmos();
}

#pragma region EDITOR CONFIG

void SceneEditor::LoadEditorConfig()
{
	editorConfig = new EditorConfig();
	std::string configPath = std::string(EDITOR_CONFIG_FILE_PATH);

	JSON configJSON;
	char* tempChar;
	try
	{
		std::ifstream file(configPath);

		file >> configJSON;

		std::string tempStr = configJSON["First_Scene_Filepath"];
		tempChar = new char[tempStr.length() + 1];
		strcpy_s(tempChar, tempStr.length() + 1, tempStr.c_str());

		editorConfig->firstSceneFilepath = tempChar;


		file.close();
		delete[] tempChar;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem loading EditorConfig.json." << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "Creating new EditorConfig.json" << std::endl;
		_mkdir("../Settings/Editor");

		editorConfig->firstSceneFilepath = "";
		SaveEditorConfig();
	}
}

void SceneEditor::SaveEditorConfig()
{
	OutputDebugStringW(L"Saving Editor Config...\n");
	std::string configPath = std::string(EDITOR_CONFIG_FILE_PATH);
	JSON configJSON;
	try
	{
		std::ofstream file(configPath);

		configJSON["First_Scene_Filepath"] = this->editorConfig->firstSceneFilepath.c_str();

		file << configJSON.dump(4) << std::endl;

		file.close();

		OutputDebugStringW(L"Saved Editor Config.\n");
		std::cout << "Saved EditorConfig.json with filePath: " << editorConfig->firstSceneFilepath << std::endl;
		//delete[] tempChar;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem Saving EditorConfig." << std::endl;
		OutputDebugStringW(L"Failed to Save EditorConfg.\n");
	}
}

void SceneEditor::LoadInitialEditorScene()
{
	bool exists = false;
	Scene_ptr scene(new Scene("TEMP_NAME"));
	if (editorConfig->firstSceneFilepath.empty() == false)
	{
		exists = SceneManager::getInstance().LoadSceneFromFile(*scene, editorConfig->firstSceneFilepath.c_str());
		if (exists)
		{
			std::cout << "Loading First Editor Scene from File!" << std::endl;
			//scene = &sc;
			// Activate Scene
			selectedGameObject = nullptr;
			SceneManager::getInstance().SetActiveScene(scene);
			return;
		}
	}
	if(exists == false)
	{
		std::cout << "Creating new Empty Scene" << std::endl;
		scene = SceneManager::getInstance().CreateNewScene();
		editorConfig->firstSceneFilepath = scene->filePath;
		SaveEditorConfig();

		// Create Scene
		//GameObject_ptr editorGo = scene->CreateGameObject("EditorCamera");
		//editorGo->AddComponent(new SceneEditor());
		//editorGo->AddComponent(std::make_shared<EditorCamera>(EditorCamera()));

		// SAVE SCENE
		SceneManager::getInstance().SaveSceneToFile(*scene);

		// Activate Scene
		selectedGameObject = nullptr;
		SceneManager::getInstance().SetActiveScene(scene);
	}
	
}

#pragma endregion

void SceneEditor::StartEditMode()
{
	//std::cout << "SCENE EDITOR: Press CTRL+SHIFT+E to Edit scene." << std::endl;
	if (ApplicationManager::getInstance().IsEditMode() == false)
	{
		ApplicationManager::getInstance().SetEditMode(true);
		if (SceneManager::getInstance().GetActiveScene() != nullptr)
		{
			selectedGameObject = nullptr;
			SceneManager::getInstance().ReloadSceneFromFile();
		}
		else
		{
			selectedGameObject = nullptr;
			SceneEditor::getInstance().LoadInitialEditorScene();
		}
		
		std::cout << "ENTERING EDIT MODE =========================" << std::endl;
		std::cout << "\tCTRL+E: Select Object to Edit" << std::endl;
		std::cout << "\tCTRL+S: Save Scene" << std::endl;
		std::cout << "\tCTRL+P: Print Scene" << std::endl;
		std::cout << "\tCTRL+I: Inspect Selected Object" << std::endl;
		std::cout << "\t[Q],[W],[E],[R]: Manipulate Selected Object." << std::endl;

		if (selectedGameObject == nullptr)
		{
			if (SceneManager::getInstance().GetActiveScene()->rootGameObjects.size() >= 1)
			{
				selectedGameObject = SceneManager::getInstance().GetActiveScene()->rootGameObjects[0];
				std::cout << "Auto-Selected GameObject[1]: " << selectedGameObject->name << std::endl;
			}
		}

		RenderManager::getInstance().setCurrentCamera(editorCamera);
	}
}

void SceneEditor::ExitEditMode()
{
	if (ApplicationManager::getInstance().IsEditMode())
	{
		std::cout << "EXITING EDIT MODE =========================" << std::endl;
		selectedGameObject = nullptr;
		ApplicationManager::getInstance().SetEditMode(false);
		SceneManager::getInstance().ReloadSceneFromFile();
	}
}

void SceneEditor::UpdateEditor()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	editorCameraGameObject->UpdateComponents();


	InspectorUpdate();

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

				//Shader* modelShader = new Shader("3Dmodel.vs", "3Dmodel.fs");
				Material* modelMaterial = new Material("3Dmodel.vs", "3Dmodel.fs");
				std::shared_ptr<MeshRenderer> modelNano(new MeshRenderer("3Dmodel/nanosuit/nanosuit.obj", modelMaterial));
				go->AddComponent(modelNano);

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
				// Set focus to console.
				SetFocus(GetConsoleWindow());
				BringWindowToTop(GetConsoleWindow());

				std::cout << "Renaming: " << selectedGameObject->name << std::endl;
				//std::string newName;
				std::cout << "Name: ";
				std::cin >> selectedGameObject->name;

				// Set focus back to app.
				glfwFocusWindow(ApplicationManager::APP_WINDOW);
			}
			else if (Input::GetKeyDown(GLFW_KEY_L))
			{
				LoadSceneMenu();
			}
		}
		else // Ctrl not pressed.
		{
			SelectManipTool();

			// Update Manip Tool
			if (selectedGameObject != nullptr)
			{
				ManipToolUpdate();
			}
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

void SceneEditor::LoadSceneMenu()
{
	// Set focus to console.
	SetFocus(GetConsoleWindow());
	BringWindowToTop(GetConsoleWindow());

	std::cout << "Select a Scene to Load: ";
	std::string sceneName;
	std::cin >> sceneName;
	Scene_ptr scene(new Scene("TEMP"));
	bool exists = SceneManager::getInstance().LoadSceneFromFileByName(*scene, sceneName.c_str());
	if (exists)
	{
		std::cout << "Scene Found!" << std::endl;
		// Activate Scene
		SceneManager::getInstance().SetActiveScene(scene);
		//std::cout << "LOADING NEW SCENE" << std::endl;
		editorConfig->firstSceneFilepath = scene->filePath;
	}
	else
	{
		std::cout << "Scene not found." << std::endl;
		std::cout << "Create new Scene? (y/n): " << std::endl;
		std::string answer;
		std::cin >> answer;
		if (answer == "Y" || answer == "y" || answer == "yes")
		{
			scene = SceneManager::getInstance().CreateNewScene(sceneName.c_str());
			editorConfig->firstSceneFilepath = scene->filePath;
			SaveEditorConfig();

			// SAVE SCENE
			SceneManager::getInstance().SaveSceneToFile(*scene);

			selectedGameObject = nullptr;
			// Activate Scene
			SceneManager::getInstance().SetActiveScene(scene);
		}
		else
		{
			std::cout << "Not creating new scene." << std::endl;
		}
	}


	// Set focus back to app.
	glfwFocusWindow(ApplicationManager::APP_WINDOW);
}


// Helper to display a little (?) mark which shows a tooltip when hovered.
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static void ShowDockingDisabledMessage()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
	ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
	ImGui::SameLine(0.0f, 0.0f);
	if (ImGui::SmallButton("click here"))
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void SceneEditor::UpdateDockSpace(bool* p_open)
{
	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}


	// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("X-Engine Editor", p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}
	else
	{
		ShowDockingDisabledMessage();
	}
	opt_flags |= ImGuiDockNodeFlags_PassthruDockspace;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Docking"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 opt_flags ^= ImGuiDockNodeFlags_NoSplit;
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
			if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))                opt_flags ^= ImGuiDockNodeFlags_NoResize;
			if (ImGui::MenuItem("Flag: PassthruDockspace", "", (opt_flags & ImGuiDockNodeFlags_PassthruDockspace) != 0))       opt_flags ^= ImGuiDockNodeFlags_PassthruDockspace;
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (opt_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          opt_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
			ImGui::Separator();
			if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}
		HelpMarker(
			"You can _always_ dock _any_ window into another by holding the SHIFT key while moving a window. Try it now!" "\n"
			"This demo app has nothing to do with it!" "\n\n"
			"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
			"ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
			"(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
		);

		ImGui::EndMenuBar();
	}

	ImGui::End();
}


void SceneEditor::InspectorUpdate()
{
	// ====
	bool open_dockspace = true;
	UpdateDockSpace(&open_dockspace);

	// ===
	//bool show_demo_window = true;
	//ImGui::ShowDemoWindow(&show_demo_window);

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;


	// We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
	//ApplicationManager::APP_WINDOW.
	//ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(250, 680), ImGuiCond_Once);// , ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowDockID(ImGui::GetID("EditorDockSpace"), ImGuiCond_Once);
	ImGui::SetNextWindowDockID(ImGui::GetWindowDockID(), ImGuiCond_Once);

	// Main body of the Demo window starts here.
	if (!ImGui::Begin("Inspector"))//, p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	
	//ImGuiContext* ctx = ImGui::GetCurrentContext();
	//ImGui::init

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			//ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene"))
		{

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			//ImGui::MenuItem("Metrics", NULL, &show_app_metrics);
			//ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
			//ImGui::MenuItem("About Dear ImGui", NULL, &show_app_about);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//ImGui::
	if (selectedGameObject != nullptr)
	{
		ImGui::Text("Inspecting (%s)", selectedGameObject->name.c_str());
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Transform"))
		{
			glm::vec3 pos = selectedGameObject->transform->getPosition();
			ImGui::InputFloat3("Position", (float*)&pos);
			if (pos != selectedGameObject->transform->getPosition())
			{
				selectedGameObject->transform->setLocalPosition(pos);
			}
			glm::vec3 rot = selectedGameObject->transform->getLocalRotationEuler();
			ImGui::InputFloat3("Rotation", (float*)&rot);
			if (rot != selectedGameObject->transform->getLocalRotationEuler())
			{
				selectedGameObject->transform->setLocalRotationEuler(rot);
			}
			glm::vec3 scale = selectedGameObject->transform->getLocalScale();
			ImGui::InputFloat3("Scale", (float*)&scale);
			if (scale != selectedGameObject->transform->getLocalScale())
			{
				selectedGameObject->transform->setLocalScale(scale);
			}
		}
		for (size_t i = 0; i < selectedGameObject->components.size(); i++)
		{
			std::string componentTypeName = Component::registry()[typeid(*selectedGameObject->components[i])].name;
			if (ImGui::CollapsingHeader(componentTypeName.c_str()))
			{
				//TODO: Add inspector element for every component.

			}
		}


	}
	else
	{
		ImGui::Text("No GameObject Selected.");
	}

	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void SceneEditor::ConfigureWindowLayout()
{
	// Set position and size of console.
	int border_thickness = GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION);
	RECT screenDimensions; //Without Taskbar
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &screenDimensions, 0);
	HWND console = GetConsoleWindow();
	int consoleWidth = 600;
	int consoleHeight = screenDimensions.bottom - border_thickness;
	int consoleX = 0;
	int consoleY = 0;// border_thickness;
	MoveWindow(console, consoleX, consoleY, consoleWidth, consoleHeight, TRUE);


	// Set position of window.
	int appPosX = consoleWidth;
	int appPosY = border_thickness; // border_thickness
	glfwSetWindowPos(ApplicationManager::APP_WINDOW, appPosX, appPosY);
}

