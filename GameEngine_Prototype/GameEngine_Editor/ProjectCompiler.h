#pragma once
#include "Singleton.h"
#include <string>
#include <Windows.h>
class ProjectCompiler : public Singleton<ProjectCompiler>
{
	friend class Singleton<ProjectCompiler>;
public:
	bool isInitialized = false;
	static ProjectCompiler* CreateManager();	// Create static instance and configure manager
	int Init();									// Init instance.

	// Defaults
	std::string libraryName = "XEngineProject.dll";
	std::string batchFileName = "Recompile.bat";
	std::string devEnvPath = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\IDE\\";
	std::string solutionPath = "..\\GameEngine_Prototype.sln";
	std::string projectPath = "DemoProject\\DemoProject.vcxproj";

	bool LoadProject();			// Load library and compile if necessary.
	bool CompileProject();
	bool LoadProjectLibrary();
	bool UnloadProjectLibrary();

	// Check if the project needs to be recompiled.
	//https://stackoverflow.com/questions/50881272/how-can-i-get-vs-to-consider-my-project-dirty-when-only-an-exe-content-item-i
	bool CheckIfProjectDirty();

protected:
	ProjectCompiler();

private:
	HINSTANCE projectLibraryInstance;
};

