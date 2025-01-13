#include "MyApplication.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Application* myApp = new Application();
	myApp->CreateAWindow(*myApp);

	bool isApplicationRunning = true;
	while (isApplicationRunning)
	{
		if (!myApp->Run())
		{
			// (ashanmugam [TODO] To handle better)
			isApplicationRunning = false;
		}
	}

	// Program exit
}