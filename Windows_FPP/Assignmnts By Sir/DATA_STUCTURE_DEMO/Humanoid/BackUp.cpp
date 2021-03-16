//Header Files

#include<Windows.h>
#include<stdio.h>
#include<math.h>
#include "MyWindow.h"
#include<gl/gl.h>

#include<gl/glu.h>                // Graphic Library Utility . h

//Macros

#define WIN_WIDTH   800
#define WIN_HEIGHT  600
#pragma comment(lib,"OpenGL32.lib")

#pragma comment(lib,"glu32.lib")

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

int O = 0;
int Rsholder = 0;
int Relbow = 0;
int Rpalm = 0;
int Rfingure = 0;

int Lsholder = 0;
int Lelbow = 0;
int Lpalm = 0;
int Lfingure = 0;

int RsholderFeet = 0;
int RelbowFeet = 0;
int LsholderFeet = 0;
int LelbowFeet = 0;

int Neck = 0;


GLfloat x,y = 0;
GLfloat z = 5.0f;


GLUquadric *quadric = NULL;
//WinMain

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//Function Declaration
	void Initialize(void);
	void Display(void);

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");
	bool bDone = false;
	int X, Y;

	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Can't Create file"), TEXT("Error"), MB_OK);
		exit(0);
	}

	fprintf(gpFile,"COde Start here!!!\n\n");

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

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, TEXT("Humanoid!!"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, X, Y, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

	ghwnd = hwnd;

	Initialize();

	ShowWindow(hwnd, iCmdShow);

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);


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
				//Here you Should Call Update Function For OpenGL Rendering.

				//Here you Should Call Display Function For OpenGL Rendering.
				Display();
			}
		}
	}
	/*
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	*/
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	void ToggleFullScreen();
	void Resize(int, int);
	void UnInitialize(void);

	switch (iMsg)
	{

	case WM_CREATE:
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
		Resize(LOWORD(lParam), HIWORD(lParam));
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

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_CHAR:
        switch(wParam)
        {
        /*
        case 'S':
            Rsholder = (Rsholder + 3) % 45;
            Lsholder = (Lsholder - 3) % 45;
            RsholderFeet = (RsholderFeet + 3) % 45;
            LsholderFeet = (LsholderFeet - 3) % 45;
            break;

        case 's':
            Rsholder = (Rsholder - 3) % 45;
            Lsholder = (Lsholder + 3) % 45;
            RsholderFeet = (RsholderFeet - 3) % 45;
            LsholderFeet = (LsholderFeet + 3) % 45;
            break;

        case 'E':
            Relbow = (Relbow + 3) % 360;
            Lelbow = (Lelbow - 3) % 360;
            RelbowFeet = (RelbowFeet + 3) % 45;
            LelbowFeet = (LelbowFeet - 3) % 45;
            break;

        case 'e':
            Relbow = (Relbow - 3) % 360;
            Lelbow = (Lelbow + 3) % 360;
            RelbowFeet = (RelbowFeet - 3) % 45;
            LelbowFeet = (LelbowFeet + 3) % 45;
            break;
            */

        case 'S':
            Rsholder = (Rsholder + 3) % 360;
            Lsholder = (Lsholder - 3) % 360;
            RsholderFeet = (RsholderFeet + 3) % 25;
            LsholderFeet = (LsholderFeet - 3) % 25;
            break;

        case 's':
            Rsholder = (Rsholder - 3) % 360;
            Lsholder = (Lsholder + 3) % 360;
            RsholderFeet = (RsholderFeet - 3) % 25;
            LsholderFeet = (LsholderFeet + 3) % 25;
            break;

        case 'O':
            O = (O + 3) % 360;
            break;

        case 'o':
            O = (O - 3) % 360;
            break;

        case 'E':
            Relbow = (Relbow + 3) % 360;
            Lelbow = (Lelbow - 3) % 360;
            RelbowFeet = (RelbowFeet + 3) % 25;
            LelbowFeet = (LelbowFeet - 3) % 25;
            break;

        case 'e':
            Relbow = (Relbow - 3) % 360;
            Lelbow = (Lelbow + 3) % 360;
            RelbowFeet = (RelbowFeet - 3) % 25;
            LelbowFeet = (LelbowFeet + 3) % 25;
            break;

        case 'P':
            Rpalm = (Rpalm + 3) % 360;
            Lpalm = (Lpalm - 3) % 360;
            break;

        case 'p':
            Rpalm = (Rpalm - 3) % 360;
            Lpalm = (Lpalm + 3) % 360;
            break;

        case 'M':
            Rfingure = (Rfingure + 3) % 360;
            Lfingure = (Lfingure - 3) % 360;
            break;

        case 'm':
            Rfingure = (Rfingure - 3) % 360;
            Lfingure = (Lfingure + 3) % 360;
            break;

        case 'N':
            Neck = (Neck - 3) % 35;
            break;

        case 'n':
            Neck = (Neck + 3) % 35;
            break;

        case 'A':
                x = x - 0.1f;
                if(x <= -12.0f)
                {
                    x = -12.0f;
                }
                break;

            case 'a':
                x = x + 0.1f;
                if(x >= 12.0f)
                {
                    x = 12.0f;
                }
                break;

            case 'B':
                y = y - 0.1f;
                if(y <= -12.0f)
                {
                    y = -12.0f;
                }
                break;

            case 'b':
                y = y + 0.1f;
                if(y >= 12.0f)
                {
                    y = 12.0f;
                }
                break;

            case 'C':
                z = z - 0.1f;
                if(z <= -12.0f)
                {
                    z = -12.0f;
                }
                break;

            case 'c':
                z = z + 0.1f;
                if(z >= 12.0f)
                {
                    z = 12.0f;
                }
                break;
        }
        break;

	case WM_DESTROY:
		fprintf(gpFile, "Code Ends Here!!\n\n");
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

void Initialize(void)
{
	//Function Declaration
	void Resize(int, int);

	//Variable Declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	ghdc = GetDC(ghwnd);

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "ChoosePixelFormat() Failed!!!\n");
		DestroyWindow(ghwnd);
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex,&pfd) == FALSE)
	{
		fprintf(gpFile, "SetPixelFormat() Failed!!!\n");
		DestroyWindow(ghwnd);
	}

	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL)
	{
		fprintf(gpFile, "wglCreateContext() Failed!!!\n");
		DestroyWindow(ghwnd);
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent() Failed!!!\n");
		DestroyWindow(ghwnd);
	}

	//SetClearColor
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	//WarmUp Resize code
	Resize(WIN_WIDTH, WIN_HEIGHT);
}

void Resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    glRotatef((GLfloat)O,0.0f,1.0f,0.0f);

    //BODY

    glPushMatrix();

    glRotatef(90.0f,1.0f,0.0f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.3f,0.25f,1.0f,10,10);

    glPushMatrix();

    glPopMatrix();
    glPopMatrix();

    //RIGHT HAND
    glPushMatrix();

    glTranslatef(0.0f,-0.1f,0.0f);
	glRotatef(Rsholder, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.4f, 0.0f, 0.0f);
	glPushMatrix();
	// push tr-trt (body + hand) to stack

	glColor3f(1.0f, 1.0f, 0.0f);

	glRotatef(90.0f,1.0f,0.0f,0.0f);
	quadric = gluNewQuadric();
	gluCylinder(quadric,0.08f,0.08f,0.5f,10,10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack

	//right elbow
	glTranslatef(0.0f, -0.6f, 0.0f);
	glRotatef(Relbow, 1.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);

    glRotatef(90.0f,1.0f,0.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.08f,0.04f,0.5f,10,10);
	glPopMatrix();
	// get tr-trt (body + HAND) from stack

    glPopMatrix();

    glPushMatrix();
	//right hand
    glTranslatef(0.0f,-0.1f,0.0f);
	glRotatef(Lsholder, 1.0f, 0.0f, 0.0f);
	glTranslatef(-0.4f, 0.0f, 0.0f);
	glPushMatrix();
	// push tr-trt (body + hand) to stack

	glColor3f(1.0f, 1.0f, 0.0f);

	glRotatef(90.0f,1.0f,0.0f,0.0f);
	quadric = gluNewQuadric();
	gluCylinder(quadric,0.08f,0.08f,0.5f,10,10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack


	//right elbow
	glTranslatef(0.0f, -0.6f, 0.0f);
	glRotatef(Relbow, 1.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);

    glRotatef(90.0f,1.0f,0.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.08f,0.04f,0.5f,10,10);
	glPopMatrix();
	// get tr-trt (body + HAND) from stack

	glPopMatrix();

    glPushMatrix();

	glRotatef((GLfloat)RsholderFeet,1.0f,0.0f,0.0f);
    glTranslatef(0.1f,-1.0f,0.0f);
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);
    glRotatef(90,1.0f,0.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.08f,0.08f,0.4f,10,10);
    glPopMatrix();

    glRotatef((GLfloat)RelbowFeet,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-0.5f,0.0f);
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);
    glRotatef(90,1.0f,0.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.08f,0.04f,0.4f,10,10);
    glPopMatrix();


    glPopMatrix();

    glPushMatrix();

	glRotatef((GLfloat)LsholderFeet,1.0f,0.0f,0.0f);
    glTranslatef(-0.1f,-1.0f,0.0f);
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);
    glRotatef(90,1.0f,0.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.08f,0.08f,0.4f,10,10);
    glPopMatrix();

    glRotatef((GLfloat)LelbowFeet,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-0.5f,0.0f);
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);
    glRotatef(90,1.0f,0.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.08f,0.04f,0.4f,10,10);
    glPopMatrix();

    glPopMatrix();

    SwapBuffers(ghdc);
}

void UnInitialize(void)
{
	//code
	if (gbFullScreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
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
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	if (gpFile)
	{
		fclose(gpFile);
		gpFile = NULL;
	}

}
