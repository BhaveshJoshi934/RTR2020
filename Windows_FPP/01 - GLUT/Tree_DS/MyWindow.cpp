//Header Files

#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include "MyWindow.h"
#include<gl/gl.h>
#include<math.h>

#include<gl/glu.h>                // Graphic Library Utility . h

//Macros

#define WIN_WIDTH   1200
#define WIN_HEIGHT  800
#define PI 3.141592653589793238

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

float x;
float y = 0.8f;
float z = 50.0f;

void MakeTree(float , float);

GLuint MakeATree;


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

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, TEXT("TREE Using Recursion!!!"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, X, Y, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

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

    case WM_CHAR:
        switch(wParam)
        {
        case 'X':
            x = x - 1.0f;
            if(x <= -50.0f)
            {
                x = -50.0f;
            }
             break;

        case 'x':
            x = x + 1.0f;
            if(x >= 50.0f)
            {
                x = 50.0f;
            }
            break;

        case 'Y':
            y = y - 1.0f;
            if(y <= -50.0f)
            {
                y = -50.0f;
            }
            break;

        case 'y':
            y = y + 1.0f;
            if(y >= 50.0f)
            {
                y = 50.0f;
            }
            break;

        case 'Z':
            z = z - 1.0f;
            if(z <= -50.0f)
            {
                z = -50.0f;
            }
            break;

        case 'z':
            z = z + 1.0f;
            if(z >= 50.0f)
            {
                z = 50.0f;
            }
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    MakeATree = glGenLists(1);

    glNewList(MakeATree,GL_COMPILE);

    MakeTree(4.0f,0.3f);

    glEndList();

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

	gluPerspective(30.0f,(GLfloat)width/(GLfloat)height,0.001f,1000.0f);

}

void Display(void)
{
    //void DrawCircle(void);
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glTranslatef(0.0,-8.0,0.0);  // (0.0 , - 8.0 , -50)

    gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glPushMatrix();

    glCallList(MakeATree);

    glPopMatrix();

   // DrawCircle();

	//glFlush();
	SwapBuffers(ghdc);
}

void MakeTree(float height , float base)
{
    void MakeCylinder(float , float);
    float angle;
    MakeCylinder(height , base);
    glTranslatef(0.0f,height,0.0f);
    height = height - height * 0.2f;
    base = base - base * 0.3f;

    for(int i = 0 ; i < 3 ; i++)
    {
        angle = rand() % 50 + 20;
        if(angle > 48)
        {
            angle = - (rand() % 50 + 20);
        }

        if(height > 1)
        {
            glPushMatrix();
            glRotatef(angle,1.0f,0.0f,1.0f);
            MakeTree(height , base);
            glPopMatrix();
        }
        else
        {
            glColor3f(0.0f,1.0f,0.0f);
            GLUquadric *quadric = gluNewQuadric();
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            gluSphere(quadric,0.2f,20,20);
        }
    }

}

void MakeCylinder(float height , float base)
{
    GLUquadric *quadric = gluNewQuadric();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3f(0.64f, 0.16f, 0.16f);
    glPushMatrix();
    glRotatef(-90.0f,1.0f,0.0f,0.0f);
    gluCylinder(quadric,base,base - (0.2f*base),height,20,20);
    glPopMatrix();
}
/*
void DrawCircle()
{
    glBegin(GL_LINE_LOOP);

    for(float i = 0.0f ; i <= 2 * PI ; i = i + 0.01f)
    {
        glVertex3f(cos(i)/2 ,0.0f,sin(i)/2 );
    }

    glEnd();
}
*/

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
