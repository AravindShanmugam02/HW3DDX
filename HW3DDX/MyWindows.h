#pragma once
#include "CustomExceptions.h"
#include "Keyboard.h"

class Application
{
public:
	class Window
	{
		friend class Application;
	public:
		// Default constructor
		Window();
		// Default Destructor
		~Window();

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

		int screenWidth = 640;
		int screenHeight = 480;
	};

	// Default constructor
	Application() {};
	// Default Destructor
	~Application() {};

	/////////// Making it a singleton ///////////
	// Deleting copy constructor
	Application(const Application&) = delete;
	// Deleting overloaded assignment operator
	Application& operator=(const Application&) = delete;
	// Application class instance to access members
	static Application myApp;
	/////////////////////////////////////////////

	const int ProcessMessage(const Window& win);

	Keyboard keyboard;

private:
	// Custom window procedure to handle multiple messages and
	// to define the behavior of the created window that is in context.
	static LRESULT CALLBACK CustWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};