//Header Files

#include<Windows.h>
#include<stdio.h>
#include "MyWindow.h"
#include<gl/gl.h>
#define _USE_MATH_DEFINES
#include<math.h>

GLfloat IdentityMatrix[16];
GLfloat TranslationMatrix[16];
GLfloat ScaleMatrix[16];
GLfloat RotationMatrix_X[16];
GLfloat RotationMatrix_Y[16];
GLfloat RotationMatrix_Z[16];

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

	if (fopen_s(&gpFile, "BhaveshLog.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Can't Create file"), TEXT("Error"), MB_OK);
		exit(0);
	}

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

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, TEXT("BHAVESH DILIP JOSHI!!!"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, X, Y, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

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
		fprintf(gpFile, "India is My Country.\n");
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

	case WM_DESTROY:
		fprintf(gpFile, "Jai Hind");
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
	pfd.cDepthBits = 32;

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

	IdentityMatrix[0] = 1.0f;
	IdentityMatrix[1] = 0.0f;
	IdentityMatrix[2] = 0.0f;
	IdentityMatrix[3] = 0.0f;
	IdentityMatrix[4] = 0.0f;
	IdentityMatrix[5] = 1.0f;
	IdentityMatrix[6] = 0.0f;
	IdentityMatrix[7] = 0.0f;
	IdentityMatrix[8] = 0.0f;
	IdentityMatrix[9] = 0.0f;
	IdentityMatrix[10] = 1.0f;
	IdentityMatrix[11] = 0.0f;
	IdentityMatrix[12] = 0.0f;
	IdentityMatrix[13] = 0.0f;
	IdentityMatrix[14] = 0.0f;
	IdentityMatrix[15] = 1.0f;

    TranslationMatrix[0] = 1.0f;
	TranslationMatrix[1] = 0.0f;
	TranslationMatrix[2] = 0.0f;
	TranslationMatrix[3] = 0.0f;
	TranslationMatrix[4] = 0.0f;
	TranslationMatrix[5] = 1.0f;
	TranslationMatrix[6] = 0.0f;
	TranslationMatrix[7] = 0.0f;
	TranslationMatrix[8] = 0.0f;
	TranslationMatrix[9] = 0.0f;
	TranslationMatrix[10] = 1.0f;
	TranslationMatrix[11] = 0.0f;
	TranslationMatrix[12] = 0.0f;
	TranslationMatrix[13] = 0.0f;
	TranslationMatrix[14] = -4.0f;
	TranslationMatrix[15] = 1.0f;

    ScaleMatrix[0] = 0.75f;
	ScaleMatrix[1] = 0.0f;
	ScaleMatrix[2] = 0.0f;
	ScaleMatrix[3] = 0.0f;
	ScaleMatrix[4] = 0.0f;
	ScaleMatrix[5] = 0.75f;
	ScaleMatrix[6] = 0.0f;
	ScaleMatrix[7] = 0.0f;
	ScaleMatrix[8] = 0.0f;
	ScaleMatrix[9] = 0.0f;
	ScaleMatrix[10] = 0.75f;
	ScaleMatrix[11] = 0.0f;
	ScaleMatrix[12] = 0.0f;
	ScaleMatrix[13] = 0.0f;
	ScaleMatrix[14] = 0.0f;
	ScaleMatrix[15] = 1.0f;


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
    static GLfloat angle_cube = 0.0f;
	static GLfloat angle_cube_rad;

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();

    glLoadMatrixf(IdentityMatrix);

//	glTranslatef(0.0f,0.0f,-6.0f);

    glMultMatrixf(TranslationMatrix);

//   glScalef(0.75f,0.75f,0.75f);

    glMultMatrixf(ScaleMatrix);

//	glRotatef(cube_angle,1.0f,0.0f,0.0f);
//	glRotatef(cube_angle,0.0f,1.0f,0.0f);
//	glRotatef(cube_angle,0.0f,0.0f,1.0f);


    angle_cube_rad = (angle_cube * M_PI)/180.0f;

    RotationMatrix_X[0] = 1.0f;
	RotationMatrix_X[1] = 0.0f;
	RotationMatrix_X[2] = 0.0f;
	RotationMatrix_X[3] = 0.0f;
	RotationMatrix_X[4] = 0.0f;
	RotationMatrix_X[5] = cos(angle_cube_rad);
	RotationMatrix_X[6] = sin(angle_cube_rad);
	RotationMatrix_X[7] = 0.0f;
	RotationMatrix_X[8] = 0.0f;
	RotationMatrix_X[9] = -sin(angle_cube_rad);
	RotationMatrix_X[10] = cos(angle_cube_rad);
	RotationMatrix_X[11] = 0.0f;
	RotationMatrix_X[12] = 0.0f;
	RotationMatrix_X[13] = 0.0f;
	RotationMatrix_X[14] = 0.0f;
	RotationMatrix_X[15] = 1.0f;

    RotationMatrix_Y[0] = cos(angle_cube_rad);
	RotationMatrix_Y[1] = 0.0f;
	RotationMatrix_Y[2] = -sin(angle_cube_rad);
	RotationMatrix_Y[3] = 0.0f;
	RotationMatrix_Y[4] = 0.0f;
	RotationMatrix_Y[5] = 1.0f;
	RotationMatrix_Y[6] = 0.0f;
	RotationMatrix_Y[7] = 0.0f;
	RotationMatrix_Y[8] = sin(angle_cube_rad);
	RotationMatrix_Y[9] = 0.0f;
	RotationMatrix_Y[10] = cos(angle_cube_rad);
	RotationMatrix_Y[11] = 0.0f;
	RotationMatrix_Y[12] = 0.0f;
	RotationMatrix_Y[13] = 0.0f;
	RotationMatrix_Y[14] = 0.0f;
	RotationMatrix_Y[15] = 1.0f;

    RotationMatrix_Z[0] = cos(angle_cube_rad);
	RotationMatrix_Z[1] = sin(angle_cube_rad);
	RotationMatrix_Z[2] = 0.0f;
	RotationMatrix_Z[3] = 0.0f;
	RotationMatrix_Z[4] = -sin(angle_cube_rad);
	RotationMatrix_Z[5] = cos(angle_cube_rad);
	RotationMatrix_Z[6] = 0.0f;
	RotationMatrix_Z[7] = 0.0f;
	RotationMatrix_Z[8] = 0.0f;
	RotationMatrix_Z[9] = 0.0f;
	RotationMatrix_Z[10] = 1.0f;
	RotationMatrix_Z[11] = 0.0f;
	RotationMatrix_Z[12] = 0.0f;
	RotationMatrix_Z[13] = 0.0f;
	RotationMatrix_Z[14] = 0.0f;
	RotationMatrix_Z[15] = 1.0f;

	glMultMatrixf(RotationMatrix_X);
	glMultMatrixf(RotationMatrix_Y);
	glMultMatrixf(RotationMatrix_Z);

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.5f,0.5f,0.5f);
	glVertex3f(-0.5f,0.5f,0.5f);
	glVertex3f(-0.5f,-0.5f,0.5f);
	glVertex3f(0.5f,-0.5f,0.5f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.5f,0.5f,-0.5f);
	glVertex3f(0.5f,0.5f,0.5f);
	glVertex3f(0.5f,-0.5f,0.5f);
	glVertex3f(0.5f,-0.5f,-0.5f);

	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(-0.5f,0.5f,-0.5f);
	glVertex3f(0.5f,0.5f,-0.5f);
	glVertex3f(0.5f,-0.5f,-0.5f);
	glVertex3f(-0.5f,-0.5f,-0.5f);

	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-0.5f,0.5f,0.5f);
	glVertex3f(-0.5f,0.5f,-0.5f);
	glVertex3f(-0.5f,-0.5f,-0.5f);
	glVertex3f(-0.5f,-0.5f,0.5f);

	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(0.5f,0.5f,-0.5f);
	glVertex3f(-0.5f,0.5f,-0.5f);
	glVertex3f(-0.5f,0.5f,0.5f);
	glVertex3f(0.5f,0.5f,0.5f);

	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f(0.5f,-0.5f,-0.5f);
	glVertex3f(-0.5f,-0.5f,-0.5f);
	glVertex3f(-0.5f,-0.5f,0.5f);
	glVertex3f(0.5f,-0.5f,0.5f);

	glEnd();

	angle_cube = angle_cube + 0.1f;

	//glFlush();
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
