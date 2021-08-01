//Header file inclusions
#include<windows.h>
#include<stdio.h>
#include<gl/gl.h>
//#include"icon.h"
#include<gl/glu.h>

#define _USE_MATH_DEFINES
#include<math.h>

//libraries to link
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "glu32.lib")

//macro declarations
#define WINWIDTH 		800
#define WINHEIGHT 		600
#define TIMER1_MSDELAY  5.0
#define TIMER2_MSDELAY 	1.0
#define TIMER3_MSDELAY  0.3

#define IDT_TIMER1  	(UINT_PTR)1000    //timer for background blinking
#define IDT_TIMER2		(UINT_PTR)1001	  //timer for peek frame animation
#define IDT_TIMER3		(UINT_PTR)1002    //timer for ongoing operation message display
//global variables declarations
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
bool gbFullScreen = false;
HWND  ghwnd = NULL;

FILE *solarFile;

bool gbActiveWindow = false;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

int SPV_iScreenWidth;
int SPV_iScreenHeight;

//new variables for planet rotation
float day = 0;
float year = 0;
float moonRotation = 0;
float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;

GLUquadric *quadric = NULL;

//global callback function declarations
LRESULT CALLBACK wndproc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function prototype declarations
	void initialize(void);
	void display(void);

	//local variable declarations
	WNDCLASSEX SPV_wndClass;
	MSG SPV_msg;
	TCHAR SPV_lpszClassName[] = "SolarSystem";
	bool bDone = false;
	int SPV_iXposition;
	int SPV_iYposition;

	//code
	//debugging file open
	if(fopen_s(&solarFile, "SolarSustemDebug.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("File creation error"), TEXT("ERROR"), MB_OK);
		exit(1);
	}

	SPV_iScreenWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	SPV_iScreenHeight = GetSystemMetrics(SM_CYSCREEN) / 2;

	SPV_iXposition = SPV_iScreenWidth - (WINWIDTH / 2);
	SPV_iYposition = SPV_iScreenHeight - (WINHEIGHT / 2);

	//wndclass parameters initialization
	SPV_wndClass.cbSize = sizeof(WNDCLASSEX);
	SPV_wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	SPV_wndClass.cbClsExtra = 0;
	SPV_wndClass.cbWndExtra = 0;
	SPV_wndClass.lpfnWndProc = wndproc;
	SPV_wndClass.hInstance = hInstance;
	SPV_wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	SPV_wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	SPV_wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	SPV_wndClass.lpszClassName = SPV_lpszClassName;
	SPV_wndClass.lpszMenuName = NULL;
	SPV_wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register the class
	RegisterClassEx(&SPV_wndClass);

	ghwnd = CreateWindowEx(WS_EX_APPWINDOW,
			SPV_lpszClassName,
			TEXT("SolarSystem"),
			(WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE),
			SPV_iXposition,
			SPV_iYposition,
			WINWIDTH,
			WINHEIGHT,
			NULL,
			NULL,
			hInstance,
			NULL);

	initialize();
	ShowWindow(ghwnd, iCmdShow);
	SetForegroundWindow(ghwnd);
	SetFocus(ghwnd);

	//game loop
	while(bDone == false)
	{
		if(PeekMessage(&SPV_msg, NULL, 0, 0, PM_REMOVE))
		{
			if(SPV_msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&SPV_msg);
				DispatchMessage(&SPV_msg);
			}
		}
		else
		{
			if(gbActiveWindow)
			{
				display();
			}
		}
	}

	return((int)SPV_msg.wParam);
}

LRESULT CALLBACK wndproc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function prototype
	void toggleFullScreen(void);
	void resize(int, int);
	void uninitialize(void);

	//code
	switch(iMsg)
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
			switch(wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hwnd);
					break;
				case 0x46:
				case 0x66:
					toggleFullScreen();
					break;
				default:
					break;
			}
			break;

		case WM_TIMER:
			switch(wParam)
			{
				case IDT_TIMER1:
					KillTimer(hwnd, IDT_TIMER1);
					year = year + 0.5f;
					if(year > 360.0f)
						year = 0.0f;
					SetTimer(hwnd, IDT_TIMER1, TIMER1_MSDELAY, NULL);
					break;
				case IDT_TIMER2:
					KillTimer(hwnd, IDT_TIMER2);
					day = day + 0.5f;
					if(day > 360.0f)
						day = 0.0f;
					SetTimer(hwnd, IDT_TIMER2, TIMER2_MSDELAY, NULL);
					break;
				case IDT_TIMER3:
					KillTimer(hwnd, IDT_TIMER3);
					moonRotation = moonRotation + 1.5f;
					if(moonRotation > 360.0f)
						moonRotation = 0.0f;
					SetTimer(hwnd, IDT_TIMER3, TIMER3_MSDELAY, NULL);
					break;
				default:
					break;
			}
			break;

		case WM_CHAR:
			switch(wParam)
			{
			case 'S':
				SetTimer(hwnd, IDT_TIMER1, TIMER1_MSDELAY, NULL);
				SetTimer(hwnd, IDT_TIMER2, TIMER2_MSDELAY, NULL);
				SetTimer(hwnd, IDT_TIMER3, TIMER3_MSDELAY, NULL);
				break;

			case 's':
				KillTimer(hwnd, IDT_TIMER1);
				KillTimer(hwnd, IDT_TIMER2);
				KillTimer(hwnd, IDT_TIMER3);
				break;

			case 'D':
				//day = (day + 6) % 360;
				day = day + 3.6f;
				if(day > 360.0f)
					day = 0.0f;
				break;
			case 'd':
				//day = (day - 6) % 360;
				break;
			case 'Y':
				//year = (year + 3) % 360;
				year = year + 1.0f;
				if(year > 360.0f)
					year = 0.0f;
				break;
			case 'y':
				//year = (year - 3) % 360;
				break;
			case 'L':
				//moonRotation = (moonRotation + 4) % 360;
				moonRotation = moonRotation + 0.5f;
				if(moonRotation > 360.0f)
					moonRotation = 0.0f;
				break;
			case 'l':
				//moonRotation = (moonRotation - 4) % 360;
				break;
			case 'b':
				camX = camX + 0.1f;
				if(camX >= 1.0f)
					camX = 1.0f;
				break;
			case 'n':
				camY = camY + 0.1f;
				if(camY >= 1.0f)
					camY = 1.0f;
				break;
			case 'm':
				camZ = camZ + 0.1f;
				if(camZ >= 1.0f)
					camZ = 1.0f;
				break;
			case 'B':
				camX = camX - 0.1f;
				if(camX <= -1.0f)
					camX = -1.0f;
				break;
			case 'N':
				camY = camY - 0.1f;
				if(camY <= -1.0f)
					camY = -1.0f;
				break;
			case 'M':
				camZ = camZ - 0.1f;
				if(camZ <= -1.0f)
					camZ = -1.0f;
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

void toggleFullScreen(void)
{
	//local variables declarations
	MONITORINFO SPV_mi = {sizeof (MONITORINFO)};

	//code
	if(gbFullScreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if(dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if(GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &SPV_mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, (dwStyle & ~(WS_OVERLAPPEDWINDOW)));
				SetWindowPos(ghwnd,
							HWND_TOP,
							SPV_mi.rcMonitor.left,
							SPV_mi.rcMonitor.top,
							(SPV_mi.rcMonitor.right - SPV_mi.rcMonitor.left),
							(SPV_mi.rcMonitor.bottom - SPV_mi.rcMonitor.top),
							(SWP_NOZORDER | SWP_FRAMECHANGED));
			}
		}

		ShowCursor(FALSE);
		gbFullScreen = true;
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, (dwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
					HWND_TOP,
					0,
					0,
					0,
					0,
					(SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOOWNERZORDER));

		ShowCursor(TRUE);
		gbFullScreen = false;
	}
}

void initialize(void)
{
	//function prototype
	void resize(int, int);

	//local variable declarations
	PIXELFORMATDESCRIPTOR spv_pfd;
	int SPV_iPixelFormatIndex;

	//code
	ghdc = GetDC(ghwnd);
	ZeroMemory(&spv_pfd, sizeof(PIXELFORMATDESCRIPTOR));
	spv_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	spv_pfd.nVersion = 1;
	spv_pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	spv_pfd.iPixelType = PFD_TYPE_RGBA;
	spv_pfd.cColorBits = 32;
	spv_pfd.cRedBits = 8;
	spv_pfd.cGreenBits = 8;
	spv_pfd.cBlueBits = 8;
	spv_pfd.cAlphaBits = 8;

	SPV_iPixelFormatIndex = ChoosePixelFormat(ghdc, &spv_pfd);
	if(SPV_iPixelFormatIndex == 0)
	{
		fprintf(solarFile, "Error: ChoosePixelFormat function failed \n");
		DestroyWindow(ghwnd);
	}

	if(SetPixelFormat(ghdc, SPV_iPixelFormatIndex, &spv_pfd) == FALSE)
	{
		fprintf(solarFile, "Error: SetPixelFormat function failed \n");
		DestroyWindow(ghwnd);
	}

	ghrc = wglCreateContext(ghdc);
	if(ghrc == NULL)
	{
		fprintf(solarFile, "Error: wglCreateContext function failed \n");
		DestroyWindow(ghwnd);
	}

	if(wglMakeCurrent(ghdc, ghrc) == NULL)
	{
		fprintf(solarFile, "Error: wglMakeCurrent function failed \n");
		DestroyWindow(ghwnd);
	}

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	resize(WINWIDTH, WINHEIGHT);
}


void resize(int width, int height)
{
	//local variables
	GLfloat glf_height;
	GLfloat glf_width;
	GLfloat fovY = 45.0f;
	GLfloat zNear = 0.1f;
	GLfloat zFar = 100.0f;

	//code
	if(height == 0)
		height = 1;

	glf_height = tan ((fovY / 2.0f) * (M_PI / 180.0f)) * zNear;
	glf_width = glf_height * ((GLfloat)width / (GLfloat)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-glf_width, glf_width, -glf_height, glf_height, zNear, zFar);
	glViewport(0,0,(GLsizei)width, (GLsizei)height);

	// if (height == 0)
	// 	height = 1;
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	// glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}


void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
			  0.0f, 0.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	glPushMatrix(); // save current matrix into a stack and do further changes in copy of current matrix

	glBegin(GL_QUADS);
	{
		// //front. color -red
		// glColor3f(0.6f, 0.6f, 0.4f);
		// glVertex3f(4.0f, 4.0f, 4.0f);
		// glVertex3f(-4.0f, 4.0f, 4.0f);
		// glVertex3f(-4.0f, -4.0, 4.0f);
		// glVertex3f(4.0f, -4.0f, 4.0f);

		// //right. color -green
		// glColor3f(0.6f, 0.4f, 0.6f);
		// glVertex3f(4.0f, 4.0f, -4.0f);
		// glVertex3f(4.0f, 4.0f, 4.0f);
		// glVertex3f(4.0f, -4.0, 4.0f);
		// glVertex3f(4.0f, -4.0f, -4.0f);

		// //back. color -blue
		// glColor3f(0.6f, 0.6f, 0.9f);
		// glVertex3f(-4.0f, 4.0f, -4.0f);
		// glVertex3f(4.0f, 4.0f, -4.0f);
		// glVertex3f(4.0f, -4.0, -4.0f);
		// glVertex3f(-4.0f, -4.0f, -4.0f);

		// //left. color -cyan
		// glColor3f(0.7f, 0.6f, 0.4f);
		// glVertex3f(-4.0f, 4.0f, 4.0f);
		// glVertex3f(-4.0f, 4.0f, -4.0f);
		// glVertex3f(-4.0f, -4.0, -4.0f);
		// glVertex3f(-4.0f, -4.0f, 4.0f);

		// //top. color -megenta
		// glColor3f(0.7f, 0.9f, 0.6f);
		// glVertex3f(4.0f, 4.0f, -4.0f);
		// glVertex3f(-4.0f, 4.0f, -4.0f);
		// glVertex3f(-4.0f, 4.0, 4.0f);
		// glVertex3f(4.0f, 4.0f, 4.0f);

		// //bottom. color -bottom
		// glColor3f(0.9f, 0.8f, 0.9f);
		// glVertex3f(4.0f, -4.0f, -4.0f);
		// glVertex3f(-4.0f, -4.0f, -4.0f);
		// glVertex3f(-4.0f, -4.0, 4.0f);
		// glVertex3f(4.0f, -4.0f, 4.0f);
		//front. color -red
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		//right. color -green
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		//back. color -blue
		glColor3f(0.2f, 0.2f, 0.2f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);

		//left. color -cyan
		glColor3f(0.1f, 0.1f, 0.1f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);

		//top. color -megenta
		glColor3f(0.6f, 0.6f, 0.6f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

		//bottom. color -bottom
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

	}
	glEnd();
	//preperation for sun
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);   //Y axis of quadric is along z axis of the global z axis of window, to make along y axis need to rotate along x-axis by 90
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // to apply filled color on surface of sphere , for wireframe use GL_LINES
	glColor3f(1.0f, 0.5f, 0.0f);

	quadric = gluNewQuadric();
	gluSphere(quadric, 0.10f, 30, 30);

	glPopMatrix();  //relese current matrix and pop saved matrix from stack and operate on that
	glPushMatrix(); //save matrix to stack before applying changes on matrix

	glRotatef((GLfloat)year,  0.0f, 1.0f, 0.0f);  //rotate planet along y axis first
	glTranslatef(0.6f, 0.0f, 0.0f); // translate towards +x axis  by 1.0f
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);   //rotate sphere by 90 along x axis to make sphere y axis along global y axis of screen

	glPushMatrix();
	glRotatef((GLfloat)day, 0.0f, 0.0f, 1.0f); //rotate sphere to its own
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0f, 0.6f , 1.0f);

	quadric = gluNewQuadric();
	gluSphere(quadric, 0.03f, 20, 20);
	glPopMatrix();

	glRotatef((GLfloat)moonRotation, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.1f, 0.0f, 0.0f);
	glRotatef((GLfloat)moonRotation, 0.0f, 0.0f, 1.0f); //rotate sphere to its own
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.9f, 0.9f , 0.9f);

	quadric = gluNewQuadric();
	gluSphere(quadric, 0.02f, 20, 20);
	glPopMatrix();

	SwapBuffers(ghdc);
}

void uninitialize(void)
{
	if(gbFullScreen)
	{
		SetWindowLong(ghwnd, GWL_STYLE, (dwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
					HWND_TOP,
					0,
					0,
					0,
					0,
					(SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOOWNERZORDER));

		ShowCursor(TRUE);
	}

	gluDeleteQuadric(quadric);

	if(wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if(ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if(ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if(solarFile)
	{
		fprintf(solarFile, "Program execution is completed \n");
		fclose(solarFile);
		solarFile = NULL;
	}
}
