//Header File
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"MyWindow.h"
#include<GL/gl.h>
#include<GL/glu.h>

//Macros
#define    WIN_WIDTH   800
#define    WIN_HEIGHT  600

//Pragma comment
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")

//Gloabal Variables
FILE *gpFile = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
HWND ghwnd = NULL;
bool gbFullScreen = NULL;

HDC ghdc = NULL;
HGLRC ghrc = NULL;

bool gbActiveWindow = false;

//Callback prototyp

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

//WinMain

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    //Local variable
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");
    bool bDone = false;
    int X = NULL;
    int Y = NULL;

    //file open check
    if(fopen_s(&gpFile,"Bhavesh_file.txt","w") != 0)
    {
        printf("Error occured...\n\n");
        exit(1);
    }
    fprintf(gpFile,"Code Started Here!!!\n\n");

    //Centering of Window code
    X = GetSystemMetrics(SM_CXSCREEN) / 2 - WIN_WIDTH / 2;
    Y = GetSystemMetrics(SM_CYSCREEN) / 2 - WIN_HEIGHT / 2;

    //wndclass declaration

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbWndExtra = 0;
    wndclass.cbClsExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hCursor = LoadCursor(hInstance,IDC_ARROW);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = NULL;
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));

    //RegisterClassEx

    RegisterClassEx(&wndclass);

    //CraeteWindow

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                          szAppName,
                          TEXT("BHAVESH JOSHI"),
                          WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,
                          X,
                          Y,
                          WIN_WIDTH,
                          WIN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    //ShowWindow

    //SetForeground and SetFocus

    //Initialize call

    //Game Loop
}

//Callback Function

//ToggleFullScreen

//Initialization

//Resize

//Display

//UnInitialize

