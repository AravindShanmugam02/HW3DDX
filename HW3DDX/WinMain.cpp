#include <Windows.h>

// Custom window procedure to handle multiple messages and
// to define the behavior of the created window that is in context.
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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// To create a window class
	WNDCLASSEX winClass = { 0 };
	
	// Give it a name, that can be used later.
	const auto winClassName = "HW3DDX";

	// Structure configuration part
	winClass.style = CS_OWNDC;
	winClass.lpfnWndProc = CustWndProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = hInstance;
	winClass.hIcon = nullptr;
	winClass.hCursor = nullptr;
	winClass.hbrBackground = nullptr;
	winClass.lpszMenuName = nullptr;
	winClass.lpszClassName = winClassName;
	winClass.hIconSm = nullptr;
	winClass.cbSize = sizeof(winClass);

	// To register a window class
	RegisterClassEx(&winClass);

	// Give it a window name that gets displayed in the title bar
	const auto winTitleName = "3DHWACCDEMO";

	// To create a window
	HWND hWindow = CreateWindowEx(
		0, winClassName,
		winTitleName, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,
		200,200, 640, 480, nullptr, nullptr, hInstance, nullptr);

	// To show the window
	ShowWindow(hWindow, SW_SHOW);

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