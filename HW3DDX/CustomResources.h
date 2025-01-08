#pragma once
#include "SlimWindows.h"
#include "resource.h"

/////////////////////////////////
// IDI_ICON1 is Shiba Inu
// IDI_ICON2 is Confused Dog
/////////////////////////////////

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

	// To load custom icons, we have got to pass in non-null hInstance of the exe.
	// To convert the macro Int to Resource we can use MAKEINTRESOURCE. Also we can manually convert it to LPCSTR. Both works.
	HICON smlIcon = LoadIcon(hInstance, reinterpret_cast<LPCSTR>(IDI_ICON1)); // MAKEINTRESOURCE

	// To load custom icons, we have got to pass in non-null hInstance of the exe.
	// To convert the macro Int to Resource we can use MAKEINTRESOURCE. Also we can manually convert it to LPCSTR. Both works.
	HICON bigIcon = LoadIcon(hInstance, reinterpret_cast<LPCSTR>(IDI_ICON2)); // MAKEINTRESOURCE
};