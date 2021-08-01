//HEADER FILES

#include<Windows.h>
#include"MyWindow.h"
#include<stdio.h>
#include<stdlib.h>
#include<gl/gl.h>
#include<gl/glu.h>

//MACROS

#define    WIN_WIDTH    800
#define    WIN_HEIGHT   600

//OPENGL COMMENT

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")

//GLOBAL VARIABLES

FILE* gpFile = NULL;

WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
DWORD dwStyle;
HWND ghwnd = NULL;
bool gbFullScreen = false;

bool gbActiveWindow = NULL;

HDC ghdc = NULL;
HGLRC ghrc = NULL;


//CALLBACK DECLARATION

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

//WinMain

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    void Initialize();
    void Display();

    //DEBUG FILE

    if(fopen_s(&gpFile,"File.txt","w") != 0)
    {
        printf("Can't Open !!!\n");
        exit(0);
    }
    fprintf(gpFile,"Window Code Start Here!!!\n\n");

    //LOCAL VARAIBLES

    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");
    int X = 0;
    int Y = 0;
    bool bDone = false;

    X = GetSystemMetrics(SM_CXSCREEN) / 2  -  WIN_WIDTH  /  2 ;

    Y = GetSystemMetrics(SM_CYSCREEN) / 2  -  WIN_HEIGHT  /  2 ;

    //DECLARATION OF STRUCT

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.hInstance = hInstance;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));

    //REGISTER IT

    RegisterClassEx(&wndclass);

    //CREATE WINDOW

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                        szAppName,
                        TEXT("BHAVESH!!!"),
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                        X,
                        Y,
                        WIN_WIDTH,
                        WIN_HEIGHT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    //SHOWWINDOW

    ShowWindow(hwnd,iCmdShow);

    //UPDATEWINDOW

    //UpdateWindow(hwnd);

    SetForegroundWindow(hwnd);

    SetFocus(hwnd);

    ghwnd = hwnd;

    Initialize();

/*
    //MSGLOOP

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

//CALLBACK DEFINATION

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    void ToggleFullScreen(void);
    void Resize(int,int);
    void UnInitialize();

    switch(iMsg)
    {
    case WM_CREATE:
        MessageBox(NULL,TEXT("Window Created Successfully!!!"),TEXT("Message"),MB_OK);
        break;

    case WM_SETFOCUS:
        gbActiveWindow = true;
        break;

    case WM_KILLFOCUS:
        gbActiveWindow = false;
        break;

    case WM_SIZE:
        Resize(HIWORD(lParam),LOWORD(lParam));
        break;

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
        PostQuitMessage(0);
        UnInitialize();
        break;
    }
    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

//TOGGLE FULL SCREEN

void ToggleFullScreen()
{
    MONITORINFO mi = {sizeof(MONITORINFO)};

    if(gbFullScreen == false)
    {
        dwStyle = GetWindowLong(ghwnd,GWL_STYLE);

        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            if(GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY),&mi))
            {
               SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW);
               SetWindowPos(ghwnd,HWND_TOP,mi.rcMonitor.left,mi.rcMonitor.top,mi.rcMonitor.right - mi.rcMonitor.left,mi.rcMonitor.bottom - mi.rcMonitor.top,SWP_FRAMECHANGED|SWP_NOZORDER);
            }
        }
        ShowCursor(FALSE);
        gbFullScreen = true;
    }
    else
    {
        SetWindowPlacement(ghwnd,&wpPrev);
        SetWindowLong(ghwnd,GWL_STYLE,dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED|SWP_NOOWNERZORDER|SWP_NOSIZE);
        ShowCursor(TRUE);
        gbFullScreen = false;
    }
}

//INITIALIZATION

void Initialize()
{
    void Resize(int,int);

    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormat;

    ghdc = GetDC(ghwnd);
    ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cRedBits = 8;
    pfd.cBlueBits = 8;
    pfd.cGreenBits = 8;
    pfd.cAlphaBits = 8;
    pfd.cColorBits = 32;

    iPixelFormat = ChoosePixelFormat(ghdc,&pfd);

    if(iPixelFormat == 0)
    {
        fprintf(gpFile,"ChoosePixelFormat Filed!!!\n\n");
        DestroyWindow(ghwnd);
    }

    if(SetPixelFormat(ghdc,iPixelFormat,&pfd) == FALSE)
    {
        fprintf(gpFile,"SetPixelFormat Filed!!!\n\n");
        DestroyWindow(ghwnd);
    }

    ghrc = wglCreateContext(ghdc);

    if(ghrc == NULL)
    {
        fprintf(gpFile,"ghrc Filed!!!\n\n");
        DestroyWindow(ghwnd);
    }

    if(wglMakeCurrent(ghdc,ghrc) == FALSE)
    {
        fprintf(gpFile,"wglMakeCurrent Filed!!!\n\n");
        DestroyWindow(ghwnd);
    }

    glShadeModel(GL_SMOOTH);
    glClearDepth(0.1f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glClearColor(0.0f,1.0f,1.0f,0.0f);

    Resize(WIN_WIDTH,WIN_HEIGHT);
}
//RESIZE

void Resize(int width,int height)
{
    if(height == 0)
        height = 1;

    glViewport(0,0,(GLsizei)width,(GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}

//DISPLAY

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //SwapBuffers(ghdc);
    glFlush();
}

//UNINITIALIZE

void UnInitialize(void)
{
    if(gbFullScreen == true)
    {
        SetWindowPlacement(ghwnd,&wpPrev);
        SetWindowLong(ghwnd,GWL_STYLE,dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED);
        ShowCursor(TRUE);
        gbFullScreen = false;
    }

    if(gpFile)
    {
        fprintf(gpFile,"FilE Closed Here !!\n\n");
        gpFile = NULL;
    }

    if(ghrc)
    {
		wglDeleteContext(ghrc);
		ghrc = NULL;
    }

    if(wglGetCurrentContext() == ghrc)
    {
        wglMakeCurrent(NULL,NULL);
    }

    if(ghdc)
    {
        ReleaseDC(ghwnd,ghdc);
        ghdc = NULL;
    }
}
