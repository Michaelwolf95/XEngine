#pragma once

#ifdef LIBRARY_EXPORTS
#    define DLLExport __declspec(dllexport)
#else
#    define DLLExport __declspec(dllimport)
#endif

// Defining the components for DLL export bypasses issues where 
// the static library wont compile unused classes.
// We need them to compile for registration.
// https://stackoverflow.com/questions/873731/object-registration-in-static-library
//define DLLExport __declspec(dllexport)