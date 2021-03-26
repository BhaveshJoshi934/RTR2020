//Header Files

#include<Windows.h>
#include<stdio.h>
#include<math.h>
#include "MyWindow.h"
#include<gl/gl.h>

#include<gl/glu.h>                // Graphic Library Utility . h
#include<math.h>

//Macros

#define WIN_WIDTH   800
#define WIN_HEIGHT  600
#define PI          3.14159265359

#pragma comment(lib,"OpenGL32.lib")

#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"Winmm.lib")

//Callback Function

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global Variables

bool flag_silly_bird = 0;
bool flag_chase = 0;
bool flag_chirp = 0;

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

static GLfloat Up = 1.0f;
static GLfloat Down = -1.0f;
bool bUp = 0;
bool bDown = 0;

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

   // if(flag_silly_bird == 1)
    //{
        PlaySound(TEXT("Silly_Chicken.wav"),NULL,SND_ASYNC | SND_FILENAME);
   // }

    //if(flag_chase == 1)
    //{
    //   PlaySound(TEXT("Chase.wav"),NULL,SND_ASYNC | SND_FILENAME);
    //}

    if(flag_chirp == 1)
    {
        PlaySound(TEXT("Chirp.wav"),NULL,SND_ASYNC | SND_FILENAME);
    }

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

    case WM_LBUTTONDOWN:
        bUp = 1;
        Up = Up + 1.0f;
        break;

    case WM_RBUTTONDOWN:
        bDown = 1;
        Down = Down - 1.0f;
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
    static GLfloat delay_bdj = 50.0f;
    static GLfloat x = 4.0f;
    static GLfloat t1 = 25.0f;
    static GLfloat t2 = 60.0f;
    static GLfloat t3 = 90.0f;

    static GLfloat bird = 40.0f;  // 40

    void Loading(void);
    void Loading_Filled_one(void);
    void Loading_Filled_two(void);
    void Loading_Filled_three(void);
    void Loading_Filled_four(void);
    void Loading_Filled_five(void);
    void Loading_Null(void);
    void Loading_Filled_Null(void);

    void Bird_one(void);
    void Humanoid(void);
    void Tree_one(void);
    void Tree_two(void);
    void Tree_three(void);
    void Tree_circle(void);
    void Tree_Throne(void);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    Loading();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);

    Loading_Filled_one();

    delay_bdj = delay_bdj - 0.01f;


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    if(delay_bdj <= 40.0f)
    {
        Loading_Filled_two();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    if(delay_bdj <= 30.0f)
    {
        Loading_Filled_three();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    if(delay_bdj <= 20.0f)
    {
        Loading_Filled_four();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    if(delay_bdj <= 10.0f)
    {
        Loading_Filled_five();
         flag_silly_bird = 1;
    }

    if(delay_bdj <= 0.0f)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f,0.0f,-5.0f);

        Loading_Null();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f,0.0f,-5.0f);
        Loading_Filled_Null();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    if(Up >= 0.0f)
    {
        glTranslatef(-3.0f,(GLfloat)Up,-10.0f);
    }
    if(Down <= 0.0f)
    {
        glTranslatef(-3.0f,(GLfloat)Down,-0.0f);
    }
    Humanoid();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(bird >= -40.0f)
    {
        glTranslatef(bird,8.0f,-30.0f);
    }

    bird = bird - 0.05f;

    if(bird <= -40.0f)
    {
        bird = 41.0f;
    }

    Bird_one();
//---------------------------------------------------------------------------------------------------------------------
//CHRISMAS TREE

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(t1 >= -60.0f)
    {
        glTranslatef(t1,-2.0f,-15.0f);
    }

    t1 =  t1 - 0.05f;

	Tree_one();

	if(t1 <= -61.0f)
    {
        t1 = 25.0f;
    }

//---------------------------------------------------------------------------------------------------------------------
//PROPER TREE

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(t3 >= -40.0f)
    {
        glTranslatef(t3,-6.0f,-30.0f);
    }

    t3 = t3 - 0.05f;

	Tree_two();

	if(t3 <= -41.0f)
    {
        t3 = 90.0f;
    }
//---------------------------------------------------------------------------------------------------------------------

//CACTUS

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(t2 >= -50.0f)
    {
        glTranslatef(t2,-6.0f,-30.0f);
    }
    t2 = t2 - 0.05f;

	Tree_three();

	if(t2 <= -51.0f)
    {
        t2 = 60.0f;
    }

//---------------------------------------------------------------------------------------------------------------------

    static GLfloat thr1 = -1.0f;
    static GLfloat thr2 = 60.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(thr2 >= -50.0f)
    {
        glTranslatef(thr2,thr1 - 4.0f,-30.0f);
    }
	thr1 = thr1 + 0.05f;
	if(thr1 >= 4.5f)
    {
        thr1 = 0.0f;
    }
	thr2 = thr2 - 0.05f;

	if(thr2 <= -51.0f)
    {
        thr2 = 60.0f;
    }

	Tree_Throne();
    }

    SwapBuffers(ghdc);
}

void Loading()
{
   glColor3f(1.0f,1.0f,1.0f);
   glLineWidth(3.0f);

   glBegin(GL_LINES);

   glVertex3f(1.0f,0.125f,0.0f);
   glVertex3f(-1.0f,0.125f,0.0f);

   glEnd();

   glBegin(GL_LINES);

   glVertex3f(-1.0f,0.125f,0.0f);
   glVertex3f(-1.0f,-0.125f,0.0f);

   glEnd();

   glBegin(GL_LINES);

   glVertex3f(-1.0f,-0.125f,0.0f);
   glVertex3f(1.0f,-0.125f,0.0f);

   glEnd();

   glBegin(GL_LINES);

   glVertex3f(1.0f,-0.125f,0.0f);
   glVertex3f(1.0f,0.125f,0.0f);

   glEnd();
}

void Loading_Null()
{
   glColor3f(0.0f,0.0f,0.0f);

   glLineWidth(3.0f);

   glBegin(GL_LINES);

   glVertex3f(1.0f,0.125f,0.0f);
   glVertex3f(-1.0f,0.125f,0.0f);

   glEnd();

   glBegin(GL_LINES);

   glVertex3f(-1.0f,0.125f,0.0f);
   glVertex3f(-1.0f,-0.125f,0.0f);

   glEnd();

   glBegin(GL_LINES);

   glVertex3f(-1.0f,-0.125f,0.0f);
   glVertex3f(1.0f,-0.125f,0.0f);

   glEnd();

   glBegin(GL_LINES);

   glVertex3f(1.0f,-0.125f,0.0f);
   glVertex3f(1.0f,0.125f,0.0f);

   glEnd();
}

void Loading_Filled_Null()
{
    glColor3f(0.0f,0.0f,0.0f);

    glBegin(GL_QUADS);

    glVertex3f(1.0f,0.125f,0.0f);
    glVertex3f(-1.0f,0.125f,0.0f);
    glVertex3f(-1.0f,-0.125f,0.0f);
    glVertex3f(1.0f,-0.125f,0.0f);

    glEnd();

}

void Loading_Filled_one()
{
    glColor3f(0.0f,0.0f,1.0f);

    glBegin(GL_QUADS);

    glVertex3f(-0.88f,0.125f,0.0f);
    glVertex3f(-1.0f,0.125f,0.0f);
    glVertex3f(-1.0f,-0.125f,0.0f);
    glVertex3f(-.88f,-0.125f,0.0f);

    glEnd();
}

void Loading_Filled_two()
{
    glColor3f(0.0f,0.0f,1.0f);

    glBegin(GL_QUADS);

    glVertex3f(-0.58f,0.125f,0.0f);
    glVertex3f(-1.0f,0.125f,0.0f);
    glVertex3f(-1.0f,-0.125f,0.0f);
    glVertex3f(-0.58f,-0.125f,0.0f);

    glEnd();
}

void Loading_Filled_three()
{
    glColor3f(0.0f,0.0f,1.0f);

    glBegin(GL_QUADS);

    glVertex3f(0.0f,0.125f,0.0f);
    glVertex3f(-1.0f,0.125f,0.0f);
    glVertex3f(-1.0f,-0.125f,0.0f);
    glVertex3f(0.0f,-0.125f,0.0f);

    glEnd();
}

void Loading_Filled_four()
{
    glColor3f(0.0f,0.0f,1.0f);

    glBegin(GL_QUADS);

    glVertex3f(0.5f,0.125f,0.0f);
    glVertex3f(-1.0f,0.125f,0.0f);
    glVertex3f(-1.0f,-0.125f,0.0f);
    glVertex3f(0.5f,-0.125f,0.0f);

    glEnd();
}

void Loading_Filled_five()
{
    glColor3f(0.0f,0.0f,1.0f);

    glBegin(GL_QUADS);

    glVertex3f(1.0f,0.125f,0.0f);
    glVertex3f(-1.0f,0.125f,0.0f);
    glVertex3f(-1.0f,-0.125f,0.0f);
    glVertex3f(1.0f,-0.125f,0.0f);

    glEnd();
}

void Tree_one()
{
    glBegin(GL_QUADS);

    glColor3f(0.3f,0.0f,0.0f);
    glVertex3f(0.4f,-2.0f,0.0f);
    glVertex3f(-0.4f,-2.0f,0.0f);
    glVertex3f(-0.5f,-4.0f,0.0f);
    glVertex3f(0.5f,-4.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.2f,0.8f,0.0f);
    glVertex3f(3.0f,-1.0f,0.0f);
    glVertex3f(-3.0f,-1.0f,0.0f);
    glVertex3f(-4.0f,-2.0f,0.0f);
    glVertex3f(4.0f,-2.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.2f,0.7f,0.1f);
    glVertex3f(1.8f,0.0f,0.0f);
    glVertex3f(-1.8f,0.0f,0.0f);
    glVertex3f(-2.8f,-1.0f,0.0f);
    glVertex3f(2.8f,-1.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.2f,0.8f,0.0f);
    glVertex3f(0.9f,0.7f,0.0f);
    glVertex3f(-0.9f,0.7f,0.0f);
    glVertex3f(-1.6f,0.0f,0.0f);
    glVertex3f(1.6f,0.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.2f,0.7f,0.1f);
    glVertex3f(0.4f,1.2f,0.0f);
    glVertex3f(-0.4f,1.2f,0.0f);
    glVertex3f(-0.8f,0.7f,0.0f);
    glVertex3f(0.8f,0.7f,0.0f);

    glEnd();

    glBegin(GL_TRIANGLES);

    glColor3f(0.2f,0.8f,0.0f);
    glVertex3f(0.0f,1.7f,0.0f);
    glVertex3f(-0.35f,1.2f,0.0f);
    glVertex3f(0.35f,1.2f,0.0f);

    glEnd();
}

void Tree_two()
{

    glPushMatrix();
    glPushMatrix();

    glBegin(GL_QUADS);

    glColor3f(0.3f,0.0f,0.0f);

    glVertex3f(1.0f,-2.0f,0.0f);
    glVertex3f(-1.0f,-2.0f,0.0f);
    glVertex3f(-1.0f,-6.0f,0.0f);
    glVertex3f(1.0f,-6.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

    glVertex3f(4.0f,-1.0f,0.0f);
    glVertex3f(3.0f,0.0f,0.0f);
    glVertex3f(-0.5f,-2.0f,0.0f);
    glVertex3f(1.0f,-3.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

    glVertex3f(-4.0f,-1.0f,0.0f);
    glVertex3f(-3.0f,0.0f,0.0f);
    glVertex3f(0.5f,-2.0f,0.0f);
    glVertex3f(-1.0f,-3.0f,0.0f);

    glEnd();

    glPopMatrix();

    glTranslatef(4.0f,0.0f,0.0f);
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f,0.8f,0.0f);

    for(GLfloat angle = 0.0f ; angle <= 2*PI ; angle = angle + 0.001f)
    {
        glVertex3f(cos(angle)*2.5,sin(angle)*2.5,0.0f);
    }

    glEnd();

    glPopMatrix();

    glTranslatef(-8.0f,0.0f,0.0f);
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f,0.8f,0.0f);

    for(GLfloat angle = 0.0f ; angle <= 2*PI ; angle = angle + 0.001f)
    {
        glVertex3f(cos(angle)*2.5,sin(angle)*2.5,0.0f);
    }

    glEnd();

    glPopMatrix();

    glTranslatef(-0.5f,2.0f,0.0f);
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f,0.8f,0.0f);

    for(GLfloat angle = 0.0f ; angle <= 2*PI ; angle = angle + 0.001f)
    {
        glVertex3f(cos(angle)*2.5,sin(angle)*2.5,0.0f);
    }

    glEnd();

    glPopMatrix();

    glTranslatef(2.0f,2.0f,0.0f);
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f,0.8f,0.0f);

    for(GLfloat angle = 0.0f ; angle <= 2*PI ; angle = angle + 0.001f)
    {
        glVertex3f(cos(angle)*2.5,sin(angle)*2.5,0.0f);
    }

    glEnd();

    glPopMatrix();

    glTranslatef(1.5f,1.0f,0.0f);
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f,0.8f,0.0f);

    for(GLfloat angle = 0.0f ; angle <= 2*PI ; angle = angle + 0.001f)
    {
        glVertex3f(cos(angle)*2.5,sin(angle)*2.5,0.0f);
    }

    glEnd();

    glPopMatrix();

    glTranslatef(3.0f,-0.5f,0.0f);
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f,0.8f,0.0f);

    for(GLfloat angle = 0.0f ; angle <= 2*PI ; angle = angle + 0.001f)
    {
        glVertex3f(cos(angle)*2.5,sin(angle)*2.5,0.0f);
    }

    glEnd();

    glPopMatrix();

    glTranslatef(1.5f,-1.3f,0.0f);
    glPushMatrix();

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f,0.8f,0.0f);

    for(GLfloat angle = 0.0f ; angle <= 2*PI ; angle = angle + 0.001f)
    {
        glVertex3f(cos(angle)*2.5,sin(angle)*2.5,0.0f);
    }

    glEnd();

    glPopMatrix();

    glPopMatrix();

}

void Tree_circle()
{
    /*
    glColor3f(0.2f,0.8f,0.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    quadric = gluNewQuadric();
    gluSphere(quadric,2.5f,20,20);
    */

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f,0.8f,0.0f);

    for(GLfloat angle = 0.0f ; angle <= 2*PI ; angle = angle + 0.001f)
    {
        glVertex3f(cos(angle)*2.5,sin(angle)*2.5,0.0f);
    }

    glEnd();
}

void Tree_three()
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glPushMatrix();
    /*---------------------------------------------------------------------------------------------------------*/
    glPushMatrix();

    quadric = gluNewQuadric();
    glColor3f(0.2f,1.0f,0.2f);
    glRotatef(90,1.0f,0.0f,0.0f);
    gluCylinder(quadric,0.5f,0.5f,6.0f,20,20);

    glPopMatrix();

    glPushMatrix();

    quadric = gluNewQuadric();
    glColor3f(0.2f,1.0f,0.2f);
    gluSphere(quadric,0.5f,30,30);

    glPopMatrix();

    /*---------------------------------------------------------------------------------------------------------*/

    glPushMatrix();
    glTranslatef(2.0f,-1.0f,0.0f);

    quadric = gluNewQuadric();
    glColor3f(0.2f,1.0f,0.2f);
    glRotatef(90,1.0f,0.0f,0.0f);
    gluCylinder(quadric,0.5f,0.5f,3.0f,20,20);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.0f,-1.0f,0.0f);
    quadric = gluNewQuadric();
    glColor3f(0.2f,1.0f,0.2f);
    gluSphere(quadric,0.5f,30,30);

    glPopMatrix();

    /*---------------------------------------------------------------------------------------------------------*/

    glPushMatrix();
    glTranslatef(-2.0f,-2.0f,0.0f);

    quadric = gluNewQuadric();
    glColor3f(0.2f,1.0f,0.2f);
    glRotatef(90,1.0f,0.0f,0.0f);
    gluCylinder(quadric,0.5f,0.5f,2.0f,20,20);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.0f,-2.0f,0.0f);
    quadric = gluNewQuadric();
    glColor3f(0.2f,1.0f,0.2f);
    gluSphere(quadric,0.5f,30,30);

    glPopMatrix();

    /*---------------------------------------------------------------------------------------------------------*/

    glPushMatrix();
    glTranslatef(0.0f,-3.7f,0.0f);
    quadric = gluNewQuadric();
    glRotatef(90,0.0f,1.0f,0.0f);
    glColor3f(0.2f,1.0f,0.2f);
    gluCylinder(quadric,0.5f,0.5f,2.4f,20,20);
    glPopMatrix();

    /*---------------------------------------------------------------------------------------------------------*/

    glPushMatrix();
    glTranslatef(0.0f,-3.7f,0.0f);
    quadric = gluNewQuadric();
    glRotatef(-90,0.0f,1.0f,0.0f);
    glColor3f(0.2f,1.0f,0.2f);
    gluCylinder(quadric,0.5f,0.5f,2.4f,20,20);
    glPopMatrix();

    /*---------------------------------------------------------------------------------------------------------*/

    glPopMatrix();
}

void Tree_Throne()
{
    glBegin(GL_TRIANGLES);

    glColor3f(0.2f,1.0f,0.2f);
    glVertex3f(0.0f,0.2f,0.0f);
    glVertex3f(-0.2f,-0.2f,0.0f);
    glVertex3f(0.2f,-0.2f,0.0f);

    glEnd();
}
/*
void Bird_one()
{
    glPushMatrix();

    //-------------------------------------Body-----------------------------------------
/*
    glPushMatrix();

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,2.0f,30,30);

    glPopMatrix();
*/
/*
    glPushMatrix();
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_TRIANGLE_FAN);

    for(GLfloat angle = 0.0f ; angle <= 2 * PI ; angle = angle + 0.001f)
    {
        glVertex3f(2*sin(angle),2*cos(angle),0.0f);
    }
    glEnd();

    glPopMatrix();
    //-------------------------------------Sheput-----------------------------------------
/*
    glPushMatrix();
    glTranslatef(2.0f,0.0f,0.0f);
    glColor3f(0.9f,0.1f,0.9f);
    glRotatef(90,0.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.2f,0.5f,0.8f,20,20);
    glPopMatrix();
*/
/*
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.9f,0.1f,0.9f);
    glVertex3f(3.0f,0.5f,0.0f);
    glVertex3f(2.0f,0.25f,0.0f);
    glVertex3f(2.0f,-0.25f,0.0f);
    glVertex3f(3.0f,-0.5f,0.0f);
    glEnd();

    glPopMatrix();
    //-------------------------------------Beak-----------------------------------------


    glPushMatrix();

    glTranslatef(-2.0f,0.0f,0.0f);

    //glTranslatef(0.0f,0.0f,2.5f);

    glBegin(GL_TRIANGLES);
    glColor3f(0.9f,0.1f,0.9f);
    glVertex3f(0.0f,0.3f,0.0f);
    glVertex3f(-0.3f,0.1f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.9f,0.1f,0.9f);
    glVertex3f(0.0f,-0.3f,0.0f);
    glVertex3f(-0.3f,-0.1f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    //-------------------------------------Right_Leg-----------------------------------------

    glPushMatrix();

    glTranslatef(0.0f,-2.0f,0.0f);

    glRotatef(90,1.0f,0.0f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.05f,0.05f,0.5f,20,20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.05f,-2.5f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(-0.4f,-0.4f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.05f,-2.5f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.4f,-0.4f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.05f,-2.5f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,-0.5f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    //-------------------------------------Left_Leg-----------------------------------------

    glPushMatrix();

    glTranslatef(0.3f,-1.8f,0.0f);

    glRotatef(90,1.0f,0.0f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.05f,0.05f,0.5f,20,20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.35f,-2.3f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(-0.4f,-0.4f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.35f,-2.3f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.4f,-0.4f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.35f,-2.3f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,-0.5f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    //-------------------------------------Wing-----------------------------------------

    static GLfloat t = 1.0f;

    glPushMatrix();

    //glTranslatef(0.0f,0.0f,2.0f);

    glColor3f(0.7f,0.4f,0.1f);
    glRotatef(90,0.0f,0,t);
    t = t - 0.01f;
    if(t <= -1.0f)
    {
        t = 1.0f;
    }

    glBegin(GL_TRIANGLE_FAN);

        for(GLfloat i = 0.0f ; i <=  PI ; i = i + 0.001f)
        {
            glVertex3f(sin(i),cos(i),0.0f);
        }

    glEnd();

    glPopMatrix();

    //-------------------------------------EYE-----------------------------------------

    glPushMatrix();

    glTranslatef(-1.0f,1.0f,0.0f);

    glColor3f(1.0f,1.0f,1.0f);

    glBegin(GL_TRIANGLE_FAN);

    for(GLfloat j = 0.0f ; j <= 2* PI ; j = j + 0.001f)
    {
        glVertex3f(sin(j)*0.20,cos(j)*0.20,0.0f);
    }

    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(-1.1f,1.0f,0.0f);

    glColor3f(0.0f,0.0f,0.0f);

    glBegin(GL_TRIANGLE_FAN);

    for(GLfloat j = 0.0f ; j <= 2* PI ; j = j + 0.001f)
    {
        glVertex3f(sin(j)*0.10,cos(j)*0.10,0.0f);
    }

    glEnd();


    glPopMatrix();


    glPopMatrix();
}
*/
void Bird_one()
{
   // glScalef(0.3f,0.3f,0.3f);

    glPushMatrix();

    //-------------------------------------Body-----------------------------------------
/*
    glPushMatrix();

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,2.0f,30,30);

    glPopMatrix();
*/

    glPushMatrix();
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_TRIANGLE_FAN);

    for(GLfloat angle = 0.0f ; angle <= 2 * PI ; angle = angle + 0.001f)
    {
        glVertex3f(2*sin(angle),2*cos(angle),0.0f);
    }
    glEnd();

    glPopMatrix();
    //-------------------------------------Sheput-----------------------------------------
/*
    glPushMatrix();
    glTranslatef(2.0f,0.0f,0.0f);
    glColor3f(0.9f,0.1f,0.9f);
    glRotatef(90,0.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.2f,0.5f,0.8f,20,20);
    glPopMatrix();
*/
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.9f,0.1f,0.9f);
    glVertex3f(3.0f,0.5f,0.0f);
    glVertex3f(2.0f,0.25f,0.0f);
    glVertex3f(2.0f,-0.25f,0.0f);
    glVertex3f(3.0f,-0.5f,0.0f);
    glEnd();

    glPopMatrix();
    //-------------------------------------Beak-----------------------------------------


    glPushMatrix();

    glTranslatef(-2.0f,0.0f,0.0f);

    //glTranslatef(0.0f,0.0f,2.5f);

    glBegin(GL_TRIANGLES);
    glColor3f(0.9f,0.1f,0.9f);
    glVertex3f(0.0f,0.3f,0.0f);
    glVertex3f(-0.3f,0.1f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.9f,0.1f,0.9f);
    glVertex3f(0.0f,-0.3f,0.0f);
    glVertex3f(-0.3f,-0.1f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    //-------------------------------------Right_Leg-----------------------------------------

    glPushMatrix();

    glTranslatef(0.0f,-2.0f,0.0f);

    glRotatef(90,1.0f,0.0f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.05f,0.05f,0.5f,20,20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.05f,-2.5f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(-0.4f,-0.4f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.05f,-2.5f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.4f,-0.4f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.05f,-2.5f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,-0.5f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    //-------------------------------------Left_Leg-----------------------------------------

    glPushMatrix();

    glTranslatef(0.3f,-1.8f,0.0f);

    glRotatef(90,1.0f,0.0f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    quadric = gluNewQuadric();
    gluCylinder(quadric,0.05f,0.05f,0.5f,20,20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.35f,-2.3f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(-0.4f,-0.4f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.35f,-2.3f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.4f,-0.4f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.35f,-2.3f,0.0f);
    glColor3f(0.7f,0.4f,0.1f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,-0.5f,0.0f);
    glVertex3f(-0.1f,0.0f,0.0f);
    glEnd();

    glPopMatrix();

    //-------------------------------------Wing-----------------------------------------

    static GLfloat t = 1.0f;

    glPushMatrix();

    //glTranslatef(0.0f,0.0f,2.0f);

    glColor3f(0.7f,0.4f,0.1f);
    glRotatef(90,0.0f,0,t);
    t = t - 0.01f;
    if(t <= -1.0f)
    {
        t = 1.0f;
    }

    glBegin(GL_TRIANGLE_FAN);

        for(GLfloat i = 0.0f ; i <=  PI ; i = i + 0.001f)
        {
            glVertex3f(sin(i),cos(i),0.0f);
        }

    glEnd();

    glPopMatrix();

    //-------------------------------------EYE-----------------------------------------

    glPushMatrix();

    glTranslatef(-1.0f,1.0f,0.0f);

    glColor3f(1.0f,1.0f,1.0f);

    glBegin(GL_TRIANGLE_FAN);

    for(GLfloat j = 0.0f ; j <= 2* PI ; j = j + 0.001f)
    {
        glVertex3f(sin(j)*0.20,cos(j)*0.20,0.0f);
    }

    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(-1.1f,1.0f,0.0f);

    glColor3f(0.0f,0.0f,0.0f);

    glBegin(GL_TRIANGLE_FAN);

    for(GLfloat j = 0.0f ; j <= 2* PI ; j = j + 0.001f)
    {
        glVertex3f(sin(j)*0.10,cos(j)*0.10,0.0f);
    }

    glEnd();


    glPopMatrix();


    glPopMatrix();
}


void Humanoid()
{
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

   //----------------------------------------------------------------------------------------------------------

    glPushMatrix();

    glTranslatef(0.0f,0.05f,0.0f);
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);
    //quadric = gluNewQuadric();
    //gluSphere(quadric,0.2f,20,20);
    glBegin(GL_POLYGON);

    glVertex3f(0.5f,0.5f,0.0f);
    glVertex3f(0.0f,0.6f,0.0f);
    glVertex3f(-0.5f,0.5f,0.0f);
    glVertex3f(-0.2f,-0.1f,0.0f);
    glVertex3f(0.2f,-0.1f,0.0f);

    glEnd();
    glPopMatrix();
    glPopMatrix();

   //----------------------------------------------------------------------------------------------------------
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
	gluSphere(quadric,0.15f,20,20);
	gluCylinder(quadric,0.08f,0.08f,0.5f,10,10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack

	//right elbow
	glTranslatef(0.0f, -0.6f, 0.0f);
	glRotatef(Relbow, 1.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);

    glRotatef(90.0f,1.0f,0.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.1f,20,20);
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
	gluSphere(quadric,0.15f,20,20);
	gluCylinder(quadric,0.08f,0.08f,0.5f,10,10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack


	//right elbow
	glTranslatef(0.0f, -0.6f, 0.0f);
	glRotatef(Relbow, 1.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);

    glRotatef(90.0f,1.0f,0.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.1f,20,20);
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
    gluSphere(quadric,0.1f,20,20);
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
    gluSphere(quadric,0.1f,20,20);
    gluCylinder(quadric,0.08f,0.04f,0.4f,10,10);
    glPopMatrix();

    glPopMatrix();
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
