#pragma once
/* This is the main entry point for test scenes.
This is NOT A PERMANENT SOLUTION - we should change this to a local config file later.

To start testing, change this entry point to route to YOUR entry point.
e.g: RunTestScene_Michael()

DO NOT COMMIT THE CHANGES TO THIS FILE.
*/

//#include "TestScenes_Michael.h"
//#include "TestScenes_Tam.h"
//#include "TestScenes_James.h"
//#include "TestScenes_Donato.h"
#include "TestScenes_Dennis.h"


void RunTestScene()
{
	RunTestScene_Dennis();
}