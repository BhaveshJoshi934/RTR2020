#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/keysym.h>
#include<X11/XKBlib.h>

using namespace std;

bool bFullScreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;
Window gWindow;
int giWindowWidth = 800;
int giWindowHeight = 600;

int main()
{
	void CreateWindow(void);
	void ToggleFullScreen(void);
	void uninitialize(void);

	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;

	CreateWindow();

	XEvent event;
	KeySym keysym;

	while(1)
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
						uninitialize();
						exit(0);

					case XK_F:
					case XK_f:
						if(bFullScreen == false)
						{
							ToggleFullScreen();
							bFullScreen = true;
						}
						else
						{
							ToggleFullScreen();
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
				break;

			case Expose:
				break;

			case DestroyNotify:
				break;

			case 33:
				uninitialize();
				exit(0);

			default:
				break;
		}
	}
	uninitialize();
	return 0;
}

void CreateWindow(void)
{
	void uninitialize();

	XSetWindowAttributes winAttribs;
	int defaultScreen;
	int defaultDepth;
	int styleMask;

	gpDisplay = XOpenDisplay(NULL);
	if(gpDisplay == NULL)
	{
		printf("ERROR : Unable To Open XDisplay.\nExittingNo!!!\n");
		uninitialize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(gpDisplay);

	defaultDepth = DefaultDepth(gpDisplay,defaultScreen);

	gpXVisualInfo = (XVisualInfo *)malloc(sizeof(XVisualInfo));
	if(gpXVisualInfo == NULL)
	{
		printf("ERROR : Unable To Allocate Memory To Visual..\nExitting Now!!!\n");
		uninitialize();
		exit(1);
	}

	XMatchVisualInfo(gpDisplay,defaultScreen,defaultDepth,TrueColor,gpXVisualInfo);

	winAttribs.border_pixel = 0;
	winAttribs.background_pixmap = 0;
	winAttribs.colormap = XCreateColormap(gpDisplay,
					      RootWindow(gpDisplay,gpXVisualInfo->screen),
					      gpXVisualInfo->visual,
					      AllocNone);
	gColormap = winAttribs.colormap;
	winAttribs.background_pixel = BlackPixel(gpDisplay,defaultScreen);
	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask |StructureNotifyMask;

	styleMask = CWBorderPixel | CWBackPixel |CWEventMask | CWColormap;

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
		printf("ERROR : Unable TO Craete Main Window..\n");
		uninitialize();
		exit(1);
	}

	XStoreName(gpDisplay,gWindow,"Window : Bhavesh Joshi");

	Atom windowManagerDelete = XInternAtom(gpDisplay,"WM_DELETE_WINDOW",True);
	XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);

	XMapWindow(gpDisplay,gWindow);
}

void ToggleFullScreen()
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
		
void uninitialize()
{
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
