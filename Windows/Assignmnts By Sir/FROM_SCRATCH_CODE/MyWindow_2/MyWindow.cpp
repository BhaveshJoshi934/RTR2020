#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include "MyWindow.h"

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int iX,iY;
FILE *gpFile = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WNDCLASSEX) };
HWND ghwnd = NULL;
bool gbFullScreen =  false;

bool gbActiveWindow = false;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    void Initialize();
    void Display();

    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");

    iX = GetSystemMetrics(SM_CXSCREEN)/2 - WIN_WIDTH /2;
    iY = GetSystemMetrics(SM_CYSCREEN)/2 - WIN_HEIGHT /2;

    bool bDone = false;

    if(fopen_s(&gpFile,"Debug.txt","w") != 0)
    {
        printf("Can't Open File!!!\n\n");
        exit(0);
    }
    fprintf(gpFile,"Code Start Here!!!\n\n");

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbWndExtra = 0;
    wndclass.cbClsExtra = 0;
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
                          TEXT("Sharada!!!"),
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
    void Uninitialize();

/*
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;
    TCHAR str[] = TEXT("Bhavesh!!!");
*/
    switch(iMsg)
    {
    case WM_CREATE:
        fprintf(gpFile,"Window Create Success!!!\n\n");
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
            GetClientRect(ghwnd,&rc);
            hdc = BeginPaint(ghwnd,&ps);
            SetBkColor(hdc,RGB(0,0,0));
            SetTextColor(hdc,RGB(0,255,0));
            DrawText(hdc,str,-1,&rc,DT_VCENTER|DT_CENTER|DT_SINGLELINE);
            EndPaint(ghwnd,&ps);
            break;
*/
    case WM_DESTROY:
        Uninitialize();
        PostQuitMessage(0);
        break;
    }
    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

void ToggleFullScreen(void)
{
    MONITORINFO mi = { sizeof(MONITORINFO)};

    if(gbFullScreen == false)
    {
        dwStyle = GetWindowLong(ghwnd,GWL_STYLE);

        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            if(GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY),&mi))
            {
                SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd,HWND_TOP,mi.rcMonitor.left,mi.rcMonitor.top,mi.rcMonitor.right - mi.rcMonitor.left,mi.rcMonitor.bottom - mi.rcMonitor.top,SWP_NOZORDER | SWP_FRAMECHANGED);
            }
        }
        ShowCursor(FALSE);
        gbFullScreen = true;
    }

    else
    {
        SetWindowLong(ghwnd,GWL_STYLE,dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd,&wpPrev);
        SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED);
        ShowCursor(TRUE);
        gbFullScreen = false;
    }

}

void Initialize(void)
{
    void Resize(int,int);
    Resize(WIN_WIDTH, WIN_HEIGHT);
}

void Resize(int width,int height)
{
    if(height == 0)
        height = 1;
}

void Display()
{

}

void Uninitialize()
{
    if(gpFile)
    {
        fprintf(gpFile,"Code Ends Here!!!\n\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}
