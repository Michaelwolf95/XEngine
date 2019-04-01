#include "EditorCamera.h"
#include "ApplicationManager.h"
#include "GameObject.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ApplicationManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "AudioEngine.h"
//using namespace XEngine;

BOOST_CLASS_EXPORT_GUID(XEngine::Editor::EditorCamera, "EditorCamera")
namespace XEngine::Editor
{

	EditorCamera::EditorCamera()
	{
		executeInEditMode = true;
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f),
			(float)ApplicationManager::config->screenWidth / (float)ApplicationManager::config->screenHeight,
			0.1f, 100.0f);
		clearColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	}

	EditorCamera::~EditorCamera()
	{
		std::cout << "\tDeleting EditorCamera" << std::endl;
		if (RenderManager::getInstance().getCurrentCamera() == this)
		{
			RenderManager::getInstance().setCurrentCamera(nullptr);
		}
	}

	glm::mat4 EditorCamera::getProjection()
	{
		return projection;
	}

	glm::mat4 __stdcall EditorCamera::getView()
	{
		//std::cout << "Getting EditorCamera View..." << std::endl;
		glm::vec3 cameraPos = gameObject->transform->getPosition();
		return (glm::mat4) glm::lookAt(cameraPos, cameraPos + gameObject->transform->getForwardDirection(), gameObject->transform->getUpDirection());
	}

	void EditorCamera::Start()
	{
		if (ApplicationManager::getInstance().IsEditMode())
		{
			//RenderManager::getInstance().setCurrentCamera(this);
			isBeingUsed = true;
		}
	}
	void EditorCamera::Update()
	{
		// Swap if edit mode changes.
		if (!isBeingUsed)
		{
			if (ApplicationManager::getInstance().IsEditMode())
			{
				//RenderManager::getInstance().setCurrentCamera(this);
				isBeingUsed = true;
			}
		}
		else
		{
			if (!ApplicationManager::getInstance().IsEditMode())
			{
				//RenderManager::getInstance().FindCameraInScene(SceneManager::getInstance().GetActiveScene().get());
				isBeingUsed = false;
			}
		}

		if (isBeingUsed)
		{

			glm::vec3 forward = this->gameObject->transform->getForwardDirection();
			glm::vec3 up = this->gameObject->transform->getUpDirection();

			// Editor Camera Control.

			// Move forward-back with Mouse Wheel.
			if (abs(Input::getInstance().GetScrollOffsetY()) > 0)
			{
				float deltaZoom = zoomSpeed * GameTime::deltaTime * Input::getInstance().GetScrollOffsetY();
				this->gameObject->transform->Translate(deltaZoom * forward);
			}

			switch (camMode)
			{
			case EditorCameraMode::None:
				if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
				{
					clickPos = Input::GetMousePos();
					lastDragPos = clickPos;
					glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					camMode = EditorCameraMode::Rotate;
					break;
				}
				if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) || (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && Input::GetKey(GLFW_KEY_LEFT_CONTROL)))
				{
					clickPos = Input::GetMousePos();
					lastDragPos = clickPos;
					glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					camMode = EditorCameraMode::Pan;
					break;
				}
				break;
			case EditorCameraMode::Rotate:
				if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
				{
					glm::vec2 currentDragPos = Input::GetMousePos();
					glm::vec2 deltaPos = currentDragPos - lastDragPos;
					lastDragPos = currentDragPos;
					float deltaYRot = xRotSpeed * GameTime::deltaTime * deltaPos.x;
					float deltaXRot = yRotSpeed * GameTime::deltaTime * deltaPos.y;

					glm::vec3 localRot = gameObject->transform->getLocalRotationEuler();

					// ALTERNATIVE USING LOOKAT
					//glm::vec3 direction = gameObject->transform->getForwardDirection();
					//direction.x -= deltaYRot;
					//direction.y -= deltaXRot;
					//gameObject->transform->LookAt(gameObject->transform->getPosition() - direction);

					localRot = glm::vec3(localRot.x + deltaXRot, localRot.y - deltaYRot, localRot.z);
					gameObject->transform->setLocalRotationEuler(localRot);
				}
				else
				{
					glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					camMode = EditorCameraMode::None;
				}
				break;
			case EditorCameraMode::Pan:
				if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) || Input::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
				{
					glm::vec2 currentDragPos = Input::GetMousePos();
					glm::vec2 deltaPos = currentDragPos - lastDragPos;
					lastDragPos = currentDragPos;
					float deltaXPan = panSpeed * GameTime::deltaTime * deltaPos.x;
					float deltaYPan = panSpeed * GameTime::deltaTime * deltaPos.y;// Input::GetDeltaPosY();

					glm::vec3 right = gameObject->transform->getRightDirection();
					//glm::vec3 up = gameObject->transform->getUpDirection();

					this->gameObject->transform->Translate(right * deltaXPan);
					this->gameObject->transform->Translate(up * deltaYPan);

				}
				else
				{
					glfwSetInputMode(ApplicationManager::APP_WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					camMode = EditorCameraMode::None;
				}

				break;
			default:
				break;
			}

			// Popup Window Experiment.
			if (menuWindow != nullptr)
			{
				glfwMakeContextCurrent(menuWindow);
				glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			//std::cout << "Read Input: " << Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) << std::endl;
			if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) && Input::GetKey(GLFW_KEY_LEFT_SHIFT))
			{
				if (menuWindow != nullptr)
				{
					glfwDestroyWindow(menuWindow);
					menuWindow = nullptr;
					glfwSwapBuffers(ApplicationManager::APP_WINDOW);
					glfwMakeContextCurrent(ApplicationManager::APP_WINDOW);
					return;
				}
				std::cout << "Opening 'Context Menu'" << std::endl;
				//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_FLOATING, true);
				glfwWindowHint(GLFW_DECORATED, false);
				glfwWindowHint(GLFW_VISIBLE, true);
				////glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
				//glfwWindowHint(GLFW_RESIZABLE, false);

				menuWindow = glfwCreateWindow(120, 220, "Second Window", NULL, ApplicationManager::APP_WINDOW);
				//GLFWwindow* second_window = glfwCreateWindow(100, 200, "Second Window", 0, 0);
				//GLFWwindow* window = glfwCreateWindow(250, 500, "My Title", NULL, NULL);
				//glfwSetWindowAttrib(second_window, GLFW_DECORATED);
				//glfwSetWindowparam

				int xPos, yPos;
				double xPosCursor, yPosCursor;
				//getting cursor position
				glfwGetWindowPos(ApplicationManager::APP_WINDOW, &xPos, &yPos);
				glfwGetCursorPos(ApplicationManager::APP_WINDOW, &xPosCursor, &yPosCursor);
				glfwSetWindowPos(menuWindow, xPos + xPosCursor, yPos + yPosCursor);

				glfwShowWindow(menuWindow);
				glfwSwapBuffers(menuWindow);
				//glfwMakeContextCurrent(second_window);
			}

			// AUDIO TEST
			if (Input::GetKeyDown(GLFW_KEY_P))
			{
				std::cout << "Playing sound...?" << std::endl;
				//Audio
				//string soundPath1 = "../Assets/sounds/old-car-engine_daniel_simion.mp3";
				std::string soundPath1 = "../Assets/sounds/inception_sound.mp3";

				//Implementation* sgpImplementation = new Implementation();
				//sgpImplementation->~Implementation;
				CAudioEngine test;
				test.Init();

				//LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
				//parameters about streaming, looping, and whether or not it's a 3D sound
				test.LoadSound(soundPath1, false, false, true);
				test.PlaySounds(soundPath1);
			}

		}
	}
}