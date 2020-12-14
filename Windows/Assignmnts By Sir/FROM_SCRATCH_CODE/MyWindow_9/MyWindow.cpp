#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"MyWindow.h"
#include<gl/gl.h>
#include<gl/glu.h>

#define WIN_WIDTH_bdj   800
#define WIN_HEIGHT_bdj  600

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")

FILE *gpFile_bdj = NULL;

DWORD dwStyle_bdj;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
bool gbFullScreen_bdj = false;
HWND ghwnd_bdj = NULL;

HDC ghdc = NULL;
HGLRC ghrc = NULL;

bool gbActiveWindow = false;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    void Initialize(void);
    void Display(void);

    WNDCLASSEX wndclass_bdj;
    HWND hwnd_bdj = NULL;
    MSG msg_bdj;
    TCHAR szAppName_bdj[] = TEXT("Bhavesh");
    bool bDone = false;

    if(fopen_s(&gpFile_bdj,"DeBuG.txt","w") != 0)
    {
        MessageBox(hwnd_bdj,TEXT("Error"),TEXT("Can't Open!!!!"),MB_OK);
        exit(0);
    }
    fprintf(gpFile_bdj,"Code Start HERE!!!\n\n");

    int iX_bdj = GetSystemMetrics(SM_CXSCREEN) /2 - WIN_WIDTH_bdj / 2;
    int iY_bdj = GetSystemMetrics(SM_CYSCREEN) /2  - WIN_HEIGHT_bdj / 2;

    wndclass_bdj.cbSize = sizeof(WNDCLASSEX);
    wndclass_bdj.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass_bdj.cbWndExtra = 0;
    wndclass_bdj.cbClsExtra = 0;
    wndclass_bdj.lpfnWndProc = WndProc;
    wndclass_bdj.hInstance = hInstance;
    wndclass_bdj.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass_bdj.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass_bdj.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass_bdj.lpszMenuName = NULL;
    wndclass_bdj.lpszClassName = szAppName_bdj;
    wndclass_bdj.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));

    RegisterClassEx(&wndclass_bdj);


    hwnd_bdj = CreateWindowEx(WS_EX_APPWINDOW,
                              szAppName_bdj,
                            TEXT("AJAY"),
                            WS_OVERLAPPEDWINDOW,
                            iX_bdj,
                            iY_bdj,
                            WIN_WIDTH_bdj,
                            WIN_HEIGHT_bdj,
                            NULL,
                            NULL,
                            hInstance,
                            NULL);

    ShowWindow(hwnd_bdj,iCmdShow);
    UpdateWindow(hwnd_bdj);

    ghwnd_bdj = hwnd_bdj;

    Initialize();

	while (bDone == false)
	{
		if (PeekMessage(&msg_bdj, NULL, 0, 0, PM_REMOVE))
		{
			if (msg_bdj.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg_bdj);
				DispatchMessage(&msg_bdj);
			}
		}
		else
		{
			if (gbActiveWindow == true)
			{
				//Here you Should Call Update Function For OpenGL Rendering.

				//Here you Should Call Display Function For OpenGL Rendering.
				Display();
			}
		}
	}
/*
    while(GetMessage(&msg_bdj,NULL,0,0))
    {
        TranslateMessage(&msg_bdj);
        DispatchMessage(&msg_bdj);
    }
*/
    return((int)msg_bdj.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd_bdj,UINT iMsg_bdj,WPARAM wParam_bdj,LPARAM lParam_bdj)
{
 void ToggleFullScreen();
	void Resize(int, int);
	void UnInitialize(void);

	switch (iMsg_bdj)
	{

	case WM_CREATE:
		fprintf(gpFile_bdj, "India is My Country.\n");
		//MessageBox(hwnd, TEXT("WM_CREATE Received!!"), TEXT("My Message"), MB_TOPMOST | MB_ICONINFORMATION | MB_OK);
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
		Resize(LOWORD(lParam_bdj), HIWORD(lParam_bdj));
		break;

	case WM_KEYDOWN:
		switch (wParam_bdj)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd_bdj);
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
		fprintf(gpFile_bdj, "Jai Hind");
		UnInitialize();
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd_bdj, iMsg_bdj, wParam_bdj, lParam_bdj));
}

void ToggleFullScreen()
{
    MONITORINFO mi = {sizeof(MONITORINFO)};

    if(gbFullScreen_bdj == false)
    {
        dwStyle_bdj = GetWindowLong(ghwnd_bdj,GWL_STYLE);
        if(dwStyle_bdj & WS_OVERLAPPEDWINDOW)
        {
            if(GetWindowPlacement(ghwnd_bdj,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd_bdj,MONITORINFOF_PRIMARY),&mi))
            {
                SetWindowLong(ghwnd_bdj,GWL_STYLE,dwStyle_bdj & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd_bdj,HWND_TOP,mi.rcMonitor.left,mi.rcMonitor.top,mi.rcMonitor.right - mi.rcMonitor.left,mi.rcMonitor.bottom - mi.rcMonitor.top,SWP_NOZORDER | SWP_FRAMECHANGED);
            }
        }
        ShowCursor(TRUE);
        gbFullScreen_bdj = true;
    }
    else
    {
        SetWindowLong(ghwnd_bdj,GWL_STYLE,dwStyle_bdj | WS_OVERLAPPEDWINDOW);
        GetWindowPlacement(ghwnd_bdj,&wpPrev);
        SetWindowPos(ghwnd_bdj,HWND_TOP,0,0,0,0,SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOOWNERZORDER|SWP_NOMOVE|SWP_NOSIZE);
        ShowCursor(TRUE);
        gbFullScreen_bdj = false;
    }
}

void Initialize(void)
{
    void Resize(int,int);

    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex;

    ghdc = GetDC(ghwnd_bdj);
    ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cBlueBits = 8;
    pfd.cGreenBits = 8;
    pfd.cAlphaBits = 8;

    iPixelFormatIndex = ChoosePixelFormat(ghdc,&pfd);

    if(iPixelFormatIndex == false)
    {
        fprintf(gpFile_bdj,"ChoosePixelFormat Failed!!!\n\n");
        DestroyWindow(ghwnd_bdj);
    }

    if(SetPixelFormat(ghdc,iPixelFormatIndex,&pfd) == FALSE)
    {
        fprintf(gpFile_bdj,"SetPixeFormat Failed!!!\n\n");
        DestroyWindow(ghwnd_bdj);
    }

    ghrc = wglCreateContext(ghdc);

    if(ghrc == FALSE)
    {
        fprintf(gpFile_bdj,"ghrc Failed!!!\n\n");
        DestroyWindow(ghwnd_bdj);
    }

    if(wglMakeCurrent(ghdc,ghrc) == FALSE)
    {
        fprintf(gpFile_bdj,"wglMakeCurrent Failed!!!\n\n");
        DestroyWindow(ghwnd_bdj);
    }

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glClearColor(0.0f,0.0f,0.0f,0.0f);

    Resize(WIN_WIDTH_bdj,WIN_HEIGHT_bdj);
}

void Resize(int height,int width)
{
    if(height == 0)
        height = 1;

    glViewport(0,0,(GLsizei)WIN_WIDTH_bdj,(GLsizei)WIN_HEIGHT_bdj);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)WIN_WIDTH_bdj/(GLfloat)WIN_HEIGHT_bdj,0.1f,100.0f);

}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SwapBuffers(ghdc);
}

void UnInitialize(void)
{
	//code
	if (gbFullScreen_bdj == true)
	{
		dwStyle_bdj = GetWindowLong(ghwnd_bdj, GWL_STYLE);
		SetWindowLong(ghwnd_bdj, GWL_STYLE, dwStyle_bdj | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd_bdj, &wpPrev);
		SetWindowPos(ghwnd_bdj, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc)
	{
		ReleaseDC(ghwnd_bdj, ghdc);
		ghdc = NULL;
	}
	if (gpFile_bdj)
	{
		fclose(gpFile_bdj);
		gpFile_bdj = NULL;
	}

}

