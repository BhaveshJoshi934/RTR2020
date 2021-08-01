#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>

#include<GL/glew.h>

#include<GL/gl.h>
#include<GL/glx.h>
#include"vmath.h"

#define   CHECKIMAGEWIDTH    64
#define   CHECKIMAGEHEIGHT   64

using namespace vmath;
using namespace std;

FILE* gpFile = NULL;

enum 
{
	BDJ_ATTRIBUTE_POSITION = 0, 
	BDJ_ATTRBUTE_COLOR,
	BDJ_ATTRIBUTE_NORMAL,
	BDJ_ATTRIBUTE_TEXTURE0
};

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint vao_checker_board;
GLuint vbo_Position_checker_board;
GLuint vbo_Texture_checker_board;

GLubyte CheckImage[CHECKIMAGEWIDTH][CHECKIMAGEHEIGHT][4];
GLuint TEXT_IMAGE;
GLuint textureSamplerUniform;

GLuint mvpMatrix;

mat4 perspectiveProjectionMatrix;

bool bFullScreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL; 
Colormap gColormap;
Window gWindow;
int giWindowWidth = 800;
int giWindowHeight = 600;

GLXContext gGLXContext;

typedef GLXContext (*glXCreateContextAttribsARBProc) (Display* , GLXFBConfig , GLXContext , Bool , const int *);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig gGLXFBConfig;
 
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

	printf("Program Starts Here!!!\n\n");

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

	GLXFBConfig *pGLXFBConfig = NULL;
	GLXFBConfig bestGLXFBConfig;
	XVisualInfo *pTempXVisualInfo = NULL;
	int numFBConfig = 0;

	int defaultScreen;
	int defaultDepth;
	int styleMask;

	static int FrameBufferAttributes[] = {
						//GLX_RGBA,
                                                GLX_X_RENDERABLE, True,
						GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,
						GLX_RENDER_TYPE,GLX_RGBA_BIT,
						GLX_X_VISUAL_TYPE,GLX_TRUE_COLOR,
						GLX_RED_SIZE,8,
						GLX_GREEN_SIZE,8,
						GLX_BLUE_SIZE,8,
						GLX_ALPHA_SIZE,8,
						GLX_STENCIL_SIZE,8,
						GLX_DOUBLEBUFFER,True,
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

	pGLXFBConfig = glXChooseFBConfig(gpDisplay,XDefaultScreen(gpDisplay),FrameBufferAttributes,&numFBConfig);

	if(pGLXFBConfig == NULL)
	{
		printf("We can't Found any FBConfig\n\n");
		unInitialize();
	}
	else
	{
		printf("GLXConfig Found = %d \n\n",numFBConfig);
	}

	int bestFrameBufferConfig = -1,worstFrameBufferConfig = -1,bestNumberOfSamples = -1,worstNumberOfSamples = 999;

	int i;

	for(i = 0 ; i < numFBConfig ; i++)
	{
		pTempXVisualInfo = glXGetVisualFromFBConfig(gpDisplay,pGLXFBConfig[i]);

		if(pTempXVisualInfo != NULL)
		{
			int sampleBuffers,samples;

			glXGetFBConfigAttrib(gpDisplay,pGLXFBConfig[i],GLX_SAMPLE_BUFFERS,&sampleBuffers);
			glXGetFBConfigAttrib(gpDisplay,pGLXFBConfig[i],GLX_SAMPLES,&samples);

			if(bestFrameBufferConfig < 0 || sampleBuffers && samples > bestNumberOfSamples)
			{
				bestFrameBufferConfig = i;
				bestNumberOfSamples = samples;
			}

			if(worstFrameBufferConfig < 0 || (!sampleBuffers) || samples > worstNumberOfSamples)
			{
				worstFrameBufferConfig = i;
				worstNumberOfSamples = samples;
			}
			printf("For ith = %d\t,samples = %d\t,sampleBuffers = %d\n\n",i,samples,sampleBuffers);
		}
		XFree(pTempXVisualInfo);
	}
	
	bestGLXFBConfig = pGLXFBConfig[bestFrameBufferConfig];
	gGLXFBConfig = bestGLXFBConfig;

	XFree(pGLXFBConfig);

	gpXVisualInfo = glXGetVisualFromFBConfig(gpDisplay,gGLXFBConfig);

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

	XStoreName(gpDisplay,gWindow,"Checker Board in PP : Bhavesh Joshi!!");

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
	void LoadGLTexture();

	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");

	const int Attribs[] = 
			{
				GLX_CONTEXT_MAJOR_VERSION_ARB,4,
				GLX_CONTEXT_MINOR_VERSION_ARB,5,
				GLX_CONTEXT_PROFILE_MASK_ARB,GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
				None
			};

	gGLXContext = glXCreateContextAttribsARB(gpDisplay,gGLXFBConfig,0,True,Attribs);

	if(!gGLXContext)
	{
		const int Attribs[] = 
				{
					GLX_CONTEXT_MAJOR_VERSION_ARB,1,
					GLX_CONTEXT_MINOR_VERSION_ARB,0,
					None
				};

		gGLXContext = glXCreateContextAttribsARB(gpDisplay,gGLXFBConfig,0,True,Attribs);
	}

	Bool bIsDirectContext = glXIsDirect(gpDisplay,gGLXContext);

	if(bIsDirectContext == True)
	{
		printf("Rendering Context is Hardware Redering Context!!!\n\n");
	}
	else
	{
		printf("Rendering Context is Software Rendering Context!!!\n\n");
	}

	glXMakeCurrent(gpDisplay,gWindow,gGLXContext);

	printf("OpenGL Vendor : %s\n",glGetString(GL_VENDOR));
	printf("OpenGL Renderer Version : %s\n",glGetString(GL_RENDERER));
	printf("OpenGL Version : %s\n",glGetString(GL_VERSION));
	printf("OpenGL GLSL Version : %s\n\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

	glewInit();

	GLint numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS,&numExtensions);

	for(int i = 0 ; i < numExtensions; i++)
	{
		printf("%s\n\n",glGetStringi(GL_EXTENSIONS,i));
	}

	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *vertexShaderSourceCode = 
		"#version 440 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec2 vTexture;" \
		"uniform mat4 u_mvp_matrix;" \
		"out vec2 out_Texture;" \
		"void main(void)" \
		"{" \
	      	"gl_Position = u_mvp_matrix * vPosition;" \
		"out_Texture = vTexture;" \
		"}";
	
	glShaderSource(gVertexShaderObject,1,(const GLchar**)&vertexShaderSourceCode,NULL);

	glCompileShader(gVertexShaderObject);
	
	GLint infoLogLength = 0;
	GLint compilationStatus = 0;
	char *szInfoLength = NULL;

	glGetShaderiv(gVertexShaderObject,GL_COMPILE_STATUS,&compilationStatus);

	if(compilationStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject,GL_INFO_LOG_LENGTH,&infoLogLength);

		if(infoLogLength > 0)
		{
			szInfoLength = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);

			if(szInfoLength != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject,infoLogLength,&written,szInfoLength);
				fprintf(gpFile,"Vertex Shader Compilation Log = %s\n\n",szInfoLength);
				free(szInfoLength);
//				unInitialize();
			}
		}
	} 

//------------------------------------------------------------------------------------------------------------------------

	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *fragmentShaderSourceCode = 
		"#version 440 core" \
		"\n" \
		"in vec2 out_Texture;" \
		"uniform sampler2D u_texture_sampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = texture(u_texture_sampler,out_Texture);" \
		"}";

	glShaderSource(gFragmentShaderObject,1,(const GLchar**)&fragmentShaderSourceCode,NULL);

	glCompileShader(gFragmentShaderObject);

	glGetShaderiv(gFragmentShaderObject,GL_COMPILE_STATUS,&compilationStatus);

	if(compilationStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject,GL_INFO_LOG_LENGTH,&infoLogLength);

		if(infoLogLength > 0)
		{
			szInfoLength = (GLchar*)malloc(sizeof(GLchar) * infoLogLength);

			if(szInfoLength != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject,infoLogLength,&written,szInfoLength);
				fprintf(gpFile,"Fragment Shader Compilation Log = %s \n\n",szInfoLength);
				free(szInfoLength);
			//	unInitialize();
			}
		}
	}

//---------------------------------------------------------------------------------------------------------------

	gShaderProgramObject = glCreateProgram();

	glAttachShader(gShaderProgramObject,gVertexShaderObject);
	glAttachShader(gShaderProgramObject,gFragmentShaderObject);

	glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_POSITION,"vPosition");
	glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_TEXTURE0,"vTexture");

	glLinkProgram(gShaderProgramObject);
	
	GLint shaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject,GL_LINK_STATUS,&shaderProgramLinkStatus);

	if(shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject,GL_INFO_LOG_LENGTH,&infoLogLength);
		
		if(infoLogLength > 0)
		{
			szInfoLength = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
			if(szInfoLength != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject,infoLogLength,&written,szInfoLength);
				fprintf(gpFile,"Program Link log = %s \n\n",szInfoLength);
				free(szInfoLength);
			//	unInitialize();
			}
		}
	}
	
	mvpMatrix = glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");
	textureSamplerUniform = glGetUniformLocation(gShaderProgramObject,"u_texture_sampler");

	const GLfloat CheckerTexcoords[] =
	{
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f
	};

	glGenVertexArrays(1,&vao_checker_board);
	glBindVertexArray(vao_checker_board);

	glGenBuffers(1,&vbo_Position_checker_board);
	glBindBuffer(GL_ARRAY_BUFFER,vbo_Position_checker_board);
	glBufferData(GL_ARRAY_BUFFER,4*3*sizeof(GLfloat),NULL,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(BDJ_ATTRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(BDJ_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1,&vbo_Texture_checker_board);
	glBindBuffer(GL_ARRAY_BUFFER,vbo_Texture_checker_board);
	glBufferData(GL_ARRAY_BUFFER,sizeof(CheckerTexcoords),CheckerTexcoords,GL_STATIC_DRAW);
	glVertexAttribPointer(BDJ_ATTRIBUTE_TEXTURE0,2,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(BDJ_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glBindVertexArray(0);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);
	LoadGLTexture();

	glClearColor(0.0f,0.0f,0.0f,1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	Resize(giWindowWidth,giWindowHeight);
}

void LoadGLTexture()
{
	void MakeCheckImage(void);
	MakeCheckImage();

	glGenTextures(1,&TEXT_IMAGE);
	glBindTexture(GL_TEXTURE_2D,TEXT_IMAGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,CHECKIMAGEWIDTH,CHECKIMAGEHEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,CheckImage);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,CHECKIMAGEWIDTH,CHECKIMAGEHEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,CheckImage);
}

void MakeCheckImage()
{
	int i,j,c;
	for(i = 0 ; i < CHECKIMAGEWIDTH ; i++)
	{
		for(j = 0 ; j < CHECKIMAGEHEIGHT ; j++)
		{
			 c  = (((i & 0X8) == 0) ^ ((j & 0X8) == 0)) * 255;
			 CheckImage[i][j][0] = (GLubyte)c;
			 CheckImage[i][j][1] = (GLubyte)c;
			 CheckImage[i][j][2] = (GLubyte)c;
			 CheckImage[i][j][3] = 255;

		}
	}
}

void Resize(int width,int height)
{
	if(height == 1)
		height = 0;

	glViewport(0,0,(GLsizei)width,(GLsizei)height);

        perspectiveProjectionMatrix = vmath::perspective(60.0f,(GLfloat)width/(GLfloat)height,0.1f,30.0f);
}

void Draw(void)
{
	static GLfloat Vertices_straight[12];
	static GLfloat Vertices_angular[12];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgramObject);
	
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 translateMatrix = vmath::translate(0.0f,0.0f,-3.6f);

	modelViewMatrix = translateMatrix;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrix,1,GL_FALSE,modelViewProjectionMatrix);

	glBindVertexArray(vao_checker_board);

	Vertices_straight[0] = -2.0f;
    	Vertices_straight[1] = -1.0f;
    	Vertices_straight[2] = 0.0f;
    	Vertices_straight[3] = -2.0f;
    	Vertices_straight[4] = 1.0f;
    	Vertices_straight[5] = 0.0f;
   	Vertices_straight[6] = 0.0f;
    	Vertices_straight[7] = 1.0f;
    	Vertices_straight[8] = 0.0f;
    	Vertices_straight[9] = 0.0f;
    	Vertices_straight[10] = -1.0f;
    	Vertices_straight[11] = 0.0f;

    	glBindBuffer(GL_ARRAY_BUFFER,vbo_Position_checker_board);
    	glBufferData(GL_ARRAY_BUFFER,4*3*sizeof(GLfloat),Vertices_straight,GL_DYNAMIC_DRAW);
    	glVertexAttribPointer(BDJ_ATTRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL);
    	glEnableVertexAttribArray(BDJ_ATTRIBUTE_POSITION);
    	glBindBuffer(GL_ARRAY_BUFFER,0);

    	glDrawArrays(GL_TRIANGLE_FAN,0,4);

	glBindVertexArray(0);

    	glBindVertexArray(vao_checker_board);

    	Vertices_angular[0] = 1.0f;
   	Vertices_angular[1] = -1.0f;
    	Vertices_angular[2] = 0.0f;
    	Vertices_angular[3] = 1.0f;
    	Vertices_angular[4] = 1.0f;
    	Vertices_angular[5] = 0.0f;
    	Vertices_angular[6] = 2.41421f;
    	Vertices_angular[7] = 1.0f;
    	Vertices_angular[8] = -1.41421f;
    	Vertices_angular[9] = 2.41421f;
    	Vertices_angular[10] = -1.0f;
    	Vertices_angular[11] = -1.41421f;

    	glBindBuffer(GL_ARRAY_BUFFER,vbo_Position_checker_board);
    	glBufferData(GL_ARRAY_BUFFER,4*3*sizeof(GLfloat),Vertices_angular,GL_DYNAMIC_DRAW);
    	glVertexAttribPointer(BDJ_ATTRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL);
    	glEnableVertexAttribArray(BDJ_ATTRIBUTE_POSITION);
    	glBindBuffer(GL_ARRAY_BUFFER,0);

    	glDrawArrays(GL_TRIANGLE_FAN,0,4);

    	glBindVertexArray(0);
	
	glUseProgram(0);

    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D,TEXT_IMAGE);
    	glUniform1i(textureSamplerUniform,0);

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

	if(vao_checker_board)
	{
		glDeleteVertexArrays(1,&vao_checker_board);
		vao_checker_board = 0;
	}

	if(vbo_Position_checker_board)
	{
		glDeleteBuffers(1,&vbo_Position_checker_board);
		vbo_Position_checker_board = 0;
	}

	if(vbo_Texture_checker_board)
	{
		glDeleteBuffers(1,&vbo_Texture_checker_board);
		vbo_Texture_checker_board = 0;
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
		printf("Program Ends Here!!!\n\n");
		gpDisplay = NULL;
	}

}
