#include "XEngine.h"
#include "ProjectCompiler.h"
#include "ProjectSettingsManager.h"
#include <iostream>
#include "Component.h"

#include <Windows.h>
namespace XEngine::Editor {
	ProjectCompiler::ProjectCompiler() {}

	ProjectCompiler * ProjectCompiler::CreateManager()
	{
		ProjectCompiler* instance = &ProjectCompiler::getInstance();
		instance->Init();
		return instance;
	}

	int ProjectCompiler::Init()
	{


		isInitialized = true;
		return 0;
	}

	//typedef void(*f_funci)();

	std::string ProjectCompiler::getSolutionPath()
	{
		/*std::string devEnvPath = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\IDE\\";
		std::string solutionPath = "..\\..\\DemoProject\\DemoProject.sln";
		std::string projectPath = "DemoProject\\DemoProject.vcxproj";*/
		std::string projectDir = PROJECT_FILE_PATH;
		ConvertPathToBackslash(projectDir);
		ProjectSettingsManager* settings = &ProjectSettingsManager::getInstance();
		return (projectDir + settings->getProjectName() + ".sln");
	}

	std::string ProjectCompiler::getProjectPath()
	{
		std::string projectDir = PROJECT_FILE_PATH;
		ConvertPathToBackslash(projectDir);
		ProjectSettingsManager* settings = &ProjectSettingsManager::getInstance();
		return (projectDir + settings->getProjectName()+ "\\" + settings->getProjectName() + ".vcxproj");
	}

	std::string ProjectCompiler::getDevEnvPath()
	{
		return devEnvPath;
	}

	bool ProjectCompiler::LoadProject()
	{
		if (CheckIfProjectDirty())
		{
			CompileProject();
		}

		return LoadProjectLibrary();
	}

	bool ProjectCompiler::CompileProject()
	{
		UnloadProjectLibrary();
		std::cout << "Rebuilding Library..." << std::endl;
		std::string command = "Recompile.bat";
		char buffer[2048];
		std::sprintf(buffer, "%s \"%s\" \"%s\" \"%s\"",
			batchFileName.c_str(),
			getDevEnvPath().c_str(),
			getSolutionPath().c_str(),
			getProjectPath().c_str());
		std::cout << buffer  << std::endl;
		int result = system(buffer);
		std::cout << "Build Finished with exit code " << result << std::endl << std::endl;

		return (result == 0);
	}

	bool ProjectCompiler::LoadProjectLibrary()
	{
		//UnloadProjectLibrary();
		std::cout << "Loading Library..." << std::endl;
		if (projectLibraryInstance != NULL)
		{
			std::cout << "Project Library already loaded." << std::endl;
			return true;
		}
		projectLibraryInstance = LoadLibrary(TEXT(libraryName.c_str()));
		if (projectLibraryInstance == NULL)
		{
			std::cout << "[ERROR]: Could not load the dynamic library" << std::endl;
			return false;
		}

		std::cout << "Testing Load Library..." << std::endl;
		for (auto const& pair : Component::registry())
		{
			if (pair.second.name == "UserComponent")
			{
				Component_ptr c = pair.second.Constructor();
				c->Start();
			}
			//std::cout << "\t" << pair.second.name << std::endl;
		}

		//// resolve function address here
		//// THIS NAME IS BECAUSES OF __stdcall! LOOK AT DUMP TO FIGURE OUT REAL NAME.
		////f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "_print_some_number@0");
		//f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "print_some_number");
		////if (!funci) 
		//if (funci == NULL)
		//{
		//	std::cout << "[ERROR]: Could not locate the function" << std::endl;
		//	return EXIT_FAILURE;
		//}
		return true;
	}

	bool ProjectCompiler::UnloadProjectLibrary()
	{
		std::cout << "Unloading Library..." << std::endl;
		if (projectLibraryInstance == NULL)
		{
			std::cout << "No library to unload." << std::endl;
			return false;
		}
		FreeLibrary(projectLibraryInstance);
		//CoFreeLibrary(projectLibraryInstance);
		//projectLibraryInstance = LoadLibrary(TEXT(libraryName.c_str()));
		/*if (projectLibraryInstance->unused)
		{
			std::cout << "[ERROR]: Could not load the dynamic library" << std::endl;
			return false;
		}*/
		projectLibraryInstance = NULL;
		return true;
	}

	bool ProjectCompiler::CheckIfProjectDirty()
	{
		// TODO: Implement dirty check.
		return true;
	}

	void ProjectCompiler::ConvertPathToBackslash(std::string &path)
	{
		char sep = '/';
		std::replace(path.begin(), path.end(), '/', '\\');
	}
}