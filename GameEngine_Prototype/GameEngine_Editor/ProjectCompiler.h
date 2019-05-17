#pragma once
#include "Singleton.h"
#include <string>
#include <Windows.h>
namespace XEngine::Editor {
	class ProjectCompiler : public Singleton<ProjectCompiler>
	{
		friend class Singleton<ProjectCompiler>;
	public:
		bool isInitialized = false;
		static ProjectCompiler* CreateManager();	// Create static instance and configure manager
		int Init();									// Init instance.

		// Default names and paths.
		// TODO: Save these to a preferences file.
		std::string batchFileName = "Recompile.bat";
		std::string libraryName = "XEngineProject.dll";
		std::string devEnvPath = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\IDE\\";
		//std::string solutionPath = "..\\..\\DemoProject\\DemoProject.sln";
		//std::string projectPath = "DemoProject\\DemoProject.vcxproj";

		std::string getSolutionPath();
		std::string getProjectPath();
		std::string getDevEnvPath();

		bool LoadProject();			// Load library and compile if necessary.
		bool CompileProject();
		bool LoadProjectLibrary();
		bool UnloadProjectLibrary();

		bool Recompile();

		// Check if the project needs to be recompiled. (NOT YET IMPLEMENTED)
		//https://stackoverflow.com/questions/50881272/how-can-i-get-vs-to-consider-my-project-dirty-when-only-an-exe-content-item-i
		bool CheckIfProjectDirty();

		static void ConvertPathToBackslash(std::string &path);
	protected:
		ProjectCompiler();

	private:
		HINSTANCE projectLibraryInstance;
	};

}