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

GLfloat lightAmbient[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffused[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat lightPosition[] = {0.0f,3.0f,3.0f,0.0f};

GLfloat light_Model_Ambient[] = {0.2f,0.2f,0.2f,1.0f};
GLfloat light_Model_Local_Viewer[] = {0.0f};

GLfloat AngleX = 0.0f;
GLfloat AngleY = 0.0f;
GLfloat AngleZ = 0.0f;

GLUquadric *quadric[24];

int KeyPressed = 0;

GLXContext gGLXContext;

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

							case XK_X:
							case XK_x:
								KeyPressed = 1;
								AngleX = 0.0f;
								break;

							case XK_Y:
							case XK_y:
								KeyPressed = 2;
								AngleY = 0.0f;
								break;

							case XK_Z:
							case XK_z:
								KeyPressed = 3;
								AngleZ = 0.0f;
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

	XStoreName(gpDisplay,gWindow,"Twenty-Four Spheres : Bhavesh Joshi!!");

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

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_Model_Ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER,light_Model_Local_Viewer);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffused);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

	for(int i = 0 ; i < 24 ; i++)
	{
		quadric[i] = gluNewQuadric();
	}

	glClearColor(0.25f,0.25f,0.25f,0.0f);

	Resize(giWindowWidth,giWindowHeight);
}

void Resize(int width,int height)
{
	if(height == 1)
		height = 0;

	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if(width <= height)
	{
		glOrtho(0.0f,
			15.5f,
			0.0f,
			15.5f * ((GLfloat)height / (GLfloat)width),
			-10.0f,
			10.0f);
	}

	else
	{
		glOrtho(0.0f,
			15.5f * ((GLfloat)width / (GLfloat)height),
			0.0f,
			15.5f,
			-10.0f,
			10.0f);
	}
}

void Draw(void)
{
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

	glXSwapBuffers(gpDisplay,gWindow);
}

void DrawTwentyFourSphere()
{
    GLfloat materialAmbient[4];
    GLfloat materialDiffused[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------
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

	for(int i = 0 ; i < 24 ; i++)
	{
		gluDeleteQuadric(quadric[i]);
		quadric[i] = NULL;
	}

	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
}
