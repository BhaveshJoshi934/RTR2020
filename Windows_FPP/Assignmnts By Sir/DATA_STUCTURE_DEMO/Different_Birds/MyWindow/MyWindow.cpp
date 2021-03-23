//Header Files

#include<Windows.h>
#include<stdio.h>
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

	fprintf(gpFile,"Code Starts Here!!!\n\n");

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

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, TEXT("Birds"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, X, Y, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

	ghwnd = hwnd;

	Initialize();

    if(flag_Silly_Bird == 1)
    {
        PlaySound(TEXT("Silly_Chicken.wav"),NULL,SND_ASYNC | SND_FILENAME);
    }

    if(flag_chase == 1)
    {
        PlaySound(TEXT("Chase.wav"),NULL,SND_ASYNC | SND_FILENAME);
    }

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

	case WM_DESTROY:
		fprintf(gpFile, "Code Ends Here!!!\n\n");
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

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	//SetClearColor
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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


    static GLfloat x = 24.0f;
    void Bird_one(void);

    void Loading(void);
    void Loading_Filled_one(void);
    void Loading_Filled_two(void);
    void Loading_Filled_three(void);
    void Loading_Filled_four(void);
    void Loading_Filled_five(void);


	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(x >= -24.0f)
    {
        glTranslatef(x,8.0f,-30.0f);
    }

    x = x - 0.1f;

    if(x <= -25.0f)
    {
        x = 24.0f;
    }

    Bird_one();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    //Loading();


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);

    //Loading_Filled_one();

    delay_bdj = delay_bdj - 0.01f;


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    if(delay_bdj <= 40.0f)
    {
//        Loading_Filled_two();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    if(delay_bdj <= 30.0f)
    {
//        Loading_Filled_three();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    if(delay_bdj <= 20.0f)
    {
//        Loading_Filled_four();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    if(delay_bdj <= 10.0f)
    {
//        Loading_Filled_five();
    }

	SwapBuffers(ghdc);
}

void Loading()
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
