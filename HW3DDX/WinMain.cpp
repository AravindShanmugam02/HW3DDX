#include "MyApplication.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Timer* timer = new Timer();
	Application* app = new Application();
	app->CreateAWindow(*app, *timer);

	bool isApplicationRunning = true;
	while (isApplicationRunning)
	{
		if (!app->Run())
		{
			// (ashanmugam [TODO] To handle better)
			isApplicationRunning = false;
		}
	}

	// Program exit
}