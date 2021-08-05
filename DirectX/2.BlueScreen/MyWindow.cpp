//Header Files

#include<Windows.h>
#include<stdio.h>
#include "MyWindow.h"
#include<d3d11.h>
#include<Math.h>

//Macros

#define WIN_WIDTH   800
#define WIN_HEIGHT  600

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

void D3D11Log(void);

//Callback Function

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global Variables

FILE* gpFile = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
bool gbFullScreen = false;
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
bool gbActiveWindow = false;

float gClearColor[4];
IDXGISwapChain *gpIDXGISwapChain = NULL;
ID3D11Device *gpID3D11Device = NULL;
ID3D11DeviceContext *gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL;

//WinMain

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//Function Declaration
	HRESULT Initialize(void);
	void Display(void);
	void UnInitialize(void);

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Direct3D11");
	bool bDone = false;
	int X,Y;

	if (fopen_s(&gpFile, "Log0.txt", "w") != 0)
	{
		fprintf(gpFile,"Can't Open \n\n");
		exit(0);
	}

	X = GetSystemMetrics(SM_CXSCREEN) / 2 - WIN_WIDTH / 2;
	Y = GetSystemMetrics(SM_CYSCREEN) / 2 - WIN_HEIGHT / 2;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	//Register Class

	RegisterClassEx(&wndclass);

	//CreateWindow

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, TEXT("Direct3D11 BlueScreen in PP : BHAVESH JOSHI!!!"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, X, Y, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

	ghwnd = hwnd;

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

    HRESULT hr;

    hr = Initialize();

	if(FAILED(hr))
    {
        fopen_s(&gpFile,"Log1.txt","a+");
        fprintf_s(gpFile,"Initialize() Failed!!!\n\n");
        fclose(gpFile);
        DestroyWindow(ghwnd);
        hwnd = NULL;
    }
    else
    {
        fopen_s(&gpFile,"Log1.txt","a+");
        fprintf_s(gpFile,"Initialize() Succeeded!!!\n\n");
        fclose(gpFile);
    }

	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == true)
			{
				Display();
			}
		}
	}
	UnInitialize();
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	void ToggleFullScreen();
	HRESULT Resize(int, int);
	void UnInitialize(void);

	HRESULT hr;

	switch (iMsg)
	{

	case WM_CREATE:
		fprintf_s(gpFile, "Window Created... \n\n");
		break;

	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_SIZE:
	    if(gpID3D11DeviceContext)
        {
            hr = Resize(LOWORD(lParam), HIWORD(lParam));
            if(FAILED(hr))
            {
                fopen_s(&gpFile,"Log2.txt","a+");
                fprintf_s(gpFile,"Resize() Failed!!!\n\n");
                fclose(gpFile);
                DestroyWindow(ghwnd);
                ghwnd = NULL;
            }
            else
            {
                fopen_s(&gpFile,"Log2.txt","a+");
                fprintf_s(gpFile,"Resize() Succeeded!!!\n\n");
                fclose(gpFile);
            }
        }
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		case 0X46:
		case 0X66:
			ToggleFullScreen();
			break;

		default:
			break;
		}
		break;

	case WM_DESTROY:
		fprintf_s(gpFile, "Destroyed!!!\n\n");
		UnInitialize();
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
	MONITORINFO mi = { sizeof(MONITORINFO) };

	if (gbFullScreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		gbFullScreen = true;
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		gbFullScreen = false;
	}
}

HRESULT Initialize(void)
{
    void UnInitialize();
    HRESULT Resize(int,int);

    D3D11Log();

    HRESULT hr;
    D3D_DRIVER_TYPE d3dDriverType;
    D3D_DRIVER_TYPE d3dDriverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE,
    };
    D3D_FEATURE_LEVEL d3dFeatureLevel_required = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL d3dFeatureLevel_aquired = D3D_FEATURE_LEVEL_10_0;
    UINT createDeviceFlags = 0;
    UINT numDriverTypes = 0;
    UINT numFeatureLevels = 1;

    numDriverTypes = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);

    DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
    ZeroMemory((void*)&dxgiSwapChainDesc,sizeof(DXGI_SWAP_CHAIN_DESC));
    dxgiSwapChainDesc.BufferCount = 1;
    dxgiSwapChainDesc.BufferDesc.Width = WIN_WIDTH;
    dxgiSwapChainDesc.BufferDesc.Height = WIN_HEIGHT;
    dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    dxgiSwapChainDesc.OutputWindow = ghwnd;
    dxgiSwapChainDesc.SampleDesc.Count = 1;
    dxgiSwapChainDesc.SampleDesc.Quality = 0;
    dxgiSwapChainDesc.Windowed = TRUE;

    for(UINT DriverTypeIndex = 0 ; DriverTypeIndex < numDriverTypes ; DriverTypeIndex++)
    {
        d3dDriverType = d3dDriverTypes[DriverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(NULL,
                                           d3dDriverType,
                                           NULL,
                                           createDeviceFlags,
                                           &d3dFeatureLevel_required,
                                           numFeatureLevels,
                                           D3D11_SDK_VERSION,
                                           &dxgiSwapChainDesc,
                                           &gpIDXGISwapChain,
                                           &gpID3D11Device,
                                           &d3dFeatureLevel_aquired,
                                           &gpID3D11DeviceContext
                                           );

        if(SUCCEEDED(hr))
            break;
    }
        if(FAILED(hr))
        {
                fopen_s(&gpFile,"Log3.txt","a+");
                fprintf_s(gpFile,"D3D11CreateDeviceAndSwapChain() Failed!!!\n\n");
                fclose(gpFile);
                return(hr);
        }
        else
        {
                fopen_s(&gpFile,"Log4.txt","a+");
                fprintf(gpFile,"D3D11CreateDeviceAndSwapChain() Succeeded!!!\n\n");
                fprintf(gpFile,"The Chosen Driver Is Of ");
                if(d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
                {
                    fprintf_s(gpFile,"HardWare type!!\n\n");
                }
                else if(d3dDriverType == D3D_DRIVER_TYPE_WARP)
                {
                    fprintf_s(gpFile,"Warp type!!\n\n");
                }
                else if(d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
                {
                    fprintf_s(gpFile,"Reference type!!\n\n");
                }
                else
                {
                    fprintf_s(gpFile,"Unknown type!!\n\n");
                }

                fprintf_s(gpFile,"The Supported Highest Feature Level Is ");
                if(d3dFeatureLevel_aquired == D3D_FEATURE_LEVEL_11_0)
                {
                    fprintf_s(gpFile,"11.0\n\n");
                }
                else if(d3dFeatureLevel_aquired == D3D_FEATURE_LEVEL_10_1)
                {
                    fprintf_s(gpFile,"10.1\n\n");
                }
                else if(d3dFeatureLevel_aquired == D3D_FEATURE_LEVEL_10_0)
                {
                    fprintf_s(gpFile,"10.0\n\n");
                }
                else
                {
                    fprintf_s(gpFile,"Unknown\n\n");
                }
                fclose(gpFile);
        }
        gClearColor[0] = 0.0f;
        gClearColor[1] = 0.0f;
        gClearColor[2] = 1.0f;
        gClearColor[3] = 1.0f;

        hr = Resize(WIN_WIDTH,WIN_HEIGHT);

        if(FAILED(hr))
        {
                fopen_s(&gpFile,"Log5.txt","a+");
                fprintf_s(gpFile,"Resize() Failed!!!\n\n");
                fclose(gpFile);
                return(hr);
        }
        else
        {
                fopen_s(&gpFile,"Log5.txt","a+");
                fprintf_s(gpFile,"Resize() Succeeded!!!\n\n");
                fclose(gpFile);
        }
    return(S_OK);
}

void D3D11Log(void)
{
    //local variable declarations
    IDXGIFactory *piDXGIFactory = NULL;   // Mi Itkya saglyanch pita ahe ki mala identitych nahiye...Pure abstract base class
                                          //pratyek interface iUnknown madhun inherit hoto
    IDXGIAdapter *piDXGIAdapter = NULL;

    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    HRESULT hr;
    char str[255];

    //code
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory),(void **)&piDXGIFactory);
    if(FAILED(hr))
    {
        printf("CreateDXGI() Failed!!!\n\n");
        goto cleanup;
    }

    if(piDXGIFactory->EnumAdapters(0,&piDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
    {
        printf("DXGIAdapter Can not be Found!!!\n\n");
        goto cleanup;
    }

    ZeroMemory((void*)&dxgiAdapterDesc,sizeof(DXGI_ADAPTER_DESC));
    hr = piDXGIAdapter->GetDesc(&dxgiAdapterDesc);

    WideCharToMultiByte(CP_ACP,0,dxgiAdapterDesc.Description,255,str,255,NULL,NULL); //code page:ANSI code page
    printf("Graphic Card Name = %s\n\n",str);
    //printf("Graphic Card VRAM = %I64d bytes\n\n",dxgiAdapterDesc.DedicatedVideoMemory);
    printf("VRAM in GB = %d GB\n\n",(int)(ceil(dxgiAdapterDesc.DedicatedVideoMemory/1024.0/1024.0/1024.0)));

    cleanup:
        if(piDXGIAdapter)
        {
            piDXGIAdapter->Release();
            piDXGIAdapter = NULL;
        }

        if(piDXGIFactory)
        {
            piDXGIFactory->Release();
            piDXGIFactory = NULL;
        }
}

HRESULT Resize(int width, int height)
{
    HRESULT hr = S_OK;

    if(gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    gpIDXGISwapChain->ResizeBuffers(1,width,height,DXGI_FORMAT_R8G8B8A8_UNORM,0);

    ID3D11Texture2D *pID3D11Texture2D_BackBuffer;
    gpIDXGISwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&pID3D11Texture2D_BackBuffer);

    hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_BackBuffer,NULL,&gpID3D11RenderTargetView);

    if(FAILED(hr))
    {
                fopen_s(&gpFile,"Log6.txt","a+");
                fprintf_s(gpFile,"ID3D11Device::CreateRenderTargetView() Failed!!!\n\n");
                fclose(gpFile);
                return(hr);
    }
    else
    {
                fopen_s(&gpFile,"Log6.txt","a+");
                fprintf_s(gpFile,"ID3D11Device::CreateRenderTargetView() Succeeded!!!\n\n");
                fclose(gpFile);
    }
    pID3D11Texture2D_BackBuffer->Release();
    pID3D11Texture2D_BackBuffer = NULL;

    gpID3D11DeviceContext->OMSetRenderTargets(1,&gpID3D11RenderTargetView,NULL);

    D3D11_VIEWPORT d3dViewPort;
    d3dViewPort.TopLeftX = 0;
    d3dViewPort.TopLeftY = 0;
    d3dViewPort.Width = (float)width;
    d3dViewPort.Height = (float)height;
    d3dViewPort.MinDepth = 0.0f;
    d3dViewPort.MaxDepth = 1.0f;
    gpID3D11DeviceContext->RSSetViewports(1,&d3dViewPort);

    return(hr);

}

void Display(void)
{
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView,gClearColor);

    gpIDXGISwapChain->Present(0,0);
}

void UnInitialize(void)
{
	//code

    if(gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    if(gpIDXGISwapChain)
    {
        gpIDXGISwapChain->Release();
        gpIDXGISwapChain = NULL;
    }

    if(gpID3D11DeviceContext)
    {
        gpID3D11DeviceContext->Release();
        gpID3D11DeviceContext = NULL;
    }

    if(gpID3D11Device)
    {
        gpID3D11Device->Release();
        gpID3D11Device = NULL;
    }

	if (gpFile)
	{
		fclose(gpFile);
		fprintf(gpFile,"Code Ended Here !!!\n\n");
		gpFile = NULL;
	}

}
