#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>

#include<GL/gl.h>
#include<GL/glx.h>

#include<GL/glu.h>

using namespace std;

bool bFullScreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL; 
Colormap gColormap;
Window gWindow;
int giWindowWidth = 800;
int giWindowHeight = 600;

GLXContext gGLXContext;

bool flag = false;
 
int main(void)
{
	void CreateWindow(void);
	void ToggleFullscreen(void);
	void unInitialize(void);

	void Initialize(void);
	void Resize(int,int);
	void Draw(void);
	
	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;
	bool bDone = false;

	CreateWindow();

	Initialize();

	XEvent event;
	KeySym keysym;

	while(bDone == false)
	{
		while(XPending(gpDisplay))
			{
				XNextEvent(gpDisplay,&event);
				switch(event.type)
				{
					case MapNotify:
						break;
					case KeyPress:
						keysym = XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);
						switch(keysym)
						{
							case XK_Escape:
								bDone = true;
								break;
						
							case XK_F:
							case XK_f:
								if(bFullScreen == false)
								{
									ToggleFullscreen();
									bFullScreen = true;
								}
								else
								{
									ToggleFullscreen();
									bFullScreen = false;
								}
								break;
							
							default:
								break;

						}
						break;
					case ButtonPress:
						switch(event.xbutton.button)
						{
							case 1:
								break;
							case 2:
								break;
							case 3:
								break;
							default:
								break;
						}
						break;
					case MotionNotify:
						break;
					case ConfigureNotify:
						winWidth = event.xconfigure.width;
						winHeight = event.xconfigure.height;
						Resize(winWidth,winHeight);
						break;
					case Expose:
						break;
					case DestroyNotify:
						break;
					case 33:
						bDone = true;
						break;
					default:
						break;
				}
			}
		Draw();
	}
	unInitialize();
	return(0);
}

void CreateWindow()
{
	void unInitialize(void);

	XSetWindowAttributes winAttribs;
	int defaultScreen;
	int defaultDepth;
	int styleMask;

	static int FrameBufferAttributes[] = {
						GLX_DOUBLEBUFFER,True,	
						GLX_RGBA,
						GLX_RED_SIZE,8,
						GLX_GREEN_SIZE,8,
						GLX_BLUE_SIZE,8,
						GLX_ALPHA_SIZE,8,
						GLX_DEPTH_SIZE,24,
						None
					     };

	gpDisplay = XOpenDisplay(NULL);
	if(gpDisplay == NULL)
	{
		printf("ERROR : Unable To Open X Display.\nExitting Now!!!\n");
		unInitialize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(gpDisplay);

	gpXVisualInfo = glXChooseVisual(gpDisplay,defaultScreen,FrameBufferAttributes);

	winAttribs.border_pixel = 0;
	winAttribs.background_pixmap = 0;
	winAttribs.colormap = XCreateColormap(gpDisplay,
					      RootWindow(gpDisplay,gpXVisualInfo->screen),
					      gpXVisualInfo->visual,
					      AllocNone);
	gColormap = winAttribs.colormap;
	winAttribs.background_pixel = BlackPixel(gpDisplay,defaultScreen);
	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
	styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;

	gWindow = XCreateWindow(gpDisplay,
				RootWindow(gpDisplay,gpXVisualInfo->screen),
				0,
				0,
				giWindowWidth,
				giWindowHeight,
				0,
				gpXVisualInfo->depth,
				InputOutput,
				gpXVisualInfo->visual,
				styleMask,
				&winAttribs);

	if(!gWindow)
	{
		printf("ERROR : Failed To Create Main Window \nExiting Now!!!\n");
		unInitialize();
		exit(1);
	}

	XStoreName(gpDisplay,gWindow,"Static India : Bhavesh Joshi!!");

	Atom windowManagerDelete = XInternAtom(gpDisplay,"WM_DELETE_WINDOW",True);
	XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);

	XMapWindow(gpDisplay,gWindow);
}

void ToggleFullscreen(void)
{
	Atom wm_state;
	Atom fullscreen;
	XEvent xev = {0};

	wm_state = XInternAtom(gpDisplay,"_NET_WM_STATE",False);
	memset(&xev,0,sizeof(xev));

	xev.type = ClientMessage;
	xev.xclient.window = gWindow;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = bFullScreen ? 0 : 1;

	fullscreen = XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",False);
	xev.xclient.data.l[1] = fullscreen;

	XSendEvent(gpDisplay,
			RootWindow(gpDisplay,gpXVisualInfo->screen),
			False,
			StructureNotifyMask,
			&xev);
}

void Initialize(void)
{
	void Resize(int,int);

	gGLXContext = glXCreateContext(gpDisplay,
					gpXVisualInfo,
					NULL,
					GL_TRUE);

	glXMakeCurrent(gpDisplay,gWindow,gGLXContext);

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	glClearColor(0.0f,0.0f,0.0f,0.0f);

	Resize(giWindowWidth,giWindowHeight);
}

void Resize(int width,int height)
{
	if(height == 1)
		height = 0;

	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}


void Draw(void)
{
	GLfloat time  =  0.1f;
	static GLfloat delay = 60.0f;

	static GLfloat i_bdj = -30.0f;
	static GLfloat a_bdj = 30.0f;
	static GLfloat n_bdj = 20.0f;
	static GLfloat I_bdj = -20.0f;
	static GLfloat p_bdj = -30.0f;
	static GLfloat py_bdj = -10.0f;

	void I();
	void N();
	void D();
	void A();
	void Planes(void);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

if(delay <= 55.0f)
{
	if(i_bdj <= -14.9f)
	{	
		glTranslatef(i_bdj,0.0f,-30.0f);
		I();
	}
	else
	{
		glTranslatef(-15.0f,0.0f,-30.0f);
		I();
	}

	i_bdj = i_bdj + 0.1f;
}

if(delay <= 35.0f)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(n_bdj >= 0.1f)
	{
		glTranslatef(-7.5f,n_bdj,-30.0f);
		N();
	}
	else
	{
		glTranslatef(-7.5f,0.0f,-30.0f);
		N();
	}
	n_bdj = n_bdj - 0.1f;
}
if(delay <= 15.0f)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-30.0f);
	D();
}
if(delay <= 25.0f)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(I_bdj <= -0.1f)
	{
		glTranslatef(7.5f,I_bdj,-30.0f);
		I();
	}
	else
	{
		glTranslatef(7.5f,0.0f,-30.0f);
		I();
	}

	I_bdj = I_bdj + 0.1f;
}
if(delay <= 45.0f)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if(a_bdj >= 15.1f)
	{
		glTranslatef(a_bdj,0.0f,-30.0f);	
		A();
	}
	else
	{
		glTranslatef(15.0f,0.0f,-30.0f);
		A();
	}

	a_bdj = a_bdj - 0.1f;
}

	delay = delay - time;

if(delay <= 5.0f)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(p_bdj <= 30.0f)
	{
		glTranslatef(p_bdj,0.0f,-30.0f);
		glScalef(0.50f,0.50f,0.50f);
		Planes();
	}
	p_bdj = p_bdj + 0.1f;
*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static GLfloat AngleUp = 45.0f;
	if(py_bdj >= 0.0f)
	{
		glTranslatef(0.0f,0.0f,-30.0f);
	}

	glScalef(0.50f,0.50f,0.50f);
	if(AngleUp >= 0.0f)
	{
		glRotatef(AngleUp,0.0f,0.0f,1.0f);
	}
	Planes();
	AngleUp = AngleUp - 0.1f;
	p_bdj = p_bdj + 0.1f;
	py_bdj = py_bdj - 0.1f;
//}

	glXSwapBuffers(gpDisplay,gWindow);
}

void Planes()
{
//	glTranslatef(0.0f,0.0f,-30.0f);

	glColor3f(0,0,128);

	glBegin(GL_TRIANGLES);
	glVertex3f(8.0f,0.0f,0.0f);
	glVertex3f(5.0f,1.0f,0.0f);
	glVertex3f(5.0f,-1.0f,0.0f);

	glVertex3f(5.0f,1.0f,0.0f);
	glVertex3f(5.0f,-1.0f,0.0f);
	glVertex3f(3.0f,0.0f,0.0f);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3f(5.0f,1.0f,0.0f);
	glVertex3f(1.0f,2.0f,0.0f);
	glVertex3f(5.0f,-1.0f,0.0f);
	glVertex3f(1.0f,-2.0f,0.0f);

	glVertex3f(1.0f,2.0f,0.0f);
	glVertex3f(-3.0f,5.0f,0.0f);
	glVertex3f(-4.0f,5.0f,0.0f);
	glVertex3f(-4.0f,2.0f,0.0f);

	glVertex3f(1.0f,-2.0f,0.0f);
	glVertex3f(-3.0f,-5.0f,0.0f);
	glVertex3f(-4.0f,-5.0f,0.0f);
	glVertex3f(-4.0f,-2.0f,0.0f);

	glVertex3f(1.0f,2.0f,0.0f);
	glVertex3f(-6.0f,2.0f,0.0f);
	glVertex3f(-6.0f,-2.0f,0.0f);
	glVertex3f(1.0f,-2.0f,0.0f);

	glColor3f(1.0f,0.5f,0.0f);

	glVertex3f(-6.0f,2.0f,0.0f);
	glVertex3f(-10.0f,2.0f,0.0f);
	glVertex3f(-10.0f,0.66f,0.0f);
	glVertex3f(-6.0f,0.66f,0.0f);

	glColor3f(1.0f,1.0f,1.0f);

	glVertex3f(-6.0f,0.66f,0.0f);
	glVertex3f(-10.0f,0.66f,0.0f);
	glVertex3f(-10.0f,-0.66f,0.0f);
	glVertex3f(-6.0f,-0.66f,0.0f);

	glColor3f(0.0f,1.0f,0.0f);

	glVertex3f(-6.0f,-2.0f,0.0f);
	glVertex3f(-10.0f,-2.0f,0.0f);
	glVertex3f(-10.0f,-0.66f,0.0f);
	glVertex3f(-6.0f,-0.66f,0.0f);

	glEnd();
}

void I()
{
//	glTranslatef(-15.0f,0.0f,-30.0f);

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(2.5f,4.0f,0.0f);
	glVertex3f(-2.5f,4.0f,0.0f);
	glVertex3f(-2.5f,3.0f,0.0f);
	glVertex3f(2.5f,3.0f,0.0f);

	glVertex3f(0.5f,3.0f,0.0f);
	glVertex3f(-0.5f,3.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(-0.5f,0.0f,0.0f);
	glVertex3f(0.5f,0.0f,0.0f);

	glVertex3f(0.5f,0.0f,0.0f);
	glVertex3f(-0.5f,0.0f,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-0.5f,-3.0f,0.0f);
	glVertex3f(0.5f,-3.0f,0.0f);

	glVertex3f(2.5f,-3.0f,0.0f);
	glVertex3f(-2.5f,-3.0f,0.0f);
	glVertex3f(-2.5f,-4.0f,0.0f);
	glVertex3f(2.5f,-4.0f,0.0f);

	glEnd();
}

void N()
{
//	glTranslatef(-7.5f,0.0f,-30.0f);

	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(1.5f,4.0f,0.0f);
	glVertex3f(0.5f,4.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(0.5f,0.0f,0.0f);
	glVertex3f(1.5f,0.0f,0.0f);

	glVertex3f(1.5f,0.0f,0.0f);
	glVertex3f(0.5f,0.0f,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.5f,-4.0f,0.0f);
	glVertex3f(1.5f,-4.0f,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-2.5f,4.0f,0.0f);
	glVertex3f(-1.5f,4.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(-1.5f,0.0f,0.0f);
	glVertex3f(-2.5f,0.0f,0.0f);

	glVertex3f(-2.5f,0.0f,0.0f);
	glVertex3f(-1.5f,0.0f,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-1.5f,-4.0f,0.0f);
	glVertex3f(-2.5f,-4.0f,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(-2.5f,4.0f,0.0f);
	glVertex3f(-1.5f,4.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(-0.8f,0.0f,0.0f);
	glVertex3f(0.2f,0.0f,0.0f);
	
	glVertex3f(0.2f,0.0f,0.0f);
	glVertex3f(-0.8f,0.0f,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.5f,-4.0f,0.0f);
	glVertex3f(1.5f,-4.0f,0.0f);

	glEnd();
}

void D()
{
	static GLfloat r_bdj = 0.0f;
	static GLfloat go_bdj = 0.0f;
	static GLfloat g_bdj = 0.0f;
	static GLfloat b_bdj = 0.0f;
	static GLfloat w_bdj = 0.0f;

//	glTranslatef(0.0f,0.0f,-30.0f);

	glBegin(GL_QUADS);

	if(r_bdj <= 0.99f && g_bdj <= 0.49f)
	{
		glColor3f(r_bdj,go_bdj,0.0f);
	}
	else
	{
		glColor3f(1.0f,0.5f,0.0f);
	}
	r_bdj = r_bdj + 0.001f;
	go_bdj = go_bdj + 0.001f;

	glVertex3f(1.5f,4.0f,0.0f);
	glVertex3f(-3.5f,4.0f,0.0f);
	glVertex3f(-3.5f,3.0f,0.0f);
	glVertex3f(1.5f,3.0f,0.0f);

	if(g_bdj <= 0.99f)
	{
		glColor3f(0.0f,g_bdj,0.0f);
	}
	else
	{
		glColor3f(0.0f,1.0f,0.0f);
	}
	g_bdj = g_bdj + 0.001f;

	glVertex3f(1.5f,-3.0f,0.0f);
	glVertex3f(-3.5f,-3.0f,0.0f);
	glVertex3f(-3.5f,-4.0f,0.0f);
	glVertex3f(1.5f,-4.0f,0.0f);

	if(r_bdj <= 0.99f && go_bdj <= 0.49f)
	{
		glColor3f(r_bdj,go_bdj,0.0f);
	}
	else
	{
		glColor3f(1.0f,0.5f,0.0f);
	}

	glVertex3f(1.5f,4.0f,0.0f);
	glVertex3f(0.5f,4.0f,0.0f);
	if(w_bdj <= 0.99f)
	{
		glColor3f(w_bdj,w_bdj,w_bdj);
	}
	else
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	w_bdj = w_bdj + 0.001f;

	glVertex3f(0.5f,0.0f,0.0f);
	glVertex3f(1.5f,0.0f,0.0f);

	glVertex3f(1.5f,0.0f,0.0f);
	glVertex3f(0.5f,0.0f,0.0f);
	if(g_bdj <= 0.99f)
	{
		glColor3f(0.0f,g_bdj,0.0f);
	}
	else
	{
		glColor3f(0.0f,1.0f,0.0f);
	}
	glVertex3f(0.5f,-4.0f,0.0f);
	glVertex3f(1.5f,-4.0f,0.0f);

	if(r_bdj <= 0.99f && go_bdj <= 0.49f)
	{
		glColor3f(r_bdj,go_bdj,0.0f);
	}
	else
	{
		glColor3f(1.0f,0.5f,0.0f);
	}
	glVertex3f(-2.5f,4.0f,0.0f);
	glVertex3f(-1.5f,4.0f,0.0f);
	if(w_bdj <= 0.99f)
	{
		glColor3f(w_bdj,w_bdj,w_bdj);
	}
	else
	{
		glColor3f(1.0f,1.0f,1.0f);
	}
	glVertex3f(-1.5f,0.0f,0.0f);
	glVertex3f(-2.5f,0.0f,0.0f);

	glVertex3f(-2.5,0.0f,0.0f);
	glVertex3f(-1.5f,0.0f,0.0f);
	if(g_bdj <= 0.99f)
	{
		glColor3f(0.0f,g_bdj,0.0f);
	}
	else
	{
		glColor3f(0.0f,1.0f,0.0f);
	}
	glVertex3f(-1.5f,-4.0f,0.0f);
	glVertex3f(-2.5f,-4.0f,0.0f);

	glEnd();
}

void A()
{
	glBegin(GL_QUADS);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.0f,4.0f,0.0f);
	glVertex3f(0.0f,2.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(0.5f,0.0f,0.0f);
	glVertex3f(1.5f,0.0f,0.0f);

	glVertex3f(1.5f,0.0f,0.0f);
	glVertex3f(0.5f,0.0f,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(2.0f,-4.0f,0.0f);
	glVertex3f(3.0f,-4.0f,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.0f,4.0f,0.0f);
	glVertex3f(0.0f,2.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(-0.5f,0.0f,0.0f);
	glVertex3f(-1.5f,0.0f,0.0f);

	glVertex3f(-1.5f,0.0f,0.0f);
	glVertex3f(-0.5f,0.0f,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-2.0f,-4.0f,0.0f);
	glVertex3f(-3.0f,-4.0f,0.0f);

	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f(0.5f,0.0f,0.0f);
	glVertex3f(-0.5f,0.0f,0.0f);
	glVertex3f(-0.55f,-0.2f,0.0f);
	glVertex3f(0.55f,-0.2f,0.0f);

	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(0.55f,-0.2f,0.0f);
	glVertex3f(-0.55f,-0.2f,0.0f);
	glVertex3f(-0.65f,-0.4f,0.0f);
	glVertex3f(0.65f,-0.4f,0.0f);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.65f,-0.4f,0.0f);
	glVertex3f(-0.65f,-0.4f,0.0f);
	glVertex3f(-0.73f,-0.6f,0.0f);
	glVertex3f(0.73,-0.6f,0.0f);

	glEnd();
}

void unInitialize(void)
{

	GLXContext CurrentGLXContext;

	CurrentGLXContext = glXGetCurrentContext();

	if(CurrentGLXContext == gGLXContext)
	{
		glXMakeCurrent(gpDisplay,0,0);
	}	

	if(gGLXContext)
	{
		glXDestroyContext(gpDisplay,gGLXContext);
	}


	if(gWindow)
	{
		XDestroyWindow(gpDisplay,gWindow);
	}

	if(gColormap)
	{
		XFreeColormap(gpDisplay,gColormap);
	}

	if(gpXVisualInfo)
	{
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}

	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
}
