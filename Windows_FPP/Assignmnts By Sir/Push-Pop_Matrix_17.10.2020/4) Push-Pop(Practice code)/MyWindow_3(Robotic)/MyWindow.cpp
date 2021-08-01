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
GLfloat z = 24.0f;


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
            RsholderFeet = (RsholderFeet + 3) % 45;
            LsholderFeet = (LsholderFeet - 3) % 45;
            break;

        case 's':
            Rsholder = (Rsholder - 3) % 360;
            Lsholder = (Lsholder + 3) % 360;
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

    void Head();
    void Right_Hand();
    void Left_Hand();
    void Right_Feet();
    void Left_Feet();
    void Bird();

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

   // glTranslatef(0.0f,0.0f,-20.0f);

    //Head();
    //Right_Hand();
    //Left_Hand();

    //Right_Feet();
    //Left_Feet();
    //Bird();


    gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glPushMatrix();

    Head();

    //glPopMatrix();
    glPushMatrix();
    glRotatef((GLfloat)Rsholder,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,0.0f);
    glTranslatef(2.3f,1.0f,0.0f);   //3.5,1.0,0.0

    glPushMatrix();
    glScalef(0.4f,2.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.5f,20,20);
    glPopMatrix();

    glTranslatef(0.0f,-1.0f,0.0f);
    glRotatef((GLfloat)Relbow,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-1.0f,0.0f);

    glPushMatrix();
    glScalef(0.4f,2.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.5f,20,20);
    glPopMatrix();

    glTranslatef(0.0f,-1.0f,0.0f);
    glRotatef((GLfloat)Rpalm,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-0.3f,0.0f);

    glPushMatrix();
    glScalef(0.6f,0.8f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.35f,20,20);
    glPopMatrix();
    glPopMatrix();

    //------------------------------------------------
    glPushMatrix();

    glRotatef((GLfloat)Lsholder,1.0f,0.0f,0.0f);
    glTranslatef(-2.3f,1.0f,0.0f);   //-1.5,1.0,0.0

    glPushMatrix();
    glScalef(0.4f,2.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.5f,20,20);
    glPopMatrix();

    glTranslatef(0.0f,-1.0f,0.0f);
    glRotatef((GLfloat)Lelbow,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-1.0f,0.0f);

    glPushMatrix();
    glScalef(0.4f,2.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.5f,20,20);
    glPopMatrix();

    glTranslatef(0.0f,-1.0f,0.0f);
    glRotatef((GLfloat)Lpalm,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-0.3f,0.0f);

    glPushMatrix();
    glScalef(0.6f,0.8f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.35f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

    Right_Feet();
    Left_Feet();
    //Head();


    SwapBuffers(ghdc);
}

void Head()
{
    glTranslatef(1.0f,0.0f,0.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,0.0f);
    glVertex3f(0.7f,3.7f,0.0f);
    glVertex3f(-0.7f,3.7f,0.0f);
    glVertex3f(-0.7f,2.5f,0.0f);
    glVertex3f(0.7f,2.5f,0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,0.0f);
    glVertex3f(2.5f,2.5f,0.0f);
    glVertex3f(-2.5f,2.5f,0.0f);
    glVertex3f(-1.5f,-1.5f,0.0f);
    glVertex3f(1.5f,-1.5f,0.0f);
    glEnd();
}

void Right_Hand()
{
    glTranslatef(0.5f,0.0f,0.0f);
    glPushMatrix();

    glRotatef((GLfloat)Rsholder,0.0f,1.0f,0.0f);
    glTranslatef(1.0f,0.0f,0.0f);

    glPushMatrix();
    glScalef(2.0f,0.4f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.5f,20,20);
    glPopMatrix();

    glTranslatef(1.0f,0.0f,0.0f);
    glRotatef((GLfloat)Relbow,0.0f,1.0f,0.0f);
    glTranslatef(1.0f,0.0f,0.0f);

    glPushMatrix();
    glScalef(2.0f,0.4f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.5f,20,20);
    glPopMatrix();

    glTranslatef(1.0f,0.0f,0.0f);
    glRotatef((GLfloat)Rpalm,0.0f,1.0f,0.0f);
    glTranslatef(0.3f,0.0f,0.0f);

    glPushMatrix();
    glScalef(0.8f,0.6f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.35f,20,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3f,0.0f,0.0f);
    glRotatef((GLfloat)Rfingure,0.0f,1.0f,0.0f);
    glTranslatef(0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2f,0.10f,0.0f);
    glRotatef((GLfloat)Rfingure,0.0f,1.0f,0.0f);
    glTranslatef(0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25f,-0.10f,0.0f);
    glRotatef((GLfloat)Rfingure,0.0f,1.0f,0.0f);
    glTranslatef(0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f,-0.20f,0.0f);
    glRotatef((GLfloat)Rfingure,0.0f,1.0f,0.0f);
    glTranslatef(0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1f,0.20f,0.0f);
    glRotatef((GLfloat)Rfingure,0.0f,1.0f,0.0f);
    glTranslatef(0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void Right_Feet()
{
// FEET
    glPushMatrix();

    glRotatef((GLfloat)RsholderFeet,1.0f,0.0f,0.0f);
    glTranslatef(0.5f,-2.3f,0.0f);
    glPushMatrix();
    glScalef(1.0f,4.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();

    glTranslatef(0.0f,-0.6f,0.0f);
    glRotatef((GLfloat)RelbowFeet,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-1.0f,0.0f);
    glPushMatrix();
    glScalef(1.0f,4.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();
}

void Left_Hand()
{
//Left
    glTranslatef(-1.3f,0.0f,0.0f);

    glPushMatrix();

    glRotatef((GLfloat)Lsholder,0.0f,1.0f,0.0f);
    glTranslatef(-1.0f,0.0f,0.0f);

    glPushMatrix();
    glScalef(2.0f,0.4f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.5f,20,20);
    glPopMatrix();

    glTranslatef(-1.0f,0.0f,0.0f);
    glRotatef((GLfloat)Lelbow,0.0f,1.0f,0.0f);
    glTranslatef(-1.0f,0.0f,0.0f);

    glPushMatrix();
    glScalef(2.0f,0.4f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.5f,20,20);
    glPopMatrix();

    glTranslatef(-1.0f,0.0f,0.0f);
    glRotatef((GLfloat)Lpalm,0.0f,1.0f,0.0f);
    glTranslatef(-0.3f,0.0f,0.0f);

    glPushMatrix();
    glScalef(0.8f,0.6f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.35f,20,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3f,0.00f,0.0f);
    glRotatef((GLfloat)Lfingure,0.0f,1.0f,0.0f);
    glTranslatef(-0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2f,0.10f,0.0f);
    glRotatef((GLfloat)Lfingure,0.0f,1.0f,0.0f);
    glTranslatef(-0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.25f,-0.10f,0.0f);
    glRotatef((GLfloat)Lfingure,0.0f,1.0f,0.0f);
    glTranslatef(-0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.15f,-0.20f,0.0f);
    glRotatef((GLfloat)Lfingure,0.0f,1.0f,0.0f);
    glTranslatef(-0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1f,0.20f,0.0f);
    glRotatef((GLfloat)Lfingure,0.0f,1.0f,0.0f);
    glTranslatef(-0.2f,0.0f,0.0f);
    glPushMatrix();
    glScalef(1.2f,0.1f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void Left_Feet()
{
    glPushMatrix();

    glRotatef((GLfloat)LsholderFeet,1.0f,0.0f,0.0f);
    glTranslatef(1.5f,-2.3f,0.0f);
    glPushMatrix();
    glScalef(1.0f,4.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();

    glTranslatef(0.0f,-0.6f,0.0f);
    glRotatef((GLfloat)LelbowFeet,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-1.0f,0.0f);
    glPushMatrix();
    glScalef(1.0f,4.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
    glPopMatrix();
    glPopMatrix();
}

void Bird()
{
    glPushMatrix();

    glRotatef((GLfloat)Neck,0.0f,0.0f,1.0f);
    glTranslatef(0.45f,0.0f,0.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.15f,20,20);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f,1.0f,0.0f);
    glVertex3f(0.1f,0.0f,0.0f);
    glVertex3f(-0.1f,0.1f,0.0f);
    glVertex3f(-0.1f,-0.1f,0.0f);
    glEnd();

    glPopMatrix();

    glScalef(2.0f,1.0f,1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric,0.2f,20,20);
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
