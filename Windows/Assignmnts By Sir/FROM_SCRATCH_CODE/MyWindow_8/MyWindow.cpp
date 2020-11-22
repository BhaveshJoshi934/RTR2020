#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include "MyWindow.h"
#include<gl/gl.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment (lib,"OpenGL32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

FILE *gpFile = NULL;
DWORD dwStyle;
bool gbFullScreen = false;
HWND ghwnd = NULL;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
bool gbActiveWindow = false;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    void Initialize();
    void Display();

    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppAName[] = TEXT("MyApp");
    int iX,iY;
    bool bDone = false;

    if(fopen_s(&gpFile,"CHeck.txt","w") != 0)
    {
        printf("Can't Open!!!\n\n");
        exit(0);
    }
    fprintf(gpFile,"Progaram start here!!!\n\n");

    iX = GetSystemMetrics(SM_CXSCREEN) /  2 - WIN_WIDTH / 2 ;
    iY = GetSystemMetrics(SM_CYSCREEN) /  2 - WIN_HEIGHT / 2 ;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbWndExtra = 0;
    wndclass.cbClsExtra = 0;
    wndclass.lpfnWndProc  = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppAName;
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));

    RegisterClassEx(&wndclass);

    hwnd =    CreateWindowEx(WS_EX_APPWINDOW,
                             szAppAName,
                 TEXT("BHAVESH!!!"),
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
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

    while(bDone == false)
    {
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
                bDone = true;
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
    void ToggleFullScreen();
    void Resize(int,int);
    void UnInitialize();
/*
    PAINTSTRUCT ps;
    RECT rc;
    HDC hdc;
    TCHAR str[] = TEXT("BHAVESH!!!");
*/
    switch(iMsg)
    {

    case WM_CREATE:
        fprintf(gpFile,"Window Created Here!!!\n\n");
        break;

    case WM_SETFOCUS:
        gbActiveWindow = true;
        break;

    case WM_KILLFOCUS:
        gbActiveWindow = false;
        break;

/*
    case WM_PAINT:
            GetClientRect(ghwnd,&rc);
            hdc = BeginPaint(ghwnd,&ps);
            SetBkColor(hdc,RGB(0,0,0));
            SetTextColor(hdc,RGB(0,0,255));
            DrawText(hdc,str,-1,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
            EndPaint(ghwnd,&ps);
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

    case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

    case WM_SIZE:
        Resize(LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_DESTROY:
        UnInitialize();
        PostQuitMessage(0);
        break;
    }
    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

void ToggleFullScreen()
{
    MONITORINFO mi = { sizeof(MONITORINFO) };

    if(gbFullScreen == false)
    {
        dwStyle = GetWindowLong(ghwnd,GWL_STYLE);
        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            if(GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY),&mi))
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
        SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_FRAMECHANGED|SWP_NOMOVE);
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
    ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;

    iPixelFormatIndex = ChoosePixelFormat(ghdc,&pfd);

    if(iPixelFormatIndex == 0)
    {
        fprintf(gpFile,"Failed!!!\n");
        DestroyWindow(ghwnd);
    }

    if(SetPixelFormat(ghdc,iPixelFormatIndex,&pfd) == FALSE)
    {
        fprintf(gpFile,"Failed!!!\n");
        DestroyWindow(ghwnd);
    }

    ghrc = wglCreateContext(ghdc);

    if(ghrc == NULL)
    {
        fprintf(gpFile,"Failed!!!\n");
        DestroyWindow(ghwnd);
    }

    if(wglMakeCurrent(ghdc,ghrc) == FALSE)
    {
        fprintf(gpFile,"Failed!!!\n");
        DestroyWindow(ghwnd);
    }

    glClearColor(0.0f,0.0f,1.0f,0.0f);

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
        height = 1;

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
        ghrc = NULL;
    }

    if(ghdc)
    {
        ReleaseDC(ghwnd,ghdc);
        ghdc = NULL;
    }

    if(gpFile)
    {
        fprintf(gpFile,"Program Ends and Window terminate Here!!\n\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
