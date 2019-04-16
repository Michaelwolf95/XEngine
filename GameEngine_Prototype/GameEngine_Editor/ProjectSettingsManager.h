#pragma once
#include "Singleton.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace XEngine::Editor {

	//
	struct ProjectConfig
	{
		char* projectName = (char*)"DemoProject";
		char* projectDirectory = (char*)"../../DemoProject/"; // Should be absolute path. This is temp.
	};

	class ProjectSettingsManager : public Singleton<ProjectSettingsManager>
	{
		friend class Singleton<ProjectSettingsManager>;
	public:
		bool isInitialized = false;
		static ProjectSettingsManager* CreateManager();
		int Init();
		ProjectSettingsManager();
		~ProjectSettingsManager();

		void LoadProjectSettings();
		void SaveProjectSettings();

		std::string getProjectName();
		std::string getProjectDirectory();

		ProjectConfig projectConfig;
	private:
		nlohmann::json projectConfigJSON;
	};

}