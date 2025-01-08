#include "MyWindows.h"

Application Application::myApp;
CustomResources CustomResources::myCustResource;

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
	winClass.hIcon = CustomResources::myCustResource.bigIcon;
	winClass.hCursor = nullptr;
	winClass.hbrBackground = nullptr;
	winClass.lpszMenuName = nullptr;
	winClass.lpszClassName = winClassName;
	winClass.hIconSm = CustomResources::myCustResource.smlIcon;
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

const int Application::ProcessMessage(const Window& win)
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

		// Testing the keyboard events
		if (keyboard.IsKeyboardKeyPressed(VK_MENU))
		{
			SetWindowText(win.hWnd, "ALT Pressed");
		}
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

LRESULT CALLBACK Application::CustWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// WM_KEYDOWN is for Non-System keys. When ALT is not pressed or F10 is not pressed.
	case WM_KEYDOWN:
		// WM_SYSKEYDOWN is for system keys like F10 or when any key is pressed after pressing and holding down ALT.
	case WM_SYSKEYDOWN:
		// To allow autorepeat while not creating repeated events for the WM_KEYDOWN message when autorepeat is off.
		// Bitmasking the lparam, in other words, checking if the 30th bit (chronologically 31st as it starts with 0) is true.
		// If true, it says the key is pressed repeatedly. But we'll allow it only if the autorepeat is enabled.
		if (!(lParam & 0x40000000) || myApp.keyboard.IsAutoRepeatEnabled())
		{
			myApp.keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
		// WM_KEYUP is for Non-System keys. When ALT is not released or F10 is not released.
	case WM_KEYUP:
		// WM_SYSKEYUP is for system keys like F10 or when any key is released while holding down ALT.
	case WM_SYSKEYUP:
		myApp.keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		myApp.keyboard.OnCharacter(static_cast<unsigned char>(wParam));
		break;
	case WM_KILLFOCUS:
		// When a window loses focus, we don't want the window to think some keystates are true. Hence clearing the keystates.
		myApp.keyboard.ClearKeyboardKeyStates();
		break;
	case WM_CLOSE:
		break;
	case WM_DESTROY:
		PostQuitMessage(99);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}