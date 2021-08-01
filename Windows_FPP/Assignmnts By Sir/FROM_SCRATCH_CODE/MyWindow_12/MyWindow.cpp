//Header File

#include<Windows.h>
#include"MyWindow.h"
#include<stdio.h>
#include<stdlib.h>
#include<gl/gl.h>
#include<gl/glu.h>

//Macros

#define  WIN_WIDTH    800
#define  WIN_HEIGHT   600

//Global Variables

FILE *gpFile = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
bool gbFullScreen = false;
HWND ghwnd = NULL;

HDC ghdc = NULL;
HGLRC ghrc = NULL;

bool gbActiveWindow = NULL;

//#Pragma Comments

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")

//Call back Function Prototype

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpszCmdLine , int iCmdShow)
{
    //Function call Defination
    void Initialize(void);
    void Display(void);

    //Local Variables
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");
    bool bDone = false;
    int X = 0;
    int Y = 0;

    //Mid Screen Code

    X = GetSystemMetrics(SM_CXSCREEN) / 2 - WIN_WIDTH / 2 ;

    Y = GetSystemMetrics(SM_CYSCREEN) / 2 - WIN_HEIGHT / 2 ;

    //File Code

    if(fopen_s(&gpFile,"File.txt","w") != 0)
    {
        printf("Can't Open File!!!\n\n");
        exit(0);
    }
    fprintf(gpFile,"Code Started!!!\n\n");

    //WNDCLASSEX elments define
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_HREDRAW | CS_OWNDC;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));

    //RegisterClassEx

    RegisterClassEx(&wndclass);

    //CreateWindowEx

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                          szAppName,
                          TEXT("Bhavesh!!!"),
                          WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                          X,
                          Y,
                          WIN_WIDTH,
                          WIN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    //ShowWindow
    ShowWindow(hwnd,iCmdShow);

    //SetForeground && SetFocus
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    //Initialize call
    Initialize();

    //Game Loop
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
    //return value
    return((int)msg.wParam);
}

//Callback Defination
LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
    //Function Prototype
    void ToggleFullScreen(void);
    void Resize(int,int);
    void UnInitialize(void);

    //switch
    switch(iMsg)
    {
        case WM_CREATE:
            MessageBox(NULL,TEXT("Message"),TEXT("Window Created!!!"),MB_OK);
            break;

        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_ESCAPE:
                    DestroyWindow(hwnd);

                case 0X46:
                case 0X66:
                    ToggleFullScreen();
                    break;

                default:
                    break;
            }
            break;

        case WM_SIZE:
            Resize(HIWORD(lParam),LOWORD(lParam));
            break;

        case WM_SETFOCUS:
            gbActiveWindow = true;
            break;

        case WM_KILLFOCUS:
            gbActiveWindow = false;
            break;

        case WM_DESTROY:
            UnInitialize();
            PostQuitMessage(0);
            break;
    }

    //return type
    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}


//ToggleFullScreen
void ToggleFullScreen(void)
{
    MONITORINFO mi = {sizeof(MONITORINFO)};

    if(gbFullScreen == false)
    {
        dwStyle = GetWindowLong(ghwnd,GWL_STYLE);
        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            if(GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY),&mi) && GetWindowPlacement(ghwnd,&wpPrev))
            {
                SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~ WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd,
                            HWND_TOP,
                            mi.rcMonitor.left,
                            mi.rcMonitor.top,
                            mi.rcMonitor.right - mi.rcMonitor.left,
                            mi.rcMonitor.top - mi.rcMonitor.top,
                            SWP_NOZORDER|SWP_FRAMECHANGED);
                ShowCursor(FALSE);
                gbFullScreen = true;
            }
            else
            {
                GetWindowPlacement(ghwnd,&wpPrev);
                SetWindowLong(ghwnd,GWL_STYLE,dwStyle | WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOMOVE);
                ShowCursor(TRUE);
                gbFullScreen = false;
            }
        }
    }

}
//Initialization
void Initialize(void)
{
    void Resize(int,int);

    int iPixelFormat;
    PIXELFORMATDESCRIPTOR pfd;

    ghdc = GetDC(ghwnd);
    ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cBlueBits = 8;
    pfd.cGreenBits = 8;
    pfd.cAlphaBits = 8;

    iPixelFormat = ChoosePixelFormat(ghdc,&pfd);
    if(iPixelFormat == NULL)
    {
        fprintf(gpFile,"ChoosePixelWindow Failed !!!\n\n");
        DestroyWindow(ghwnd);
    }

    if(SetPixelFormat(ghdc,iPixelFormat,&pfd) == FALSE)
    {
        fprintf(gpFile,"SetPixelWindow Failed !!!\n\n");
        DestroyWindow(ghwnd);
    }

    ghrc = wglCreateContext(ghdc);

    if(ghrc == NULL)
    {
        fprintf(gpFile,"ghrc Failed !!!\n\n");
        DestroyWindow(ghwnd);
    }

    if(wglMakeCurrent(ghdc,ghrc) == NULL)
    {
        fprintf(gpFile,"wglMakeCurrent Failed !!!\n\n");
        DestroyWindow(ghwnd);
    }

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glClearColor(0.0f,0.0f,0.0f,0.0f);

    Resize(WIN_WIDTH,WIN_HEIGHT);
}

//Resize
void Resize(int height,int width)
{
    if(height == 0)
        height = 1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}

//Display
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SwapBuffers(ghdc);
}

//UnInitialize
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
        fclose(gpFile);
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


