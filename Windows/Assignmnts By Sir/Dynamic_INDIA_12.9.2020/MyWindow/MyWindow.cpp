//Header Files

#include<Windows.h>
#include<stdio.h>
#include "MyWindow.h"
#include<gl/gl.h>

#include<gl/glu.h>                // Graphic Library Utility . h

//Macros

#define WIN_WIDTH   800
#define WIN_HEIGHT  600
#pragma comment(lib,"OpenGL32.lib")

#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"Winmm.lib")

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
    PlaySound(TEXT("Final.wav"),NULL,SND_ASYNC | SND_FILENAME);

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
	//code
	glClear(GL_COLOR_BUFFER_BIT);

/*
	glMatrixMode(GL_MODELVIEW);               //RED
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);

	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);

	glBegin(GL_LINES);

	for(GLfloat v = 0.05f;v <= 1.05f; v = v + 0.05f)
	{
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(1.0f,v,0.0f);

		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,v,0.0f);
	}

	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);

	glBegin(GL_LINES);

	for(GLfloat v = 0.05f;v <= 1.05f; v = v + 0.05f)
	{
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(1.0f,-v,0.0f);

		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,-v,0.0f);
	}

	glEnd();

	glMatrixMode(GL_MODELVIEW);     //GREEN
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glEnd();

	//First For Loop (Upper Lines)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);

	glBegin(GL_LINES);

	for(GLfloat v = 0.05f;v <= 1.05f; v = v + 0.05f)
	{
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(v,1.0f,0.0f);

		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(v,-1.0f,0.0f);
	}

	glEnd();

	//Second For Loop (Lower Lines)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);

	glBegin(GL_LINES);

	for(GLfloat v = 0.05f;v <= 1.05f; v = v + 0.05f)
	{
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-v,1.0f,0.0f);

		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-v,-1.0f,0.0f);
	}

	glEnd();

*/

    int flag,flag_1,flag_2,flag_3,flag_4,flag_5,flag_6 = 0;
	// I (Upper)

	static GLfloat x = -2.3f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(x <= -1.6f)
	{
		glTranslatef(x,0.0f,-3.0f);
	}
	else
	{
		glTranslatef(-1.7f,0.0f,-3.0f);
		flag = 1;
	}

	x = x + 0.0003f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.3,0.9,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.3,0.9,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.3,0.8,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.3,0.8,0.0f);

	glEnd();


	// I (Mid)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(x <= -1.6f)
	{
		glTranslatef(x,0.0f,-3.0f);
	}
	else
	{
		glTranslatef(-1.7f,0.0f,-3.0f);
		flag = 1;
	}

	x = x + 0.0003f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.07,0.8,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.07,0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.07,-0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.07,-0.8,0.0f);

	glEnd();

	// I (Lower)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(x <= -1.6f)
	{
		glTranslatef(x,0.0f,-3.0f);
	}
	else
	{
		glTranslatef(-1.7f,0.0f,-3.0f);
		flag = 1;
	}

	x = x + 0.0003f;

	glBegin(GL_QUADS);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.3,-0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.3,-0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.3,-0.9,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.3,-0.9,0.0f);

	glEnd();
if(flag == 1)
{
	// N(Left)
	static GLfloat y = 1.8f;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(y >= 0.1)
    {
        glTranslatef(-1.2f,y,-3.0f);
    }
    else
    {
        glTranslatef(-1.2f,0.0f,-3.0f);
        flag_1 = 1;
    }

    y = y - 0.001f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.07,0.9,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.07,0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.07,-0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.07,-0.9,0.0f);

	glEnd();

	// N (Mid)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(y >= 0.1)
    {
        glTranslatef(-0.95f,y,-3.0f);
        glRotatef(15.5f,0.0f,0.0f,1.0f);
    }
    else
    {
        glTranslatef(-0.95f,0.0f,-3.0f);
        glRotatef(15.5f,0.0f,0.0f,1.0f);
        flag_1 = 1;
    }

    y = y - 0.001f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.07,0.9,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.07,0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.07,-0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.07,-0.8,0.0f);

	glEnd();

	// N(Right)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(y >= 0.1)
    {
        glTranslatef(-0.7f,y,-3.0f);
    }
    else
    {
        glTranslatef(-0.7f,0.0f,-3.0f);
        flag_1 = 1;
    }

    y = y - 0.001f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.07,0.9,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.07,0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.07,-0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.07,-0.9,0.0f);

	glEnd();
}

if(flag_1 == 1)
{
	//  D (Left)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-0.2f,0.0f,-3.0f);

	glBegin(GL_QUADS);

    GLfloat k = 0.0f;
    GLfloat k1 = 0.0f;

    if(k1 <= 0.4f && k <= 0.9f)
    {
        glColor3f(k,k1,0.0f);
        	glVertex3f(0.07,0.8,0.0f);
    }
    else
    {
       	glColor3f(1.0f,0.5f,0.0f);
        glVertex3f(0.07,0.8,0.0f);
        flag_2 = 1;
    }


    if(k1 <= 0.4f && k <= 0.9f )
    {
        glColor3f(k,k1,0.0f);
        	glVertex3f(-0.07,0.8,0.0f);
    }
    else
    {
       	glColor3f(1.0f,0.5f,0.0f);
        glVertex3f(-0.07,0.8,0.0f);
        flag_2 = 1;
    }


	static GLfloat p = 0.0f;

    if(p <= 0.9f)
    {
       	glColor3f(0.0f,p,0.0f);
    }
    else
    {
       	glColor3f(0.0f,1.0f,0.0f);
       	flag_2 = 1;
    }
	glVertex3f(-0.07,-0.8,0.0f);

    if(p <= 0.9f)
    {
       	glColor3f(0.0f,p,0.0f);
    }
    else
    {
       	glColor3f(0.0f,1.0f,0.0f);
       	flag_2 = 1;
    }
	glVertex3f(0.07,-0.8,0.0f);

    k = k + 0.001f;
    k1 = k1 + (0.001f/2.0f);
    p = p + 0.001f;

	glEnd();

	// D (Upper)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-0.2f,0.0f,-3.0f);

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.4,0.9,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.3,0.9,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.3,0.8,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.4,0.8,0.0f);

	glEnd();


	// D (Right)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.2f,0.0f,-3.0f);

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.07,0.9,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.07,0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.07,-0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.07,-0.9,0.0f);

	glEnd();


	// D (Lower)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-0.2f,0.0f,-3.0f);

	glBegin(GL_QUADS);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.4,-0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.3,-0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.3,-0.9,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.4,-0.9,0.0f);

	glEnd();
}

if(flag_2 == 1)
{
	// I (2nd : Upper)
    static GLfloat a = -1.8f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(a <= 0.1f)
    {
      	glTranslatef(0.7f,a,-3.0f);
    }
    else
    {
        glTranslatef(0.7f,0.0f,-3.0f);
        flag_3 = 1;
    }
    a = a + 0.0005f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.3,0.9,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.3,0.9,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.3,0.8,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.3,0.8,0.0f);

	glEnd();


	// I (2nd : Mid)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(a <= 0.1f)
    {
      	glTranslatef(0.7f,a,-3.0f);
    }
    else
    {
        glTranslatef(0.7f,0.0f,-3.0f);
        flag_3 = 1;
    }
    a = a + 0.0005f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.07,0.8,0.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.07,0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.07,-0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.07,-0.8,0.0f);

	glEnd();

	// I (2nd : Lower)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(a <= 0.1f)
    {
      	glTranslatef(0.7f,a,-3.0f);
    }
    else
    {
        glTranslatef(0.7f,0.0f,-3.0f);
        flag_3 = 1;
    }
    a = a + 0.0005f;

	glBegin(GL_QUADS);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.3,-0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.3,-0.8,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.3,-0.9,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.3,-0.9,0.0f);

	glEnd();
}

if(flag_3 == 1)
{
	// A (Left)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    static GLfloat b = 2.73;

    if(b >= 1.7f)
    {
       	glTranslatef(b,0.0f,-3.0f);
        glRotatef(15.5f,0.0f,0.0f,1.0f);
    }
    else
    {
     	glTranslatef(1.8f,0.0f,-3.0f);
        glRotatef(15.5f,0.0f,0.0f,1.0f);
        flag_4 = 1;
    }
    b = b - 0.0005f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.07,0.95,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.07,0.95,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.07,-0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.07,-0.9,0.0f);

	glEnd();


	// A (Mid)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(b >= 1.4f)
    {
       	glTranslatef(b,0.0f,-3.0f);
    }
    else
    {
     	glTranslatef(1.5f,0.0f,-3.0f);
    }
    b = b - 0.0005f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.20,0.1,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.20,0.1,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.20,-0.1,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.26,-0.1,0.0f);

	glEnd();

	// A (Right)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(b >= 1.2f)
    {
       	glTranslatef(b,0.0f,-3.0f);
        glRotatef(-15.5f,0.0f,0.0f,1.0f);
    }
    else
    {
     	glTranslatef(1.3f,0.0f,-3.0f);
        glRotatef(-15.5f,0.0f,0.0f,1.0f);
        flag_4 = 1;
    }
    b = b - 0.0005f;

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.07,0.95,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-0.07,0.95,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.07,-0.9,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.07,-0.9,0.0f);

	glEnd();
}

if(flag_4 == 1)
{
    static GLfloat j = -3.5f;
    static GLfloat i = -3.5f;
    static GLfloat angle =  45.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(j <= -0.1f)
    {
       	glTranslatef(i,j,-5.0f);
        glRotatef(angle,0.0f,0.0f,1.0f);
    }
    else
    {
        glTranslatef(i,0.0f,-5.0f);
        glRotatef(0.0f,0.0f,0.0f,0.0f);
    }

    i = i + 0.002f;
    j = j + 0.002f;
    angle = angle - 0.01f;

	glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.3f,0.1f,0.0f);
	glVertex3f(-0.7f,0.1f,0.0f);
	glVertex3f(-0.7f,-0.1f,0.0f);
	glVertex3f(0.3f,-0.1f,0.0f);

    glEnd();

	glBegin(GL_TRIANGLES);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.3f,-0.1f,0.0f);
	glVertex3f(0.4f,0.0f,0.0f);
	glVertex3f(0.3f,0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.0f,0.1f,0.0f);
	glVertex3f(-0.3f,0.4f,0.0f);
	glVertex3f(-0.4f,0.4f,0.0f);
	glVertex3f(-0.3f,0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.0f,-0.1f,0.0f);
	glVertex3f(-0.3f,-0.4f,0.0f);
	glVertex3f(-0.4f,-0.4f,0.0f);
	glVertex3f(-0.3f,-0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(-0.7f,0.1f,0.0f);
	glVertex3f(-0.8f,0.2f,0.0f);
	glVertex3f(-0.75f,0.0f,0.0f);
	glVertex3f(-0.7f,0.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(-0.7f,-0.1f,0.0f);
	glVertex3f(-0.8f,-0.2f,0.0f);
	glVertex3f(-0.75f,0.0f,0.0f);
	glVertex3f(-0.7f,0.0f,0.0f);

    glEnd();

    static GLfloat j1 = 3.5f;
    static GLfloat i1 = -3.5f;
    static GLfloat angle0 = 45.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(j1 >= 0.1f)
    {
       	glTranslatef(i1,j1,-5.0f);
        glRotatef(angle0,0.0f,0.0f,-1.0f);
    }
    else
    {
        glTranslatef(i1,0.0f,-5.0f);
        glRotatef(0.0f,0.0f,0.0f,0.0f);
    }

    i1 = i1 + 0.002f;
    j1 = j1 - 0.002f;
    angle0 = angle0 - 0.01f;

	glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.3f,0.1f,0.0f);
	glVertex3f(-0.7f,0.1f,0.0f);
	glVertex3f(-0.7f,-0.1f,0.0f);
	glVertex3f(0.3f,-0.1f,0.0f);

    glEnd();

	glBegin(GL_TRIANGLES);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.3f,-0.1f,0.0f);
	glVertex3f(0.4f,0.0f,0.0f);
	glVertex3f(0.3f,0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.0f,0.1f,0.0f);
	glVertex3f(-0.3f,0.4f,0.0f);
	glVertex3f(-0.4f,0.4f,0.0f);
	glVertex3f(-0.3f,0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.0f,-0.1f,0.0f);
	glVertex3f(-0.3f,-0.4f,0.0f);
	glVertex3f(-0.4f,-0.4f,0.0f);
	glVertex3f(-0.3f,-0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(-0.7f,0.1f,0.0f);
	glVertex3f(-0.8f,0.2f,0.0f);
	glVertex3f(-0.75f,0.0f,0.0f);
	glVertex3f(-0.7f,0.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(-0.7f,-0.1f,0.0f);
	glVertex3f(-0.8f,-0.2f,0.0f);
	glVertex3f(-0.75f,0.0f,0.0f);
	glVertex3f(-0.7f,0.0f,0.0f);

    glEnd();

    //static GLfloat j2 = 0f;
    static GLfloat i2 = -3.5f;
    //static GLfloat angle_2 = 45.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glTranslatef(i2,0.0f,-5.0f);

    i2 = i2 + 0.002f;
   // j2 = j2 + 0.001f;
   // angle_2 = angle_2 - 0.01f;

	glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.3f,0.1f,0.0f);
	glVertex3f(-0.7f,0.1f,0.0f);
	glVertex3f(-0.7f,-0.1f,0.0f);
	glVertex3f(0.3f,-0.1f,0.0f);

    glEnd();

	glBegin(GL_TRIANGLES);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.3f,-0.1f,0.0f);
	glVertex3f(0.4f,0.0f,0.0f);
	glVertex3f(0.3f,0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.0f,0.1f,0.0f);
	glVertex3f(-0.3f,0.4f,0.0f);
	glVertex3f(-0.4f,0.4f,0.0f);
	glVertex3f(-0.3f,0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.0f,-0.1f,0.0f);
	glVertex3f(-0.3f,-0.4f,0.0f);
	glVertex3f(-0.4f,-0.4f,0.0f);
	glVertex3f(-0.3f,-0.1f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(-0.7f,0.1f,0.0f);
	glVertex3f(-0.8f,0.2f,0.0f);
	glVertex3f(-0.75f,0.0f,0.0f);
	glVertex3f(-0.7f,0.0f,0.0f);

    glEnd();

    glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(-0.7f,-0.1f,0.0f);
	glVertex3f(-0.8f,-0.2f,0.0f);
	glVertex3f(-0.75f,0.0f,0.0f);
	glVertex3f(-0.7f,0.0f,0.0f);

    glEnd();
}
/*
if(flag_6 == 1)
{
    static GLfloat b = 2.73;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(b >= 1.4f)
    {
       	glTranslatef(b,0.0f,-3.0f);
    }
    else
    {
     	glTranslatef(1.5f,0.0f,-3.0f);
    }
    b = b - 0.001f;
    glEnd();
}
*/
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
