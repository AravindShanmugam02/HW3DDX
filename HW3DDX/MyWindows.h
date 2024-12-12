#pragma once
#include "SlimWindows.h"

// Custom window procedure to handle multiple messages and
// to define the behavior of the created window that is in context.
LRESULT CALLBACK CustWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class MyWindows
{
public:
	// Default constructor
	MyWindows();
	// Default Destructor
	~MyWindows();

/////////// Making it a singleton ///////////
	// Deleting copy constructor
	MyWindows(const MyWindows&) = delete;
	// Deleting overloaded assignment operator
	MyWindows& operator=(const MyWindows&) = delete;
	// MyWindows class instance to access members
	static MyWindows myWindows;
/////////////////////////////////////////////

	const int ProcessMessage();

protected:

private:
	// To hold the instance handle of the current window module
	HINSTANCE newWindowInstanceHandle;

	// Used as an handle to the window created
	HWND hWnd;
	// Give it a name, that can be used later.
	const char* winClassName = "HW3DDX";
	// Give it a window name that gets displayed in the title bar
	const char* winTitleName = "3DHWACCDEMO";

	void RegisterAWindowClass(const WNDCLASSEX& winClass);
	HWND CreateAWindow();
	void ShowAWindow(const HWND& hwnd);

	int screenWidth = 640;
	int screenHeight = 480;
};