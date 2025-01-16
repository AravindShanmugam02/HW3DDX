#include "MyApplication.h"
#include "MyWindows.h"

Keyboard Application::keyboard;
Mouse Application::mouse;

Application::Application()
{
	window = nullptr;
	timer = nullptr;
}

bool Application::CreateAWindow(Application& _myApp, Timer& _myTimer)
{
	timer = &_myTimer;

	if (timer != nullptr)
	{
		window = new Window(*this, *timer);
	}

	return (window != nullptr);
}

bool Application::Run()
{
	const int retVal = ProcessMessage();
	if (retVal == 99)
	{
		return false;
	}

	return (RunAFrame() == true); // (ashanmugam [TO-DO] handle this better as what if there is an error or warning or softassert)
}

const int Application::ProcessMessage()
{
	// To make a message pump
	// 1. First, need a MSG
	MSG winMessage;

	// We use while so that the window stays on as long as we are quitting or it runs into error.
	// 2. Second, pass the MSG to PeekMessage()
	while (PeekMessage(&winMessage, nullptr, 0, 0, PM_REMOVE))
		// PeekMessage() returns 0 if no messages are there and 1 if there are messages.
		// Unlike GetMessage() it doesn't give anything about errors.
	{
		if (winMessage.message == WM_QUIT)
		{
			return winMessage.wParam;
		}

		// 3. Translate the MSG. (ashanmugam [TO-DO] To Find More?)
		TranslateMessage(&winMessage);
		// 4. Dispatch the MSG to Win32's window procedure for the window created.
		DispatchMessage(&winMessage);
	}

	return winMessage.wParam;
}

bool Application::RunAFrame()
{
	timer->UpdateTime();
	window->SetWindowTitle();
	return true;
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
		if (!(lParam & 0x40000000) || keyboard.IsAutoRepeatEnabled())
		{
			keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
	}
	break;
	// WM_KEYUP is for Non-System keys. When ALT is released or F10 is released.
	case WM_KEYUP:
		// WM_SYSKEYUP is for system keys like F10 or when any key is released while holding down ALT.
	case WM_SYSKEYUP:
	{
		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
	}
	break;
	case WM_CHAR:
	{
		keyboard.OnCharacter(static_cast<unsigned char>(wParam));
	}
	break;
	case WM_KILLFOCUS:
	{
		// When a window loses focus, we don't want the window to think some keystates are true. Hence clearing the keystates.
		keyboard.ClearKeyboardKeyStates();
	}
	break;
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMouseLeftPressed(pt.x, pt.y);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMouseRightPressed(pt.x, pt.y);
	}
	break;
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMouseLeftReleased(pt.x, pt.y);
	}
	break;
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMouseRightReleased(pt.x, pt.y);
	}
	break;
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		// This means the mouse pointer/cursor is inside the client region.
		if ((pt.x >= 0 && pt.x <= screenWidth) && (pt.y >= 0 && pt.y <= screenHeight))
		{
			if (!mouse.IsInsideClientRegion())
			{
				mouse.OnMouseInsideClientRegion(pt.x, pt.y);

				// Sets a window on which the mouse is over to capture mouse events.
				// It also captures mouse move event outside client region
				// when a button was down while inside client region and still down outside.
				SetCapture(hWnd);
			}

			// WM_MOUSEMOVE carries some set of button or key down message in wParam.
			// Bit masking wParam to check if we are dragging something.
			if (wParam & MK_LBUTTON) // could also use IsLeftButtonPressed from Mouse class.
			{
				mouse.OnMouseDrag(pt.x, pt.y);
			}
			else
			{
				// Unlike DRAG event, only if the mouse cursor / pointer is inside the client region I need MOVE event.
				// DRAG can happen when mouse button down when inside the client region
				// as well as outside of the client region provided the button is still down.
				mouse.OnMouseMove(pt.x, pt.y);
			}
		}
		else
		{
			// WM_MOUSEMOVE carries some set of button or key down message in wParam.
			// Bit masking wParam to check if we are dragging something.
			if (wParam & MK_LBUTTON) // could also use IsLeftButtonPressed from Mouse class.
			{
				mouse.OnMouseDrag(pt.x, pt.y);
			}
			else
			{
				if (!mouse.IsOutsideClientRegion())
				{
					ReleaseCapture();
					mouse.OnMouseOutsideClientRegion(pt.x, pt.y);
				}
			}
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMouseWheelDelta(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
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