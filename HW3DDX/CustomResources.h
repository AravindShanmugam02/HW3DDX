#pragma once
#include "SlimWindows.h"
#include "resource.h"

class CustomResources
{
public:
	/////////// Making it a singleton ///////////
	// Deleting copy constructor
	CustomResources(const CustomResources&) = delete;
	// Deleting overloaded assignment operator
	CustomResources& operator=(const CustomResources&) = delete;
	// Application class instance to access members
	static CustomResources myCustResource;
	/////////////////////////////////////////////
	
	// Default constructor
	CustomResources() {};
	// Default Destructor
	~CustomResources() {};

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	HICON smlIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); // Shiba Inu
	HICON bigIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); // Shiba Inu
	// or can change to different icon if we want
};