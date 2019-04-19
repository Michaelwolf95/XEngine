#include "EditorSettingsManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

namespace XEngine::Editor {

	EditorSettingsManager::EditorSettingsManager() {}
	EditorSettingsManager::~EditorSettingsManager() {}

	EditorSettingsManager * XEngine::Editor::EditorSettingsManager::CreateManager()
	{
		EditorSettingsManager* instance = &EditorSettingsManager::getInstance();

		instance->Init();
		return instance;
	}
	int EditorSettingsManager::Init()
	{
		editorConfig = EditorConfig();
		LoadEditorSettings();
		SaveEditorSettings();
		// TODO: Load from JSON...

		isInitialized = true;
		return 0;
	}

	void EditorSettingsManager::LoadEditorSettings()
	{
		std::string settingsPath = "../EditorSettings/EditorSettings.json";

		try
		{
			std::ifstream file(settingsPath);
			nlohmann::json editorConfigJSON;
			file >> editorConfigJSON;

			editorConfig.devEnvPath = editorConfigJSON["Visual_Studio_DevEnv_Path"];
			editorConfig.launchProjectPath = editorConfigJSON["Launch_Project_Path"];
			editorConfig.launchScenePath = editorConfigJSON["Launch_Scene_Path"];

			file.close();
		}
		catch (const std::exception& e)
		{
			std::cout << "ERROR: Problem loading EditorConfig.json." << std::endl;
		}
	}

	void EditorSettingsManager::SaveEditorSettings()
	{
		std::string settingsPath = "../EditorSettings/EditorSettings.json";

		try
		{
			std::ofstream file(settingsPath);
			nlohmann::json editorConfigJSON;

			editorConfigJSON["Visual_Studio_DevEnv_Path"] = editorConfig.devEnvPath;
			editorConfigJSON["Launch_Project_Path"] = editorConfig.launchProjectPath;
			editorConfigJSON["Launch_Scene_Path"] = editorConfig.launchScenePath;

			file << editorConfigJSON.dump(4) << std::endl;
			file.close();
		}
		catch (const std::exception& e)
		{
			std::cout << "ERROR: Problem saving EditorConfig.json." << std::endl;
		}
	}

	std::string EditorSettingsManager::getDevEnvName()
	{
		return editorConfig.devEnvPath;
	}

}