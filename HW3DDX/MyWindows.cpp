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
	winRect.bottom = winRect.top + myApp.screenHeight;
	winRect.left = 200;
	winRect.right = winRect.left + myApp.screenWidth;

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

void Application::Window::SetWindowTitle()
{
	// Testing the keyboard events
	std::ostringstream keyboardString;
	{
		if (myApp.keyboard.IsKeyboardKeyPressed(VK_MENU))
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
		const auto e = myApp.mouse.ReadMouseEvent();
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
	}

	// Testing mouse scroll events
	std::ostringstream mouseScrollString;
	{
		mouseScrollString <<
			" || Mouse Scroll " <<
			(myApp.mouse.GetMouseScrollsNumber() >= 0 ? "UP: " : "DOWN: ") <<
			myApp.mouse.GetMouseScrollsNumber();
	}

	SetWindowText(hWnd, (keyboardString.str() + mouseString.str() + mouseScrollString.str()).c_str());
}

const int Application::ProcessMessage(Window& win)
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

		// Testing keyboard and mouse system by setting title
		win.SetWindowTitle();
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
	{
		// To allow autorepeat while not creating repeated events for the WM_KEYDOWN message when autorepeat is off.
		// Bitmasking the lparam, in other words, checking if the 30th bit (chronologically 31st as it starts with 0) is true.
		// If true, it says the key is pressed repeatedly. But we'll allow it only if the autorepeat is enabled.
		if (!(lParam & 0x40000000) || myApp.keyboard.IsAutoRepeatEnabled())
		{
			myApp.keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
	}
		break;
		// WM_KEYUP is for Non-System keys. When ALT is released or F10 is released.
	case WM_KEYUP:
		// WM_SYSKEYUP is for system keys like F10 or when any key is released while holding down ALT.
	case WM_SYSKEYUP:
	{
		myApp.keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
	}
		break;
	case WM_CHAR:
	{
		myApp.keyboard.OnCharacter(static_cast<unsigned char>(wParam));
	}
		break;
	case WM_KILLFOCUS:
	{
		// When a window loses focus, we don't want the window to think some keystates are true. Hence clearing the keystates.
		myApp.keyboard.ClearKeyboardKeyStates();
	}
		break;
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		myApp.mouse.OnMouseLeftPressed(pt.x, pt.y);
	}
		break;
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		myApp.mouse.OnMouseRightPressed(pt.x, pt.y);
	}
		break;
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		myApp.mouse.OnMouseLeftReleased(pt.x, pt.y);
	}
		break;
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		myApp.mouse.OnMouseRightReleased(pt.x, pt.y);
	}
		break;
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		// This means the mouse pointer/cursor is inside the client region.
		if ((pt.x >= 0 && pt.x <= myApp.screenWidth) && (pt.y >= 0 && pt.y <= myApp.screenHeight))
		{
			if (!myApp.mouse.IsInsideClientRegion())
			{
				myApp.mouse.OnMouseInsideClientRegion(pt.x, pt.y);

				// Sets a window on which the mouse is over to capture mouse events.
				// It also captures mouse move event outside client region
				// when a button was down while inside client region and still down outside.
				SetCapture(hWnd);
			}

			// WM_MOUSEMOVE carries some set of button or key down message in wParam.
			// Bit masking wParam to check if we are dragging something.
			if (wParam & MK_LBUTTON) // could also use IsLeftButtonPressed from Mouse class.
			{
				myApp.mouse.OnMouseDrag(pt.x, pt.y);
			}
			else
			{
				// Unlike DRAG event, only if the mouse cursor / pointer is inside the client region I need MOVE event.
				// DRAG can happen when mouse button down when inside the client region
				// as well as outside of the client region provided the button is still down.
				myApp.mouse.OnMouseMove(pt.x, pt.y);
			}
		}
		else
		{
			// WM_MOUSEMOVE carries some set of button or key down message in wParam.
			// Bit masking wParam to check if we are dragging something.
			if (wParam & MK_LBUTTON) // could also use IsLeftButtonPressed from Mouse class.
			{
				myApp.mouse.OnMouseDrag(pt.x, pt.y);
			}
			else
			{
				if (!myApp.mouse.IsOutsideClientRegion())
				{
					ReleaseCapture();
					myApp.mouse.OnMouseOutsideClientRegion(pt.x, pt.y);
				}
			}
		}
	}
		break;
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		myApp.mouse.OnMouseWheelDelta(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
	}
		break;
	case WM_CLOSE:
	{

	}
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(99);
	}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}