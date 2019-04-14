#include "ProjectCompiler.h"
#include <iostream>
#include "Component.h"

#include <Windows.h>

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
	int n, a = 5, b = 3;
	std::sprintf(buffer, "%s \"%s\" \"%s\" \"%s\"", batchFileName.c_str(), devEnvPath.c_str(), solutionPath.c_str(), projectPath.c_str());
	std::cout << buffer << "||" << std::endl;
	int result = system(buffer);
	std::cout << "Build Finished with exit code " << result << std::endl << std::endl;

	return (result == 0);
}

bool ProjectCompiler::LoadProjectLibrary()
{
	//UnloadProjectLibrary();
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
	return true;
}

