#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include "MyWindow.h"
#include<gl/gl.h>

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

#pragma comment(lib,"OpenGL32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int iX,iY;

FILE *gpFile = NULL;

DWORD dwStyle;
bool gbFullScreen = false;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
HWND ghwnd = NULL;

HDC ghdc   = NULL;
HGLRC ghrc = NULL;

bool gbActiveWindow = false;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    void Initialize();
    void Display();

    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");
    bool bDone = false;

    if(fopen_s(&gpFile,"Debug.txt","w") != 0)
    {
        printf("Can't Open File!!!\n\n");
        exit(0);
    }
    fprintf(gpFile,"Code Start here!!!\n\n");

    iX = GetSystemMetrics(SM_CXSCREEN) /2 - WIN_WIDTH / 2;

    iY = GetSystemMetrics(SM_CYSCREEN) /2 - WIN_HEIGHT / 2;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));

    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                          szAppName,
                        TEXT("Minal!!!"),
                        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                        iX,
                        iY,
                        WIN_WIDTH,
                        WIN_HEIGHT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    ShowWindow(hwnd,iCmdShow);
    //UpdateWindow(hwnd);

    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    ghwnd = hwnd;

    Initialize();
/*
    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
*/

    while(bDone == false)
    {
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                bDone = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if(gbActiveWindow == true)
            {
                Display();
            }
        }
    }
    return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
    void ToggleFullScreen(void);
    void Resize(int,int);
    void UnInitialize();
/*
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;
    TCHAR str[] = TEXT("Dhruv!!!");
*/
    switch(iMsg)
    {
    case WM_CREATE:
        fprintf(gpFile,"Window is Created !!!\n\n");
        break;

    case WM_SETFOCUS:
        gbActiveWindow = true;
        break;

    case WM_KILLFOCUS:
        gbActiveWindow = false;
        break;

    case WM_SIZE:
        Resize(LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
/*
    case WM_PAINT:
        GetClientRect(hwnd,&rc);
        hdc = BeginPaint(hwnd,&ps);
        SetBkColor(hdc,RGB(0,0,0));
        SetTextColor(hdc,RGB(0,255,0));
        DrawText(hdc,str,-1,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
        EndPaint(hwnd,&ps);
        break;
*/
    case WM_KEYDOWN:
        switch(wParam)
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
        UnInitialize();
        PostQuitMessage(0);
        break;
    }
    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

void ToggleFullScreen(void)
{
    MONITORINFO mi = {sizeof(MONITORINFO)};
    if(gbFullScreen == false)
    {
        dwStyle = GetWindowLong(ghwnd,GWL_STYLE);

        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            if(GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY), &mi))
            {
                SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd,HWND_TOP,mi.rcMonitor.left,mi.rcMonitor.top,mi.rcMonitor.right - mi.rcMonitor.left,mi.rcMonitor.bottom - mi.rcMonitor.top,SWP_NOZORDER|SWP_FRAMECHANGED);
            }
        }
        ShowCursor(FALSE);
        gbFullScreen = true;
    }

    else
    {
        SetWindowPlacement(ghwnd,&wpPrev);
        SetWindowLong(ghwnd,GWL_STYLE,dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOMOVE|SWP_FRAMECHANGED);

        ShowCursor(TRUE);
        gbFullScreen = false;
    }
}

void Initialize()
{
    void Resize(int,int);

    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex;

    ghdc = GetDC(ghwnd);
    ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));;

    pfd.nSize = { sizeof(PIXELFORMATDESCRIPTOR) };
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cBlueBits = 8;
    pfd.cGreenBits = 8;
    pfd.cAlphaBits = 8;

    iPixelFormatIndex = ChoosePixelFormat(ghdc,&pfd);

    if(iPixelFormatIndex == 0)
    {
        fprintf(gpFile,"Choose Failed!!\n\n");;
        DestroyWindow(ghwnd);
    }

    if(SetPixelFormat(ghdc,iPixelFormatIndex,&pfd) == FALSE)
    {
        fprintf(gpFile,"Set Failed!!\n\n");;
        DestroyWindow(ghwnd);
    }

    ghrc = wglCreateContext(ghdc);

    if(ghrc == NULL)
    {
        fprintf(gpFile,"ghrc Failed!!\n\n");;
        DestroyWindow(ghwnd);
    }

    if(wglMakeCurrent(ghdc,ghrc) == FALSE)
    {
        fprintf(gpFile,"WgMake Failed!!\n\n");;
        DestroyWindow(ghwnd);
    }

    glClearColor(1.0f,0.0f,0.0f,0.0f);

    Resize(WIN_WIDTH,WIN_HEIGHT);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();
}

void Resize(int width,int height)
{
    if(height == 0)
    {
        height = 1;
    }
    glViewport(0,0,(GLsizei)width,(GLsizei)height);
}

void UnInitialize()
{
    if(wglGetCurrentContext() == ghrc)
    {
        wglMakeCurrent(NULL,NULL);
    }

    if(ghrc)
    {
        wglDeleteContext(ghrc);
    }

    if(ghdc)
    {
        ReleaseDC(ghwnd,ghdc);
        ghdc = NULL;
    }

    if(gpFile)
    {
        fprintf(gpFile,"Code End Here!!!\n\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
