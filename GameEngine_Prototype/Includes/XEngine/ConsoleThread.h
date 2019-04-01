#pragma once
/* CURRENTLY UNUSED 
Meant for running the console ins a seperate thread.
Based on: https://stackoverflow.com/questions/38358035/c-create-new-window-from-console-in-new-thread-win32
*/

#include <process.h>
#include <Windows.h>
#include <thread>
#include <iostream>
using namespace std;

unsigned int __stdcall mythread(void* data)
{

	cout << "/n created monitor window /n";

	const char* const myclass = "myclass";

	WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, WindowProcedure,
		0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
		LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
		0, myclass, LoadIcon(0,IDI_APPLICATION) };

	if (RegisterClassEx(&wndclass))
	{
		HWND window = CreateWindowEx(0, myclass, "title",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), 0);
		if (window)
		{
			ShowWindow(window, SW_SHOWDEFAULT);
			MSG msg;
			while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
		}
	}

	return 0;
}

long __stdcall WindowProcedure(HWND window, unsigned int msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{

		/*case checksomethinghere: {

		}*/
	default: // fall thru 

		return DefWindowProc(window, msg, wp, lp);
	}
}

void RunThread()
{
	HANDLE myhandle;

	myhandle = (HANDLE)_beginthreadex(0, 0, &mythread, 0, 0, 0);

}