#include "MyWindows.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Application::Window* win1 = new Application::Window;
	/*Application::Window* win2 = new Application::Window;*/

	bool isProgramRunning = true;
	while (isProgramRunning)
	{
		if (Application::myApp.ProcessMessage(*win1) == 99
			|| Application::myApp.ProcessMessage(*win1) == -1
			|| Application::myApp.ProcessMessage(*win1) == 0)
		{
			// (ashanmugam [TODO] To handle better)
			isProgramRunning = false;
		}
	}

	// Program exit
}