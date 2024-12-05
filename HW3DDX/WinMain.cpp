#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// To create a window class
	WNDCLASSEX winClass = { 0 };
	
	// Give it a name, that can be used later.
	const auto winClassName = "HW3DDX";

	// Structure configuration part
	winClass.style = CS_OWNDC;
	winClass.lpfnWndProc = DefWindowProc;
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
	
	while (true);
	return 0;
}