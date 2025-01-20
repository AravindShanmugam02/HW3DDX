#pragma once
#include "SlimWindows.h"
#include <d3d11.h>
// ^^^ Looks like including only this results in linker error.
// We have added header but where are the definitions for this header? It is not a part of std lib.
// Thus, we got to include the d3d11 lib. We can do that either through settings or like below.
#pragma comment(lib, "d3d11.lib")
// ^^^ When compiler hits this #pragma it will set the linker settings and include the d3d11.lib for us.
// By that, we don't have to set the lib in settings when we are using this file in different project.


class Graphics
{
public:
	Graphics(HWND hWnd);
	// Copying a graphics object or moving a graphics object doesn't really make sense.
	// Also we have destructor to release the resources. So,
	Graphics(const Graphics&) = delete;
	Graphics operator=(const Graphics&) = delete;
	~Graphics();

	void PresentFrame();
	void ClearRenderTargetView(float r, float g, float b, float a);

private:
	ID3D11Device* gfxDevice;
	ID3D11DeviceContext* gfxDeviceContext;
	IDXGISwapChain* gfxSwapChain;
	ID3D11RenderTargetView* gfxRenderTargetView;
};