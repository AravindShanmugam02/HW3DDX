#include "MyWindows.h"

Application Application::myApp;

const int Application::ProcessMessage()
{
	// To make a message pump
	// 1. First, need a MSG
	MSG winMessage;

	// To handle different MSG code that is returned from GetMessage().
	// Therefore, we declare a BOOL to hold the returned code.
	BOOL winRetCode;

	// We use while so that the window stays on as long as we are quitting or it runs into error.
	while (// 2. Second, pass the MSG to GetMessage()
		(winRetCode = GetMessage(&winMessage, nullptr, 0, 0)) > 0)
	{
		// 3. Translate the MSG. (ashanmugam [TODO] To Find More?)
		TranslateMessage(&winMessage);
		// 4. Dispatch the MSG to Win32's window procedure for the window created.
		DispatchMessage(&winMessage);
	}

	if (winRetCode == -1)
	{
		// (ashanmugam [TODO] To handle error)
		return -1;
	}
	else
	{
		return winMessage.wParam;
	}
}

LRESULT CALLBACK CustWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		break;
	case WM_DESTROY:
		PostQuitMessage(99);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Application::Window::Window()
	: newWindowInstanceHandle(GetModuleHandle(nullptr))
{
	// To create a window class
	WNDCLASSEX winClass = { 0 };

	// Structure configuration part
	winClass.style = CS_OWNDC;
	winClass.lpfnWndProc = CustWndProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = newWindowInstanceHandle;
	winClass.hIcon = nullptr;
	winClass.hCursor = nullptr;
	winClass.hbrBackground = nullptr;
	winClass.lpszMenuName = nullptr;
	winClass.lpszClassName = winClassName;
	winClass.hIconSm = nullptr;
	winClass.cbSize = sizeof(winClass);

	RegisterAWindowClass(winClass);
	hWnd = CreateAWindow();
	ShowAWindow(hWnd);
}

Application::Window::~Window()
{
	// Unregister a window class
	UnregisterClass(winClassName, newWindowInstanceHandle);
}

void Application::Window::RegisterAWindowClass(const WNDCLASSEX& winClass)
{
	// To register a window class
	RegisterClassEx(&winClass);
}

HWND Application::Window::CreateAWindow()
{
	// To calculate the adjusted size of our screen so that the client region is per our requirement
	RECT winRect;
	winRect.top = 200;
	winRect.bottom = winRect.top + screenHeight;
	winRect.left = 200;
	winRect.right = winRect.left + screenWidth;

	DWORD winStyle = WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;

	AdjustWindowRect(&winRect, winStyle, false);

	// To create a window
	return CreateWindowEx(
		0, winClassName,
		winTitleName, winStyle,
		winRect.top, winRect.left, winRect.right - winRect.left, winRect.bottom - winRect.top,
		nullptr, nullptr, newWindowInstanceHandle, nullptr);
}

void Application::Window::ShowAWindow(const HWND& hwnd)
{
	if (hWnd != nullptr)
	{
		// To show the window
		ShowWindow(hWnd, SW_SHOW);
	}
}