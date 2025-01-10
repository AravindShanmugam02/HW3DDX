#include "MyApplication.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Application* myApp = new Application();
	myApp->CreateAWindow(*myApp);

	bool isProgramRunning = true;
	while (isProgramRunning)
	{
		if (myApp->ProcessMessage() == 99
			|| myApp->ProcessMessage() == -1
			|| myApp->ProcessMessage() == 0)
		{
			// (ashanmugam [TODO] To handle better)
			isProgramRunning = false;
		}
	}

	// Program exit
}