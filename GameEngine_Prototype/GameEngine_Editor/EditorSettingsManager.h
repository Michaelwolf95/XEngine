#pragma once
#include "XEngine.h"
#include "Singleton.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace XEngine::Editor {

	//
	struct EditorConfig
	{
		std::string devEnvPath = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\IDE\\";
		std::string launchProjectPath = "../../DemoProject/";
		// TODO: Implement this instead of the one in SceneEditor
		std::string launchScenePath = "";
	};

	class EditorSettingsManager : public Singleton<EditorSettingsManager>
	{
		friend class Singleton<EditorSettingsManager>;
	public:
		bool isInitialized = false;
		static EditorSettingsManager* CreateManager();
		int Init();
		EditorSettingsManager();
		~EditorSettingsManager();

		void LoadEditorSettings();
		void SaveEditorSettings();

		std::string getDevEnvName();

		EditorConfig editorConfig;
	private:
		//nlohmann::json editorConfigJSON;
	};

}