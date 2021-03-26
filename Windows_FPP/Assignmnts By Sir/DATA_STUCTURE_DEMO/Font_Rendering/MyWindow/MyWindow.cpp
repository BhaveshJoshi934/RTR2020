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

bool bDone = 0;

GLfloat x = 1.8f;
GLfloat y = 0.0f;
GLfloat z = 20.0f;

	HFONT hVertexFont;
	GLuint uiFontList;
    GLYPHMETRICSFLOAT agmf[128];

bool DeleteFont = 0;
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

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, TEXT("Font Rendering!!!"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, X, Y, WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);

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

            case 'E':
            case 'e':
                bDone = 1;
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


	//initialization for font
	LOGFONT logFont;
	logFont.lfHeight = 60;
	logFont.lfWidth = 30;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_BOLD;
	logFont.lfItalic = FALSE;
	logFont.lfUnderline = FALSE;
	logFont.lfStrikeOut = FALSE;
	logFont.lfCharSet = ANSI_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = DEFAULT_QUALITY;
	logFont.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(logFont.lfFaceName,"Optima");

	//create the font and display list
	hVertexFont = CreateFontIndirect(&logFont);

	SelectObject(ghdc, hVertexFont);
	uiFontList = glGenLists(128);
	//wglUseFontOutlines(ghdc, 0, 128, uiFontList, 0.0f, 0.5f, WGL_FONT_POLYGONS, agmf);
	wglUseFontBitmaps(ghdc, 0, 128, uiFontList);

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
    static GLfloat delay = 30.0f;

	void DrawFont_AMC(void);
	void DrawFont_FRAG(void);
	void DrawFont_AMC_Disable(void);
	void DrawFont_FRAG_Disable(void);
	void DrawFont_Credit(void);
	void DrawFont_Credit_Disable(void);
	void DrawFont_Credit_Special(void);
	void DrawFont_Credit_Special_Disable(void);
	void DrawFont_Technical_Details(void);
	void DrawFont_Technical_Details_Disable(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

      if(delay <= 30.0f)
      {
        DrawFont_AMC();
      }
     if(delay <= 25.0f)
     {
         DrawFont_AMC_Disable();
     }
    if(delay <= 20.0f)
    {
        DrawFont_Technical_Details();
    }
    if(delay <= 15.0f)
    {
        DrawFont_Technical_Details_Disable();
    }
	if(delay <= 10.0f)
    {
       DrawFont_FRAG();
    }
    if(delay <= 5.0f)
    {
        DrawFont_FRAG_Disable();
    }

if(bDone == 1)
{
    static GLfloat delay_2 = 30.0f;
    if(delay_2 <= 30.0f)
    {
        DrawFont_Credit();
    }
    if(delay_2 <= 25.0f)
    {
        DrawFont_Credit_Disable();
    }
    if(delay_2 <= 20.0f)
    {
        DrawFont_Credit_Special();
    }
    if(delay_2 <= 15.0f)
    {
        DrawFont_Credit_Special_Disable();
    }

    delay_2 = delay_2 - 0.01f;
}

    delay = delay - 0.01f;


	//glFlush();
	SwapBuffers(ghdc);
}

void DrawFont_AMC(void)
{
    static GLfloat a,b = 0.0f;
    static GLfloat x   = 1.0f;
    static GLfloat y  =  0.5f;
    bool Done = 0;

	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glColor3f(1.0f,1.0f,1.0f);

	glRasterPos3f(0.0f, 2.0f, 0.0f);
	glListBase(uiFontList);

    glCallLists(15, GL_UNSIGNED_BYTE, "ASTROMEDICOMP'S");

    if(DeleteFont == 1)
    {
        DeleteObject(hVertexFont);
    }
}

void DrawFont_FRAG(void)
{
    static GLfloat c,d = 0.0f;

	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos3f(0.25f, -3.0f, 0.0f);

    glCallLists(14, GL_UNSIGNED_BYTE, "FRAGMENT GROUP");

	glRasterPos3f(2.0f, -4.0f, 0.0f);

    glCallLists(8, GL_UNSIGNED_BYTE, "PRESENTS");

    if(DeleteFont == 1)
    {
        DeleteObject(hVertexFont);
    }
}

void DrawFont_AMC_Disable(void)
{
    static GLfloat a,b = 1.0f;

	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);

	glRasterPos3f(0.0f, 2.0f, 0.0f);
	glListBase(uiFontList);

    glCallLists(15, GL_UNSIGNED_BYTE, "ASTROMEDICOMP'S");

    DeleteObject(hVertexFont);
}

void DrawFont_FRAG_Disable(void)
{
    static GLfloat c,d = 0.0f;

	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

	glRasterPos3f(0.25f, -3.0f, 0.0f);

    glCallLists(14, GL_UNSIGNED_BYTE, "FRAGMENT GROUP");

	glRasterPos3f(2.0f, -4.0f, 0.0f);
    glCallLists(8, GL_UNSIGNED_BYTE, "PRESENTS");

    DeleteObject(hVertexFont);
}

void DrawFont_Technical_Details(void)
{
	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos3f(-3.0f, 5.0f, 0.0f);

    glCallLists(19, GL_UNSIGNED_BYTE, "Technical Details -");

	glRasterPos3f(-1.0f, 4.0f, 0.0f);

    glCallLists(26, GL_UNSIGNED_BYTE, "Programming Language - C++");

	glRasterPos3f(-1.0f, 3.0f, 0.0f);

    glCallLists(29, GL_UNSIGNED_BYTE, "Rendering Technology - OpenGL");

	glRasterPos3f(-1.0f, 2.0f, 0.0f);

    glCallLists(18, GL_UNSIGNED_BYTE, "Platform - Windows");

	glRasterPos3f(-3.0f, -2.0f, 0.0f);

    glCallLists(16, GL_UNSIGNED_BYTE, "Music Credits - ");

	glRasterPos3f(-1.0f, -3.0f, 0.0f);

    glCallLists(13, GL_UNSIGNED_BYTE, "Silly Chicken");

	glRasterPos3f(-1.0f, -4.0f, 0.0f);

    glCallLists(43, GL_UNSIGNED_BYTE, "https://www.youtube.com/watch?v=W16EsK4JCKM");

    DeleteObject(hVertexFont);
}

void DrawFont_Technical_Details_Disable(void)
{
	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);

	glRasterPos3f(-3.0f, 5.0f, 0.0f);

    glCallLists(19, GL_UNSIGNED_BYTE, "Technical Details -");

	glRasterPos3f(-1.0f, 4.0f, 0.0f);

    glCallLists(26, GL_UNSIGNED_BYTE, "Programming Language - C++");

	glRasterPos3f(-1.0f, 3.0f, 0.0f);

    glCallLists(29, GL_UNSIGNED_BYTE, "Rendering Technology - OpenGL");

	glRasterPos3f(-1.0f, 2.0f, 0.0f);

    glCallLists(18, GL_UNSIGNED_BYTE, "Platform - Windows");

	glRasterPos3f(-3.0f, -2.0f, 0.0f);

    glCallLists(16, GL_UNSIGNED_BYTE, "Music Credits - ");

	glRasterPos3f(-1.0f, -3.0f, 0.0f);

    glCallLists(13, GL_UNSIGNED_BYTE, "Silly Chicken");

	glRasterPos3f(-1.0f, -4.0f, 0.0f);

    glCallLists(43, GL_UNSIGNED_BYTE, "https://www.youtube.com/watch?v=W16EsK4JCKM");

    DeleteObject(hVertexFont);
}

void DrawFont_Credit(void)
{
	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos3f(-2.0f,1.0f,0.0f);

    glCallLists(15, GL_UNSIGNED_BYTE, "Developed By  -");

	glRasterPos3f(5.0f, 1.0f, 0.0f);

    glCallLists(13, GL_UNSIGNED_BYTE, "Bhavesh Joshi");

	glRasterPos3f(-2.0f, -1.0f, 0.0f);

    glCallLists(15, GL_UNSIGNED_BYTE, "Group Leader  -");

	glRasterPos3f(5.0f, -1.0f, 0.0f);

    glCallLists(11, GL_UNSIGNED_BYTE, "Ajay Ambure");

    DeleteObject(hVertexFont);
}

void DrawFont_Credit_Disable(void)
{
	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);

	glRasterPos3f(-2.0f,1.0f,0.0f);

    glCallLists(15, GL_UNSIGNED_BYTE, "Developed By  -");

	glRasterPos3f(5.0f, 1.0f, 0.0f);

    glCallLists(13, GL_UNSIGNED_BYTE, "Bhavesh Joshi");

	glRasterPos3f(-2.0f, -1.0f, 0.0f);

    glCallLists(15, GL_UNSIGNED_BYTE, "Group Leader  -");

	glRasterPos3f(5.0f, -1.0f, 0.0f);

    glCallLists(11, GL_UNSIGNED_BYTE, "Ajay Ambure");

    DeleteObject(hVertexFont);
}

void DrawFont_Credit_Special(void)
{
	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos3f(-3.0f, 2.0f, 0.0f);

    glCallLists(14, GL_UNSIGNED_BYTE, "Dedicated To -");

	glRasterPos3f(-1.0f, 0.0f, 0.0f);

    glCallLists(18, GL_UNSIGNED_BYTE, "Sharada Joshi(Mom)");

	glRasterPos3f(-0.1f, -1.0f, 0.0f);

    glCallLists(16, GL_UNSIGNED_BYTE, "Dilip Joshi(Dad)");

	glRasterPos3f(2.5f, -2.0f, 0.0f);

    glCallLists(3, GL_UNSIGNED_BYTE, "AND");

	glRasterPos3f(-1.0f, -3.0f, 0.0f);

    glCallLists(21, GL_UNSIGNED_BYTE, "Dr. Vijay Gokhale Sir");

    DeleteObject(hVertexFont);
}

void DrawFont_Credit_Special_Disable(void)
{
	//code
	glLoadIdentity();

	glTranslatef(-3.5f, 0.0f, -15.0f);
    //gluLookAt(x,y,z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);

	glRasterPos3f(-3.0f, 2.0f, 0.0f);

    glCallLists(14, GL_UNSIGNED_BYTE, "Dedicated To -");

	glRasterPos3f(-1.0f, 0.0f, 0.0f);

    glCallLists(18, GL_UNSIGNED_BYTE, "Sharada Joshi(Mom)");

	glRasterPos3f(-0.1f, -1.0f, 0.0f);

    glCallLists(16, GL_UNSIGNED_BYTE, "Dilip Joshi(Dad)");

	glRasterPos3f(2.5f, -2.0f, 0.0f);

    glCallLists(3, GL_UNSIGNED_BYTE, "AND");

	glRasterPos3f(-1.0f, -3.0f, 0.0f);

    glCallLists(21, GL_UNSIGNED_BYTE, "Dr. Vijay Gokhale Sir");

    DeleteObject(hVertexFont);
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
