#include "MyWindows.h"

CustomResources CustomResources::myCustResource;

Window::Window(Application& _myApp, Timer& _myTimer)
	: newWindowInstanceHandle(GetModuleHandle(nullptr)),
	app(&_myApp),
	timer(&_myTimer)
{
	// To create a window class
	WNDCLASSEX winClass = { 0 };

	// Structure configuration part
	winClass.style = CS_OWNDC;
	winClass.lpfnWndProc = Application::CustWndProc;
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

	// Create graphics for this Window
	gfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	// Unregister a window class
	UnregisterClass(winClassName, newWindowInstanceHandle);
}

void Window::RegisterAWindowClass(const WNDCLASSEX& winClass)
{
	// To register a window class
	RegisterClassEx(&winClass);
}

HWND Window::CreateAWindow()
{
	// To calculate the adjusted size of our screen so that the client region is per our requirement
	RECT winRect;
	winRect.top = 200;
	winRect.bottom = winRect.top + app->screenHeight;
	winRect.left = 200;
	winRect.right = winRect.left + app->screenWidth;

	DWORD winStyle = WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;

	AdjustWindowRect(&winRect, winStyle, false);

	// To create a window
	return CreateWindowEx(
		0, winClassName,
		winTitleName, winStyle,
		winRect.top, winRect.left, winRect.right - winRect.left, winRect.bottom - winRect.top,
		nullptr, nullptr, newWindowInstanceHandle, nullptr);
}

void Window::ShowAWindow(const HWND& hwnd)
{
	if (hWnd != nullptr)
	{
		// To show the window
		ShowWindow(hWnd, SW_SHOW);
	}
}

void Window::SetWindowTitle()
{
	// Testing the keyboard events
	std::ostringstream keyboardString;
	{
		if (app->keyboard.IsKeyboardKeyPressed(VK_MENU))
		{
			keyboardString << "ATL Key Pressed";
		}
		else
		{
			keyboardString << "None";
		}
	}

	// Testing mouse events
	std::ostringstream mouseString;
	{
		const auto e = app->mouse.ReadMouseEvent();
		if (e.GetType() == Mouse::MouseEvents::EventType::MOVE)
		{
			mouseString <<
				" || Mouse MOVE position: (" <<
				e.GetMouseXPositionForEvent() <<
				", " << e.GetMouseYPositionForEvent() <<
				")";
		}
		else if (e.GetType() == Mouse::MouseEvents::EventType::DRAG)
		{
			mouseString <<
				" || Mouse DRAG position: (" <<
				e.GetMouseXPositionForEvent() <<
				", " << e.GetMouseYPositionForEvent() <<
				")";
		}
		// even if there is no event for move or drag, just display old position
		else
		{
			mouseString <<
				" || Mouse position: (" <<
				app->mouse.GetMouseXPosition() <<
				", " << app->mouse.GetMouseXPosition() <<
				")";
		}
	}

	// Testing mouse scroll events
	std::ostringstream mouseScrollString;
	{
		mouseScrollString <<
			" || Mouse Scroll " <<
			(app->mouse.GetMouseScrollsNumber() >= 0 ? "UP: " : "DOWN: ") <<
			app->mouse.GetMouseScrollsNumber();
	}

	// Testing FPS
	std::ostringstream fpsString;
	{
		fpsString << " || FPS: " << timer->GetFPS();
	}

	SetWindowText(hWnd, (keyboardString.str() + mouseString.str() + mouseScrollString.str() + fpsString.str()).c_str());
}