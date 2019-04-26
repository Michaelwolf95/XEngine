// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DEMOPROJECT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DEMOPROJECT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PROJECT_EXPORTS
#define PROJECT_API __declspec(dllexport)
#else
#define PROJECT_API __declspec(dllimport)
#endif

//
//// This class is exported from the dll
//class DEMOPROJECT_API CDemoProject {
//public:
//	CDemoProject(void);
//	// TODO: add your methods here.
//};
//
//extern DEMOPROJECT_API int nDemoProject;
//
//DEMOPROJECT_API int fnDemoProject(void);
