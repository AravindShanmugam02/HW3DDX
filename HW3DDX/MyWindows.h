#pragma once
#include "MyApplication.h"
#include "Graphics.h"

class Window
{
public:
	// Default constructor
	Window() {};
	// A constructor
	Window(Application& _myApp, Timer& _myTimer);
	// Default Destructor
	~Window();

	void SetWindowTitle();
	Graphics& GetGraphics() { return *gfx; };
private:
	// A handle for the current window module/instance
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

	Application* app;
	Timer* timer;

	/*
	ashan: [INFO] We are making it to be unique ptr as we want to defer its initialization as
	graphics constructor needs a valid handle to window (HWND).
	It is only obtained after Window constructor gets executed.
	*/
	std::unique_ptr<Graphics> gfx;
};