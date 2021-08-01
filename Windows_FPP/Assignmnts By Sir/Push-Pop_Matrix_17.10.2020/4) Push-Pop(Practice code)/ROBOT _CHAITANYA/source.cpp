#include <Windows.h>
#include "Mywindow.h"
#include <stdio.h>
#include <gl/GL.h>
#include <gl/GLU.h>


// global callback function
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#pragma comment(lib, "OpenGL32.lib")//link directive
#pragma comment(lib, "glu32.lib")


//global variables
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
bool gbFullScreen = false;
HWND ghwnd = NULL;
bool gbActiveWindow = false;
FILE* gpFile = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
int sholder = 0;
int right_elbow = 0;
int overall_rotation = 0;
int right_sholder = 0;
int left_sholder = 0;
int left_elbow = 0;


GLUquadric* quadric = NULL;


//function declaration

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	void initialize(void);
	void display(void);

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");
	bool bDone = false;

	//int width = 800;
	//int height = 600;
	int X, Y;
	X = GetSystemMetrics(SM_CXSCREEN) / 2 - WIN_WIDTH / 2;
	Y = GetSystemMetrics(SM_CYSCREEN) / 2 - WIN_HEIGHT / 2;

	if (fopen_s(&gpFile, "CHS Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Can't Create file"), TEXT("Error"), MB_OK);
		exit(0);
	}

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
	RegisterClassEx(&wndclass);
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("My Application: Chaitanya Saraf : Humanoid"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		100,
		100,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ghwnd = hwnd;
	initialize();
	ShowWindow(hwnd, iCmdShow);
	//UpdateWindow(hwnd);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	// game loop
	while (bDone == false)
	{
		//GetMessage(&msg, NULL, 0, 0)
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
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
			if (gbActiveWindow == true)
			{
				// Hear you should call update function for your OpenGl rendering

				// hear you should call display function for OpenGl rendering
				display();
			}
		}
	}
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	void ToggleFullScreen();
	void resize(int, int);
	void uninitialize(void);

	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;
	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;
	case WM_ERASEBKGND:
		return(0);
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
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
	case WM_CHAR:
		switch (wParam)
		{
		case 'S':
			right_sholder = (right_sholder + 3) % 360;
			break;

		case 's':
			right_sholder = (right_sholder - 3) % 360;
			break;

		case 'E':
			right_elbow = (right_elbow + 3) % 360;
			break;

		case 'e':
			right_elbow = (right_elbow - 3) % 360;
			break;

		case 'r':
			overall_rotation = (overall_rotation - 3) % 360;
			break;
		//case 'r':
		//	overall_rotation = (overall_rotation - 3) % 360;
		case 'l':
			left_sholder = (left_sholder - 3) % 360;
			break;
		case 'L':
			left_sholder = (left_sholder + 3) % 360;
			break;

		case 'I':
			left_elbow = (left_elbow - 3) % 360;
			break;
		case 'i':
			left_elbow = (left_elbow - 3) % 360;
			break;
		default:
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		uninitialize();
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

void initialize()
{
	//fuction declaration
	void resize(int, int);


	//local variable declarations
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
		fprintf(gpFile, "Choose pixel format function failed");
		//uninitialize();
		DestroyWindow(ghwnd);
	}


	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "Set pixel format function failed");
		//uninitialize();
		DestroyWindow(ghwnd);
	}

	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL)
	{
		fprintf(gpFile, "Create context failed");
		DestroyWindow(ghwnd);
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "Make current function failed");
	}

	//set clear color
	//first OpenGl function
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//fuction call
	resize(WIN_HEIGHT, WIN_WIDTH);// warmup resize
}

void resize(int width, int height)
{
	//code
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, GLsizei(width), GLsizei(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
	//initial code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//body

	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(overall_rotation, 1.0f, 0.0f, 0.0f);

	glPushMatrix();
	//push tr to stack = and same available on current matrix


	glRotatef(90, 0.5f, 0.0f, 0.0f);
	glColor3f(0.5f, 0.35f, 0.05f);
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.4f, 0.2f, 1.0f, 10, 10);
	glPushMatrix();
	//push trr to stack + current have trr


	glPopMatrix();
	// get trr stack in current matrix

	glPopMatrix();
	// get tr stack in current matrix

	glPushMatrix();
	// push tr stack in current matrix

	//neck

	glTranslatef(0.0f, 0.1f, 0.0f);
	glRotatef(90, 0.5f, 0.0f, 0.0f);

	glColor3f(0.5f, 0.35f, 0.05f);
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.1f, 0.3f, 0.2f, 10, 10);
	glPopMatrix();
	// get tr (body) from stack

	glPushMatrix();
	// put tr (body) to stack


	//head
	glTranslatef(0.0f, 0.3f, 0.0f);
	glColor3f(0.5f, 0.35f, 0.05f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();
	// get tr (body) from stack

	glPushMatrix();
	// put tr (body) to stack


	//right hand
	glTranslatef(0.3f, -0.1f, 0.0f);
	glRotatef(right_sholder, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.3f, 0.0f, 0.0f);
	glPushMatrix();
	// push tr-trt (body + hand) to stack

	glColor3f(0.5f, 0.35f, 0.05f);
	glScalef(1.5f, 0.5f, 0.5f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack


	//right elbow
	glTranslatef(0.3f, 0.0f, 0.0f);
	glRotatef(right_elbow, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.3f, 0.0f, 0.0f);

	glColor3f(0.5f, 0.35f, 0.05f);
	glScalef(1.5f, 0.5f, 0.5f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack

	glPopMatrix();
	// get tr (body) from stack

	glPushMatrix();
	// push tr (body) to stack


	//left hand
	glTranslatef(-0.3f, -0.1f, 0.0f);
	glRotatef(left_sholder, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.3f, 0.0f, 0.0f);
	glPushMatrix();
	// push tr-trt (body + hand) to stack

	glColor3f(0.5f, 0.35f, 0.05f);
	glScalef(1.5f, 0.5f, 0.5f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack


	//left elbow
	glTranslatef(-0.3f, 0.0f, 0.0f);
	glRotatef(left_elbow, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.3f, 0.0f, 0.0f);

	glColor3f(0.5f, 0.35f, 0.05f);
	glScalef(1.5f, 0.5f, 0.5f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();
	// get tr (body) from stack


	//left leg start
	glTranslatef(-0.1f, -0.9f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.3f, 0.0f, 0.0f);
	glPushMatrix();
	// push tr-trt (body + hand) to stack

	glColor3f(0.5f, 0.35f, 0.05f);
	glScalef(1.5f, 0.5f, 0.5f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack


	//left leg()
	glTranslatef(-0.3f, 0.0f, 0.0f);
	glRotatef(0, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.3f, 0.0f, 0.0f);

	glColor3f(0.5f, 0.35f, 0.05f);
	glScalef(1.5f, 0.5f, 0.5f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();
	// get tr (body) from stack



	//right leg start
	glTranslatef(0.1f, -0.9f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.3f, 0.0f, 0.0f);
	glPushMatrix();
	// push tr-trt (body + hand) to stack

	glColor3f(0.5f, 0.35f, 0.05f);
	glScalef(1.5f, 0.5f, 0.5f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();
	// get tr-trt (body + HAND) from stack


	//right leg()
	glTranslatef(-0.3f, 0.0f, 0.0f);
	glRotatef(0, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.3f, 0.0f, 0.0f);

	glColor3f(0.5f, 0.35f, 0.05f);
	glScalef(1.5f, 0.5f, 0.5f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.2f, 10, 10);

	glPopMatrix();










	//right hand
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(1.5f, -1.2f, -12.0f);
	//glPushMatrix();

	//glRotatef(90, 0.0f, 0.0f, 1.0f);
	//glTranslatef(1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glScalef(2.0f, 0.5f, 1.0f);
	//glColor3f(0.5f, 0.35f, 0.05f);
	//quadric = gluNewQuadric();
	//gluSphere(quadric, 0.5f, 10, 10);
	//glPopMatrix();

	//glTranslatef(1.0f, 0.0f, 0.0f);
	//glRotatef(6, 0.0f, 0.0f, 1.0f);
	//glTranslatef(0.91f, 0.0f, 0.0f);
	//glPushMatrix();
	//glScalef(2.0f, 0.5f, 1.0f);
	//glColor3f(0.5f, 0.35f, 0.05f);
	//quadric = gluNewQuadric();
	//gluSphere(quadric, 0.5f, 10, 10);
	//glPopMatrix();

	//glPopMatrix();



	////left hand
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(-1.5f, -1.2f, -12.0f);
	//glPushMatrix();

	//glRotatef(90, 0.0f, 0.0f, 1.0f);
	//glTranslatef(1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glScalef(2.0f, 0.5f, 1.0f);
	//glColor3f(0.5f, 0.35f, 0.05f);
	//quadric = gluNewQuadric();
	//gluSphere(quadric, 0.5f, 10, 10);
	//glPopMatrix();

	//glTranslatef(1.0f, 0.0f, 0.0f);
	//glRotatef(-6, 0.0f, 0.0f, 1.0f);
	//glTranslatef(0.91f, 0.0f, 0.0f);
	//glPushMatrix();
	//glScalef(2.0f, 0.5f, 1.0f);
	//glColor3f(0.5f, 0.35f, 0.05f);
	//quadric = gluNewQuadric();
	//gluSphere(quadric, 0.5f, 10, 10);
	//glPopMatrix();

	//glPopMatrix();
	//glTranslatef(0.0f, 0.0f, -12.0f);
	//
	////body
	////glBegin(GL_QUADS);
	////glVertex3f(-1.0f, -1.0f, 0.0f);
	////glVertex3f(5.0f, -1.0f, 0.0f);
	////glVertex3f(5.0f, 7.0f, 0.0f);
	////glVertex3f(-1.0f, 7.0f, 0.0f);
	////glEnd();


	////right leg

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(1.0f, -4.0f, -12.0f);
	//glPushMatrix();

	//glRotatef(90, 0.0f, 0.0f, 1.0f);
	//glTranslatef(1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glScalef(2.0f, 0.5f, 1.0f);
	//glColor3f(0.5f, 0.35f, 0.05f);
	//quadric = gluNewQuadric();
	//gluSphere(quadric, 0.5f, 10, 10);
	//glPopMatrix();

	//glTranslatef(1.0f, 0.0f, 0.0f);
	//glRotatef(6, 0.0f, 0.0f, 1.0f);
	//glTranslatef(0.91f, 0.0f, 0.0f);
	//glPushMatrix();
	//glScalef(2.0f, 0.5f, 1.0f);
	//glColor3f(0.5f, 0.35f, 0.05f);
	//quadric = gluNewQuadric();
	//gluSphere(quadric, 0.5f, 10, 10);
	//glPopMatrix();

	//glPopMatrix();


	////left leg

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(-1.0f, -4.0f, -12.0f);
	//glPushMatrix();

	//glRotatef(90, 0.0f, 0.0f, 1.0f);
	//glTranslatef(1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glScalef(2.0f, 0.5f, 1.0f);
	//glColor3f(0.5f, 0.35f, 0.05f);
	//quadric = gluNewQuadric();
	//gluSphere(quadric, 0.5f, 10, 10);
	//glPopMatrix();

	//glTranslatef(1.0f, 0.0f, 0.0f);
	//glRotatef(-6, 0.0f, 0.0f, 1.0f);
	//glTranslatef(0.91f, 0.0f, 0.0f);
	//glPushMatrix();
	//glScalef(2.0f, 0.5f, 1.0f);
	//glColor3f(0.5f, 0.35f, 0.05f);
	//quadric = gluNewQuadric();
	//gluSphere(quadric, 0.5f, 10, 10);
	//glPopMatrix();

	//glPopMatrix();
	//glTranslatef(0.0f, 0.0f, -12.0f);



	////head

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(0.0f, 3.5f, -12.0f);
	//gluSphere(quadric, 0.5f, 10, 10);

	//glTranslatef(0.0f, -1.3f, -12.0f);
	//glScalef(1.0f, 1.5f, 1.0f);
	//gluSphere(quadric, 2.5f, 50, 50);

	//glTranslatef(0.0f, 3.3f, -12.0f);
	//glScalef(2.3f, 0.5f, 1.0f);
	//gluSphere(quadric, 2.0f, 50, 50);



	//glFlush();
	SwapBuffers(ghdc);
}

void uninitialize(void)
{

	if (gbFullScreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);

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
	}
}
