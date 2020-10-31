#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include "MyWindow.h"

#define WIN_WIDTH    800
#define WIN_HEIGHT   600

int iX,iY;
FILE *gpFile = NULL;

DWORD dwStyle;
bool gbFullScreen = false;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
HWND ghwnd = NULL;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    WNDCLASSEX wndclass;
    MSG msg;
    HWND hwnd;
    TCHAR szAppName[] = TEXT("MyApp");

    if(fopen_s(&gpFile,"Debug_Info.txt","w") != 0)
    {
        printf("Can't Open File!!!\n\n");
        exit(0);
    }
    fprintf(gpFile,"Your Code Start From Here!!!\n\n");

    iX = GetSystemMetrics(SM_CXSCREEN) / 2 - WIN_WIDTH  / 2;
    iY = GetSystemMetrics(SM_CYSCREEN) / 2 - WIN_HEIGHT / 2;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(szAppName,
                        TEXT("Dilip!!!"),
                        WS_OVERLAPPEDWINDOW,
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

    ghwnd = hwnd;

    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
    void ToggleFullScreen();

/*
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;
    TCHAR str[] = TEXT("Bhavesh");
*/
    switch(iMsg)
    {

    case WM_CREATE:
        fprintf(gpFile,"Window Created!!!\n\n");
        break;
/*
    case WM_LBUTTONDOWN:
        GetClientRect(hwnd,&rc);
        //hdc = BeginPaint(hwnd,&ps);
        hdc = GetDC(hwnd);
        SetBkColor(hdc,RGB(0,0,0));
        SetTextColor(hdc,RGB(0,0,255));
        DrawText(hdc,str,-1,&rc,DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        //EndPaint(hwnd,&ps);
        ReleaseDC(hwnd,hdc);
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
        PostQuitMessage(0);
        fprintf(gpFile,"Code Ends Here!!!\n\n");
        fclose(gpFile);
        gpFile = NULL;
        break;
    }
    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

void ToggleFullScreen(void)
{
    MONITORINFO mi = { sizeof(MONITORINFO) };

    if(gbFullScreen == false)
    {
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            if(GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY),&mi))
               {
                   SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW);
                   SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
               }
        }
        ShowCursor(FALSE);
        gbFullScreen = true;
    }
    else
    {
        SetWindowLong(ghwnd,GWL_STYLE,dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd, &wpPrev);
        SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOOWNERZORDER|SWP_NOMOVE|SWP_NOSIZE);
        ShowCursor(TRUE);
        gbFullScreen = false;
    }
}

/*
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
*/
