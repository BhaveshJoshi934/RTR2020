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

#include<SOIL/SOIL.h>
#include"Teapot.h"


using namespace std;

bool bFullScreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL; 
Colormap gColormap;
Window gWindow;
int giWindowWidth = 800;
int giWindowHeight = 600;

GLXContext gGLXContext;

GLuint Marbel_Texture;

bool gbTexture = false;
bool gbAnimate = false;
bool gbLight = false;

GLfloat Angle = 0.0f;

GLfloat lightAmbient[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffused[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat lightSpecular[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat lightPosition[] = {100.0f,100.0f,100.0f,1.0f};

GLfloat materialAmbient[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat materialDiffused[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat materialSpecular[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat materialShininess = 50.0f;
 
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

							case XK_T:
							case XK_t:
								if(gbTexture == false)
								{
									glEnable(GL_TEXTURE_2D);
									gbTexture = true;
								}
								else
								{
									glDisable(GL_TEXTURE_2D);
									gbTexture = false;
								}
								break;

							case XK_A:
							case XK_a:
								if(gbAnimate == false)
								{
									gbAnimate = true;
								}
								else
								{
									gbAnimate = false;
								}
								break;

							case XK_L:
							case XK_l:
								if(gbLight == true)
								{
									glEnable(GL_LIGHTING);
									gbLight = false;
								}
								else
								{
									glDisable(GL_LIGHTING);
									gbLight = true;
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

	XStoreName(gpDisplay,gWindow,"Teapot With Light : Bhavesh Joshi!!");

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

	GLuint LoadBitmapAsTexture(const char *);

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

	//glEnable(GL_TEXTURE_2D);
	Marbel_Texture = LoadBitmapAsTexture("marble.bmp");

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffused);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

	glMaterialfv(GL_FRONT,GL_AMBIENT,materialAmbient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffused);
	glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
	glMaterialf(GL_FRONT,GL_SHININESS,materialShininess);

	glClearColor(0.0f,0.0f,0.0f,0.0f);

	Resize(giWindowWidth,giWindowHeight);
}

GLuint LoadBitmapAsTexture(const char * path)
{
	int width;
	int height;
	unsigned char *imagedata = NULL;
	GLuint textureID;

	imagedata = SOIL_load_image(path,
				    &width,
				    &height,
				    NULL,
				    SOIL_LOAD_RGB);

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D,3,width,height,GL_RGB,GL_UNSIGNED_BYTE,imagedata);

	SOIL_free_image_data(imagedata);

	return textureID;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-1.0f);

	glRotatef(Angle,0.0f,1.0f,0.0f);

	if(gbAnimate == true)
	{
		Angle = Angle + 1.0f;
	}

	glBindTexture(GL_TEXTURE_2D,Marbel_Texture);

	glBegin(GL_TRIANGLES);

	for(int i = 0 ; i < sizeof(face_indicies)/sizeof(face_indicies[0]);i++)
	{
		for(int j = 0; j < 3 ; j++)
		{
			int vi = face_indicies[i][j];
			int ni = face_indicies[i][j+3];
			int ti = face_indicies[i][j+6];

			glNormal3f(normals[ni][0],normals[ni][1],normals[ni][2]);
			glTexCoord2f(textures[ti][0],textures[ti][1]);
			glVertex3f(vertices[vi][0],vertices[vi][1],vertices[vi][2]);
		}
	}

	glEnd();
	
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

	if(Marbel_Texture)
	{
		glDeleteTextures(1,&Marbel_Texture);
		Marbel_Texture = 0;
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
