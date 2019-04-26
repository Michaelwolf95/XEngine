#include "ProjectSettingsManager.h"
#include <iostream>
#include <string>

namespace XEngine::Editor 
{
	ProjectSettingsManager::ProjectSettingsManager() {}
	ProjectSettingsManager::~ProjectSettingsManager() {}
	ProjectSettingsManager * ProjectSettingsManager::CreateManager()
	{
		ProjectSettingsManager* instance = &ProjectSettingsManager::getInstance();

		instance->Init();
		return instance;
	}
	int ProjectSettingsManager::Init()
	{
		projectConfig = ProjectConfig();

		// TODO: Load from JSON...

		isInitialized = true;
		return 0;
	}


	std::string ProjectSettingsManager::getProjectName()
	{
		//TODO: Load this from config.
		return projectConfig.projectName;
	}

	std::string ProjectSettingsManager::getProjectDirectory()
	{
		return projectConfig.projectDirectory;
	}

	void ProjectSettingsManager::LoadProjectSettings()
	{
		/*std::string appConfigPath = std::string(APP_CONFIG_FILE_PATH) + "AppConfig.json";

		try
		{
			std::ifstream file(appConfigPath);

			file >> appSettings;

			std::string tempStr = appSettings["Window_Title"];
			tempChar = new char[tempStr.length() + 1];
			strcpy_s(tempChar, tempStr.length() + 1, tempStr.c_str());
			this->config->appTitle = tempChar;

			this->config->screenHeight = appSettings["Screen_Height"];
			this->config->screenWidth = appSettings["Screen_Width"];

			file.close();
		}
		catch (const std::exception& e)
		{
			std::cout << "ERROR: Problem loading AppConfig.json." << std::endl;
		}*/
	}

	void ProjectSettingsManager::SaveProjectSettings()
	{
		/*try
		{
			std::string appConfigPath = std::string(APP_CONFIG_FILE_PATH) + "AppConfig.json";

			std::ofstream file(appConfigPath);

			this->saveSettings["Window_Title"] = this->config->appTitle;
			this->saveSettings["Screen_Height"] = this->config->screenHeight;
			this->saveSettings["Screen_Width"] = this->config->screenWidth;

			file << saveSettings.dump(4) << std::endl;

			file.close();

			delete[] tempChar;
		}
		catch (const std::exception& e)
		{
			std::cout << "ERROR: Problem saving to file." << std::endl;
		}*/
	}


}