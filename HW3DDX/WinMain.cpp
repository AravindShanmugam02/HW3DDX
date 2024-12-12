#include "MyWindows.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	bool isProgramRunning = true;
	while (isProgramRunning)
	{
		if (MyWindows::myWindows.ProcessMessage() == 99
			|| MyWindows::myWindows.ProcessMessage() == -1
			|| MyWindows::myWindows.ProcessMessage() == 0)
		{
			// (ashanmugam [TODO] To handle better)
			isProgramRunning = false;
		}
	}

	// Program exit
}