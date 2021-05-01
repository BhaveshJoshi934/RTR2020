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

bool gbLight = false;

GLfloat lightAmbient_one[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffused_one[] = {1.0f,0.0f,0.0f,1.0f};
GLfloat lightSpecular_one[] = {1.0f,0.0f,0.0f,1.0f};
GLfloat lightPosition_one[] = {0.0f,0.0f,0.0f,1.0f};

GLfloat lightAmbient_two[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffused_two[] = {0.0f,1.0f,0.0f,1.0f};
GLfloat lightSpecular_two[] = {0.0f,1.0f,0.0f,1.0f};
GLfloat lightPosition_two[] = {0.0f,0.0f,0.0f,1.0f};

GLfloat lightAmbient_three[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffused_three[] = {0.0f,0.0f,1.0f,1.0f};
GLfloat lightSpecular_three[] = {0.0f,0.0f,1.0f,1.0f};
GLfloat lightPosition_three[] = {0.0f,0.0f,0.0f,1.0f};

GLfloat materialAmbient[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat materialDiffused[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat materialSpecular[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat materialShininess = 50.0f;

GLfloat LightAngle_one = 0.0f;
GLfloat LightAngle_two = 0.0f;
GLfloat LightAngle_three = 0.0f;

GLXContext gGLXContext;

GLUquadric *quadric = NULL;

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

							case XK_L:
							case XK_l:
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

	XStoreName(gpDisplay,gWindow,"2 Lights on Pyramid : Bhavesh Joshi!!");

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

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient_one);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffused_one);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular_one);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition_one);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1,GL_AMBIENT,lightAmbient_two);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDiffused_two);
	glLightfv(GL_LIGHT1,GL_SPECULAR,lightSpecular_two);
	glLightfv(GL_LIGHT1,GL_POSITION,lightPosition_two);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2,GL_AMBIENT,lightAmbient_three);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,lightDiffused_three);
	glLightfv(GL_LIGHT2,GL_SPECULAR,lightSpecular_three);
	glLightfv(GL_LIGHT2,GL_POSITION,lightPosition_three);

	glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);
	glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
	glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

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
	void Cube();
	void Pyramid();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	gluLookAt(0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

	glPushMatrix();

	glRotatef(LightAngle_one,1.0f,0.0f,0.0f);
	lightPosition_one[1] = LightAngle_one;
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition_one);

	glPopMatrix();

	glPushMatrix();

	glRotatef(LightAngle_two,0.0f,1.0f,0.0f);
	lightPosition_two[0] = LightAngle_two;
	glLightfv(GL_LIGHT1,GL_POSITION,lightPosition_two);

	glPopMatrix();

	glPushMatrix();

	glRotatef(LightAngle_three,0.0f,0.0f,1.0f);
	lightPosition_three[0] = LightAngle_three;
	glLightfv(GL_LIGHT3,GL_POSITION,lightPosition_three);

	glPopMatrix();
	
	glTranslatef(0.0f,0.0f,-1.0f);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	quadric = gluNewQuadric();
	gluSphere(quadric,0.3f,30,30);

	glPopMatrix();

	LightAngle_one = LightAngle_one + 1.0f;
	if(LightAngle_one >= 360.0f)
	{
		LightAngle_one = 0.0f;
	}

	LightAngle_two = LightAngle_two + 1.0f;
	if(LightAngle_two >= 360.0f)
	{
		LightAngle_two = 0.0f;
	}

	LightAngle_three = LightAngle_three + 1.0f;
	if(LightAngle_three >= 360.0f)
	{
		LightAngle_three = 0.0f;
	}

	glXSwapBuffers(gpDisplay,gWindow);
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
