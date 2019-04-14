// DemoProject.cpp : Defines the exported functions for the DLL application.
//
#include "XEngineProject.h"

// This is an example of an exported variable
PROJECT_API int nDemoProject=0;

// This is an example of an exported function.
PROJECT_API int fnDemoProject(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
CDemoProject::CDemoProject()
{
    return;
}
