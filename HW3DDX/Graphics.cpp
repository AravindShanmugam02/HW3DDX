#include "Graphics.h"

// Creates Device, Swapchain, Front/Back buffers, and rendering context
Graphics::Graphics(HWND hWnd)
{
	/*The DXGI_RATIONAL structure operates under the following rules:
		0/0 is legal and will be interpreted as 0/1.
		0/anything is interpreted as zero.
		If you are representing a whole number, the denominator should be 1.*/
	DXGI_RATIONAL refreshRate = {};
	refreshRate.Numerator = 0;
	refreshRate.Denominator = 0;

	DXGI_FORMAT displayFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_MODE_SCANLINE_ORDER rasterScanMethod = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	// Since our program is going to be windowed, no need for scaling now. Also, the width and height when mentioned as 0 will
	// match the window's width and height after it is created.
	DXGI_MODE_SCALING displayScaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// This describes the back buffer display mode.
	DXGI_MODE_DESC bufferDesc = {};
	// If you specify the width as zero when you call the IDXGIFactory::CreateSwapChain
	// method to create a swap chain, the runtime obtains the width from the output window
	// and assigns this width value to the swap-chain description.
	bufferDesc.Width = 0;
	
	// If you specify the height as zero when you call the IDXGIFactory::CreateSwapChain
	// method to create a swap chain, the runtime obtains the width from the output window
	// and assigns this width value to the swap-chain description.
	bufferDesc.Height = 0;

	bufferDesc.RefreshRate = refreshRate;
	bufferDesc.Format = displayFormat;
	bufferDesc.ScanlineOrdering = rasterScanMethod;
	bufferDesc.Scaling = displayScaling;

	// This describes multi-sampling parameters.
	// That is, anti-aliasing.
	// But we don't want it so setting the Count to 1 and Quality to 0 gives no anti-aliasing.
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	// This describes the surface usage and CPU access options for the back buffer.
	// The back buffer can be used for shader input or render-target output.
	DXGI_USAGE bufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Options for handling pixels in a display surface after calling IDXGISwapChain1::Present1.
	// DXGI_SWAP_EFFECT_DISCARD is basically the vanilla which gives best performance.
	DXGI_SWAP_EFFECT swapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Describes a swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = bufferUsage;
	
	// A value that describes the number of buffers in the swap chain.
	// When you call IDXGIFactory::CreateSwapChain to create a full-screen swap chain,
	// you typically include the front buffer in this value.
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = swapEffect;

	// This is basically the Options for swap-chain behavior. 0 is nothing.
	swapChainDesc.Flags = 0;


	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr, // Setting feature level to nullptr makes it to choose the most latest level that is available/compatible.
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&gfxSwapChain,
		&gfxDevice,
		nullptr,
		&gfxDeviceContext);

	// Gaining access to texture sub-resource in swap chain (back buffer)
	ID3D11Resource* backBuffer;
	
	// To get back buffer, we need to query swap chain and get it using type ID3D11Resource and id as 0. 0 is back buffer.
	gfxSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));

	// Creating Render Target View. Render target is represented by Back buffer.
	// So, we need to pointer to pass back buffer as ID3D11Resource.
	gfxDevice->CreateRenderTargetView(backBuffer, nullptr, &gfxRenderTargetView);

	// Releasing back buffer after created a RenderTargetView.
	backBuffer->Release();
}

Graphics::~Graphics()
{
	// We check it is not nulptr because there is no guarantee that this pointer to pointer (**) would have got a value.
	if (gfxRenderTargetView != nullptr)
	{
		gfxRenderTargetView->Release();
	}

	// We check it is not nulptr because there is no guarantee that this pointer to pointer (**) would have got a value.
	if (gfxDeviceContext != nullptr)
	{
		gfxDeviceContext->Release();
	}

	// We check it is not nulptr because there is no guarantee that this pointer to pointer (**) would have got a value.
	if (gfxSwapChain != nullptr)
	{
		gfxSwapChain->Release();
	}

	// We check it is not nulptr because there is no guarantee that this pointer to pointer (**) would have got a value.
	if (gfxDevice != nullptr)
	{
		gfxDevice->Release();
	}
}

void Graphics::PresentFrame()
{
	// We need to use swap chain to present frames.
	// The first parameter is sync interval. We want the presentation to sync after every 1 VBlank.
	// 1 defaults to syncing 60 frames.
	// There is more to know: https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-present
	gfxSwapChain->Present(1u, 0u);
}

void Graphics::ClearRenderTargetView(float r, float g, float b, float a)
{
	const float colour[] = { r,g,b,a };
	gfxDeviceContext->ClearRenderTargetView(gfxRenderTargetView, colour);
}