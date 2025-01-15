#pragma once
#include "CustomExceptions.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include <sstream>

class Window;

class Application
{
	friend class Window;
public:
	// Default constructor
	Application();
	// Default Destructor
	~Application() {};

	// Deleting copy constructor
	Application(const Application&) = delete;

	bool Run();
	bool CreateAWindow(Application& _myApp, Timer& _myTimer);

private:
	const int ProcessMessage();
	bool RunAFrame();

	Window* window;
	Timer* timer;

	// Custom window procedure to handle multiple messages and
	// to define the behavior of the created window that is in context.
	static LRESULT CALLBACK CustWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static Keyboard keyboard;
	static Mouse mouse;

	static const int screenWidth = 640;
	static const int screenHeight = 480;
};