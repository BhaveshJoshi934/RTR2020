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

	fprintf(gpFile,"Code Start Here!!!\n\n");

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

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, TEXT("Trees!!!"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, X, Y, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

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
		//MessageBox(hwnd,"You Will see Trees","My Message",MB_OK);
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

    static GLfloat t1 = 15.0f;
    static GLfloat t2 = 40.0f;
    static GLfloat t3 = 70.0f;

    void Tree_one(void);
    void Tree_two(void);
    void Tree_three(void);
    void Tree_circle(void);
    void Tree_Throne(void);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(t1 >= -15.0f)
    {
        glTranslatef(t1,-2.0f,-15.0f);
    }

    t1 =  t1 - 0.05f;

	Tree_one();

/*---------------------------------------------------------------------------------------------------------------------*/

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(t3 >= -70.0f)
    {
        glTranslatef(t3,-6.0f,-30.0f);
    }

    t3 = t3 - 0.05f;

	Tree_two();
/*---------------------------------------------------------------------------------------------------------------------*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(t2 >= -40.0f)
    {
        glTranslatef(t2,-6.0f,-30.0f);
    }
    t2 = t2 - 0.05f;

	Tree_three();

/*---------------------------------------------------------------------------------------------------------------------*/

    static GLfloat thr1 = -1.0f;
    static GLfloat thr2 = 40.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(thr2 >= -40.0f)
    {
        glTranslatef(thr2,thr1 - 4.0f,-30.0f);
    }
	thr1 = thr1 + 0.05f;
	if(thr1 >= 3.5f)
    {
        thr1 = 0.0f;
    }
	thr2 = thr2 - 0.05f;

	Tree_Throne();

	SwapBuffers(ghdc);
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
