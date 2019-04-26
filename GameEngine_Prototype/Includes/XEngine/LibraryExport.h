#pragma once
#ifdef ENGINE_EXPORTS
#	define ENGINE_API __declspec(dllexport)
#else
#	define ENGINE_API __declspec(dllimport)
#endif
// Other Libraries
#define GLAD_GLAPI_EXPORT
#ifdef ENGINE_EXPORTS
	#define API_MODE "Export Mode"
	#define _GLFW_BUILD_DLL
	#define GLAD_GLAPI_EXPORT_BUILD
	#define IMGUI_API __declspec(dllexport)
	//#define BOOST_ALL_DYN_LINK
	//#define BOOST_SERIALIZATION_SOURCE
#else
	#define API_MODE "Import Mode"
	#define GLFW_DLL
	#define IMGUI_API __declspec(dllimport)
	//#define BOOST_ALL_DYN_LINK
#endif



// DISABLE WARNINGS ===================================

// 4251: Type needs dll-interface
#pragma warning(disable: 4251)

// Disable: 'initializing': conversion from 'type1' to 'type2', possible loss of data.
#pragma warning(disable: 4244)

// 4244: 'initializing': truncation from 'type1' to 'type2'
#pragma warning(disable: 4305)

// 4018: '<': signed/unsigned mismatch
#pragma warning(disable: 4018) 

// 4101: 'e': unreferenced local variable
#pragma warning(disable: 4101) 
