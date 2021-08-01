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


bool gbLight = false;

GLfloat lightAmbient[] = {0.0f,0.0f,0.0f,1.0f};  //Gray Light
GLfloat lightDiffused[] = {1.0f,1.0f,1.0f,1.0f}; //White Light
GLfloat lightPosition[] = {0.0f,3.0f,3.0f,0.0f};  //Z ais madhun aat yetoy

GLfloat light_Model_Ambient[] = {0.2f,0.2f,0.2f,1.0f};
GLfloat light_Model_Local_Viewer[] = {0.0f};

GLfloat AngleX = 0.0f;
GLfloat AngleY = 0.0f;
GLfloat AngleZ = 0.0f;


GLUquadric *quadric[24];

int KeyPressed = 0;

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

    case WM_CHAR:
        switch(wParam)
        {
        case 'L':
        case 'l':
            if(gbLight == false)
            {
                glEnable(GL_LIGHTING);
                gbLight = true;
            }
            else
            {
                glDisable(GL_LIGHTING);
                gbLight = false;
            }
            break;

        case 'X':
        case 'x':
            KeyPressed = 1;
            AngleX = 0.0f;
            break;

        case 'Y':
        case 'y':
            KeyPressed = 2;
            AngleY = 0.0f;
            break;

        case 'Z':
        case 'z':
            KeyPressed = 3;
            AngleZ = 0.0f;
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


	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_Model_Ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER,light_Model_Local_Viewer);

    //Lights
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffused);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
	glEnable(GL_LIGHT0);

	for(GLint i = 0 ; i < 24 ; i++)
    {
        quadric[i] = gluNewQuadric();
    }

    //SetClearColor
	glClearColor(0.25f, 0.25f, 0.25f, 0.0f);

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

	if(width <= height)
	{
		glOrtho(0.0f,
			 15.5f,
			 0.0f,
			 15.5f*((GLfloat)height/(GLfloat)width),
			-10.0f,
			 10.0f);
	}

	else
	{
		glOrtho(0.0f,
			 15.5f*((GLfloat)width/(GLfloat)height),
			 0.0f,
			 15.5f,
			-10.0f,
			 10.0f);
	}
}

void Display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    void DrawTwentyFourSphere(void);

    if(KeyPressed == 1)
    {
        AngleX = AngleX + 1.0f;
        glRotatef(AngleX,1.0f,0.0f,0.0f);
        lightPosition[1] = AngleX;
    }

    else if(KeyPressed == 2)
    {
        AngleY = AngleY + 1.0f;
        glRotatef(AngleY,0.0f,1.0f,0.0f);
        lightPosition[2] = AngleY;
    }

    else if(KeyPressed == 3)
    {
        AngleZ = AngleZ + 1.0f;
        glRotatef(AngleZ,0.0f,0.0f,1.0f);
        lightPosition[0] = AngleZ;
    }

    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

    DrawTwentyFourSphere();

	SwapBuffers(ghdc);
}

void DrawTwentyFourSphere()
{
    GLfloat materialAmbient[4];
    GLfloat materialDiffused[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//1.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0215;
    materialAmbient[1] = 0.1745;
    materialAmbient[2] = 0.0215;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.07568;
    materialDiffused[1] = 0.61424;
    materialDiffused[2] = 0.07568;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.633;
    materialSpecular[1] = 0.727811;
    materialSpecular[2] = 0.633;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.6 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1.5f,14.0f,0.0f);
    gluSphere(quadric[0],1.0f,30,30);
//2.------------------------------------------------------------------------------
    materialAmbient[0] = 0.135;
    materialAmbient[1] = 0.2225;
    materialAmbient[2] = 0.1575;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.54;
    materialDiffused[1] = 0.89;
    materialDiffused[2] = 0.63;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.316228;
    materialSpecular[1] = 0.316228;
    materialSpecular[2] = 0.316228;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.1 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(26.0f,14.0f,0.0f);     //Center :14
    gluSphere(quadric[1],1.0f,30,30);
    //3.------------------------------------------------------------------------------
    materialAmbient[0] = 0.05375;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.06625;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.18275;
    materialDiffused[1] = 0.17;
    materialDiffused[2] = 0.22525;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.332741;
    materialSpecular[1] = 0.328634;
    materialSpecular[2] = 0.346435;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.3 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(11.5f,14.0f,0.0f);     //Center :14
    gluSphere(quadric[1],1.0f,30,30);
    //4.------------------------------------------------------------------------------
    materialAmbient[0] = 0.25;
    materialAmbient[1] = 0.20725;
    materialAmbient[2] = 0.20725;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 1.0;
    materialDiffused[1] = 0.829;
    materialDiffused[2] = 0.829;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.296648;
    materialSpecular[1] = 0.296648;
    materialSpecular[2] = 0.296648;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.088 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(6.5f,14.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //5.------------------------------------------------------------------------------
    materialAmbient[0] = 0.1745;
    materialAmbient[1] = 0.01175;
    materialAmbient[2] = 0.01175;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.61424;
    materialDiffused[1] = 0.04136;
    materialDiffused[2] = 0.04136;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.727811;
    materialSpecular[1] = 0.626959;
    materialSpecular[2] = 0.626959;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.6 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(16.5f,14.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //6.------------------------------------------------------------------------------
    materialAmbient[0] = 0.1;
    materialAmbient[1] = 0.18725;
    materialAmbient[2] = 0.1745;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.396;
    materialDiffused[1] = 0.74151;
    materialDiffused[2] = 0.69102;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.297254;
    materialSpecular[1] = 0.30829;
    materialSpecular[2] = 0.306678;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.1 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(21.5f,14.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //7.------------------------------------------------------------------------------
    materialAmbient[0] = 0.329412;
    materialAmbient[1] = 0.223529;
    materialAmbient[2] = 0.027451;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.780392;
    materialDiffused[1] = 0.568627;
    materialDiffused[2] = 0.113725;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.992157;
    materialSpecular[1] = 0.941176;
    materialSpecular[2] = 0.807843;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.21794872 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1.5f,10.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //8.------------------------------------------------------------------------------
    materialAmbient[0] = 0.1;
    materialAmbient[1] = 0.18725;
    materialAmbient[2] = 0.1745;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.396;
    materialDiffused[1] = 0.74151;
    materialDiffused[2] = 0.69102;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.297254;
    materialSpecular[1] = 0.30829;
    materialSpecular[2] = 0.306678;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.1 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(21.5f,14.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //9.------------------------------------------------------------------------------
    materialAmbient[0] = 0.2125;
    materialAmbient[1] = 0.1275;
    materialAmbient[2] = 0.054;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.714;
    materialDiffused[1] = 0.4284;
    materialDiffused[2] = 0.18144;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.393548;
    materialSpecular[1] = 0.271906;
    materialSpecular[2] = 0.166721;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.2 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(6.5f,10.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //10.------------------------------------------------------------------------------
    materialAmbient[0] = 0.25;
    materialAmbient[1] = 0.25;
    materialAmbient[2] = 0.25;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.4;
    materialDiffused[1] = 0.4;
    materialDiffused[2] = 0.4;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.774597;
    materialSpecular[1] = 0.774597;
    materialSpecular[2] = 0.774597;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.6 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(11.5f,10.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //11.------------------------------------------------------------------------------
    materialAmbient[0] = 0.19125;
    materialAmbient[1] = 0.0735;
    materialAmbient[2] = 0.0225;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.7038;
    materialDiffused[1] = 0.27048;
    materialDiffused[2] = 0.0828;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.256777;
    materialSpecular[1] = 0.137622;
    materialSpecular[2] = 0.086014;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.1 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(16.5f,10.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //12.------------------------------------------------------------------------------
    materialAmbient[0] = 0.24725;
    materialAmbient[1] = 0.1995;
    materialAmbient[2] = 0.0745;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.75164;
    materialDiffused[1] = 0.60648;
    materialDiffused[2] = 0.22648;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.628281;
    materialSpecular[1] = 0.555802;
    materialSpecular[2] = 0.366065;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.4 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(21.5f,10.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //13.------------------------------------------------------------------------------
    materialAmbient[0] = 0.19225;
    materialAmbient[1] = 0.19225;
    materialAmbient[2] = 0.19225;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.50754;
    materialDiffused[1] = 0.50754;
    materialDiffused[2] = 0.50754;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.508273;
    materialSpecular[1] = 0.508273;
    materialSpecular[2] = 0.508273;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.4 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(26.0f,10.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //14.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.01;
    materialDiffused[1] = 0.01;
    materialDiffused[2] = 0.01;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.50;
    materialSpecular[1] = 0.50;
    materialSpecular[2] = 0.50;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.25 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1.5f,6.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //15.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.1;
    materialAmbient[2] = 0.06;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.0;
    materialDiffused[1] = 0.50980392;
    materialDiffused[2] = 0.50980392;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.50196078;
    materialSpecular[1] = 0.50196078;
    materialSpecular[2] = 0.50196078;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.25 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(6.5f,6.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //16.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.1;
    materialDiffused[1] = 0.35;
    materialDiffused[2] = 0.1;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.45;
    materialSpecular[1] = 0.55;
    materialSpecular[2] = 0.45;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.25 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(11.5f,6.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //17.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.5;
    materialDiffused[1] = 0.0;
    materialDiffused[2] = 0.0;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.6;
    materialSpecular[2] = 0.6;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.25 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(16.5f,6.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //18.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.55;
    materialDiffused[1] = 0.55;
    materialDiffused[2] = 0.55;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.70;
    materialSpecular[1] = 0.70;
    materialSpecular[2] = 0.70;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.25 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(21.5f,6.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //19.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.5;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.0;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.60;
    materialSpecular[1] = 0.60;
    materialSpecular[2] = 0.50;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.25 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(26.0f,6.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //20.------------------------------------------------------------------------------
    materialAmbient[0] = 0.02;
    materialAmbient[1] = 0.02;
    materialAmbient[2] = 0.02;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.01;
    materialDiffused[1] = 0.01;
    materialDiffused[2] = 0.01;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.4;
    materialSpecular[1] = 0.4;
    materialSpecular[2] = 0.4;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.078125 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1.5f,2.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //21.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.05;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.4;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.5;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.04;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.7;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.078125 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(6.5f,2.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //22.------------------------------------------------------------------------------
    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.4;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.4;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.04;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.04;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.078125 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(11.5f,2.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //23.------------------------------------------------------------------------------
    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.5;
    materialDiffused[1] = 0.4;
    materialDiffused[2] = 0.4;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.04;
    materialSpecular[2] = 0.04;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.078125 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(16.5f,2.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //24.------------------------------------------------------------------------------
    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.05;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.5;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.5;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.7;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.078125 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(21.5f,2.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
    //25.------------------------------------------------------------------------------
    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);

    materialDiffused[0] = 0.5;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.4;
    materialDiffused[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.04;
    materialSpecular[3] = 1.0f;
    glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);

    materialShininess = 0.078125 * 128;
    glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(26.0f,2.0f,0.0f);
    gluSphere(quadric[1],1.0f,30,30);
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

	for(int i = 0 ; i < 24 ; i++ )
    {
        if(quadric[i])
        {
            gluDeleteQuadric(quadric[i]);
            quadric[i] = NULL;
        }
    }


	if (gpFile)
	{
		fclose(gpFile);
		gpFile = NULL;
	}

}
