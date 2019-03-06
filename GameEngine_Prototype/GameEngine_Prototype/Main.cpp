#define GLM_FORCE_RADIANS
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#define STB_DEFINE  
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "DebugUtility.h" // Define only once
#include "Serialization.h"
#include "AssetManager.h"
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "Input.h"

#define X_EDIT_MODE

#ifdef X_EDIT_MODE
#include "SceneEditor.h"
#else
#include "TestScenes.h"
#endif


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// ENTRY POINT
int main()
{
	std::cout << "===== LAUNCHING X-ENGINE =====" << std::endl;
	
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	 // Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	
	// Init Managers
	ApplicationManager::CreateManager();
	Time::CreateManager();
	Input::CreateManager();
	RenderManager::CreateManager();
	SceneManager::CreateManager();
	AssetManager::CreateManager();

#ifdef X_EDIT_MODE
	SceneEditor::CreateManager();
#endif

	// Create & Load Scene
#ifdef X_EDIT_MODE
	SceneEditor::getInstance().StartEditMode();
#else
	RunTestScene();
#endif
	//float f = 2.0f;
	//static char buf[32];
	//ImGui::Text("Hello, world %d", 123);
	//if (ImGui::Button("Save"))
	//{
	//	// do stuff
	//}
	//ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

	
	// FRAME LOOP
	while (!ApplicationManager::getInstance().CheckIfAppShouldClose())
	{
		ApplicationManager::getInstance().ApplicationStartUpdate();
		Time::getInstance().UpdateTime();
		Input::getInstance().UpdateInput();

		// Editor Update
#ifdef X_EDIT_MODE
		SceneEditor::getInstance().UpdateEditor();
#endif

		// Do Game Logic here
		SceneManager::getInstance().UpdateActiveScene();

		RenderManager::getInstance().Render();

		Input::getInstance().EndUpdateFrame();
		ApplicationManager::getInstance().ApplicationEndUpdate();
	}

	ApplicationManager::getInstance().CloseApplication();
	return 0;
}