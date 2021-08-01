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

using namespace vmath;
using namespace std;

#include<SOIL/SOIL.h>

FILE* gpFile = NULL;

enum 
{
	BDJ_ATTRIBUTE_POSITION = 0, 
	BDJ_ATTRIBUTE_COLOR,
	BDJ_ATTRIBUTE_NORMAL,
	BDJ_ATTRIBUTE_TEXTURE0
};

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint vao_pyramid_BDJ;
GLuint vbo_pyramid_Position_BDJ;
GLuint vbo_pyramid_Texture_BDJ;

GLuint vao_cube_BDJ;
GLuint vbo_cube_Position_BDJ;
GLuint vbo_cube_Texture_BDJ;
GLuint vao_PCNT_BDJ;
GLuint vbo_PCNT_BDJ;

GLuint modelViewMatrixUniform;
GLuint perspectiveProjectionUniform;
GLuint LKeyPressedUniform;
GLuint LdUniform;
GLuint KdUniform;
GLuint lightPositionUniform;

GLuint Texture_Stone;
GLuint Texture_Marble;
GLuint textureSamplerUniform;

mat4 perspectiveProjectionMatrix;

bool bFullScreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL; 
Colormap gColormap;
Window gWindow;
int giWindowWidth = 800;
int giWindowHeight = 600;
bool bLight = false;
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

							case XK_L:
							case XK_l:
								if(bLight == false)
								{
									bLight = true;
								}
								else
								{
									bLight = false;
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

	XStoreName(gpDisplay,gWindow,"Interleaved in PP : Bhavesh Joshi!!");

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
	GLuint LoadBitmapAsTexture(const char*);

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
            "in vec4 vColor;" \
            "in vec2 vTexCoord;" \
            "in vec3 vNormal;" \
            "uniform mat4 u_model_view_matrix;" \
            "uniform mat4 u_projection_matrix;" \
            "uniform int u_LKeyPressed;" \
            "uniform vec3 u_ld;" \
            "uniform vec3 u_kd;" \
            "uniform vec4 u_light_position;" \
            "out vec3 diffuse_light;" \
            "out vec2 out_TexCoord;" \
            "out vec4 out_Color;" \
            "void main(void)" \
            "{" \
            "if(u_LKeyPressed == 1)" \
            "{" \
            "vec4 eye_coordinates = u_model_view_matrix * vPosition;" \
            "mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));" \
            "vec3 tnorm = normalize(normal_matrix * vNormal);" \
            "vec3 s = normalize(vec3(u_light_position - eye_coordinates));" \
            "diffuse_light = u_ld * u_kd * max(dot(s,tnorm),0.0f);" \
            "}"
            "gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;" \
            "out_TexCoord = vTexCoord;" \
            "out_Color = vColor;" \
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
        "in vec3 diffuse_light;" \
        "uniform int u_LKeyPressed;" \
        "in vec2 out_TexCoord;" \
        "in vec4 out_Color;" \
        "uniform sampler2D u_texture_sampler;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "vec4 tex =  vec4(texture(u_texture_sampler,out_TexCoord));" \
        "vec4 light;" \
        "if(u_LKeyPressed == 1)" \
        "{" \
        "light = vec4(diffuse_light,1.0f);" \
        "}" \
        "else" \
        "{" \
        "light = vec4(1.0f,1.0f,1.0f,1.0f);"
        "}" \
        "FragColor = tex * light * out_Color;" \
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
    	glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_COLOR,"vColor");
    	glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_NORMAL,"vNormal");
    	glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_TEXTURE0,"vTexCoord");
	
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
	
	textureSamplerUniform = glGetUniformLocation(gShaderProgramObject,"u_texture_sampler");
    	modelViewMatrixUniform = glGetUniformLocation(gShaderProgramObject,"u_model_view_matrix");
    	perspectiveProjectionUniform = glGetUniformLocation(gShaderProgramObject,"u_projection_matrix");
    	LKeyPressedUniform = glGetUniformLocation(gShaderProgramObject,"u_LKeyPressed");
    	LdUniform = glGetUniformLocation(gShaderProgramObject,"u_ld");
    	KdUniform = glGetUniformLocation(gShaderProgramObject,"u_kd");
    	lightPositionUniform = glGetUniformLocation(gShaderProgramObject,"u_light_position");

    const GLfloat PCNT_cube[] =
            {
                0.5f,0.5f,0.5f,    1.0f,0.0f,0.0f,  0.0f, 0.0f, 1.0f,   0.0f,0.0f,
                -0.5f,0.5f,0.5f,   1.0f,0.0f,0.0f,  0.0f, 0.0f, 1.0f,   1.0f,0.0f,
                -0.5f,-0.5f,0.5f,  1.0f,0.0f,0.0f,  0.0f, 0.0f, 1.0f,   1.0f,1.0f,
                0.5f,-0.5f,0.5f,   1.0f,0.0f,0.0f,  0.0f, 0.0f, 1.0f,   0.0f,1.0f,

                0.5f,0.5f,-0.5f,   0.0f,1.0f,0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
                0.5f,0.5f,0.5f,    0.0f,1.0f,0.0f,  1.0f, 0.0f, 0.0f,   1.0f,1.0f,
                0.5f,-0.5f,0.5f,   0.0f,1.0f,0.0f,  1.0f, 0.0f, 0.0f,   0.0f,1.0f,
                0.5f,-0.5f,-0.5f,  0.0f,1.0f,0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,

                -0.5f,0.5f,-0.5f,  0.0f,0.0f,1.0f,  0.0f, 0.0f, -1.0f,  1.0f,0.0f,
                0.5f,0.5f,-0.5f,   0.0f,0.0f,1.0f,  0.0f, 0.0f, -1.0f,  1.0f,1.0f,
                0.5f,-0.5f,-0.5f,  0.0f,0.0f,1.0f,  0.0f, 0.0f, -1.0f,  0.0f,1.0f,
                -0.5f,-0.5f,-0.5f, 0.0f,0.0f,1.0f,  0.0f, 0.0f, -1.0f,  0.0f,0.0f,

                -0.5f,0.5f,0.5f,   1.0f,1.0f,0.0f,  -1.0f, 0.0f, 0.0f,  0.0f,0.0f,
                -0.5f,0.5f,-0.5f,  1.0f,1.0f,0.0f,  -1.0f, 0.0f, 0.0f,  1.0f,0.0f,
                -0.5f,-0.5f,-0.5f, 1.0f,1.0f,0.0f,  -1.0f, 0.0f, 0.0f,  1.0f,1.0f,
                -0.5f,-0.5f,0.5f,  1.0f,1.0f,0.0f,  -1.0f, 0.0f, 0.0f,  0.0f,1.0f,

                0.5f,0.5f,-0.5f,   0.0f,1.0f,1.0f,   0.0f, 1.0f, 0.0f,  0.0f,1.0f,
                -0.5f,0.5f,-0.5f,  0.0f,1.0f,1.0f,   0.0f, 1.0f, 0.0f,  0.0f,0.0f,
                -0.5f,0.5f,0.5f,   0.0f,1.0f,1.0f,   0.0f, 1.0f, 0.0f,  1.0f,0.0f,
                0.5f,0.5f,0.5f,    0.0f,1.0f,1.0f,   0.0f, 1.0f, 0.0f,  1.0f,1.0f,

                0.5f,-0.5f,-0.5f,  1.0f,0.0f,1.0f,   0.0f, -1.0f, 0.0f,  1.0f,1.0f,
                -0.5f,-0.5f,-0.5f, 1.0f,0.0f,1.0f,   0.0f, -1.0f, 0.0f,  0.0f,1.0f,
                -0.5f,-0.5f,0.5f,  1.0f,0.0f,1.0f,   0.0f, -1.0f, 0.0f,  0.0f,0.0f,
                0.5f,-0.5f,0.5f,   1.0f,0.0f,1.0f,   0.0f, -1.0f, 0.0f,  1.0f,0.0f
            };

    	glGenVertexArrays(1,&vao_PCNT_BDJ);
    	glBindVertexArray(vao_PCNT_BDJ);

    	glGenBuffers(1,&vbo_PCNT_BDJ);
    	glBindBuffer(GL_ARRAY_BUFFER,vbo_PCNT_BDJ);
    	glBufferData(GL_ARRAY_BUFFER,24 * 11 * sizeof(float),PCNT_cube,GL_STATIC_DRAW);
    	//Position
    	glVertexAttribPointer(BDJ_ATTRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,11 * sizeof(GLfloat),0);
    	glEnableVertexAttribArray(BDJ_ATTRIBUTE_POSITION);
    	//Color
    	glVertexAttribPointer(BDJ_ATTRIBUTE_COLOR,3,GL_FLOAT,GL_FALSE,11 * sizeof(GLfloat),(GLvoid *)(3 * sizeof(GLfloat)));
    	glEnableVertexAttribArray(BDJ_ATTRIBUTE_COLOR);
    	//Normal
    	glVertexAttribPointer(BDJ_ATTRIBUTE_NORMAL,3,GL_FLOAT,GL_FALSE,11 * sizeof(GLfloat),(GLvoid *)(6 * sizeof(GLfloat)));
    	glEnableVertexAttribArray(BDJ_ATTRIBUTE_NORMAL);
    	//Texture
    	glVertexAttribPointer(BDJ_ATTRIBUTE_TEXTURE0,2,GL_FLOAT,GL_FALSE,11 * sizeof(GLfloat),(GLvoid *)(9 * sizeof(GLfloat)));
    	glEnableVertexAttribArray(BDJ_ATTRIBUTE_TEXTURE0);

    	glBindBuffer(GL_ARRAY_BUFFER,0);

    	glBindVertexArray(0);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f,0.0f,0.0f,1.0f);
	
	glEnable(GL_TEXTURE_2D);
	Texture_Stone   = LoadBitmapAsTexture("./Stone.bmp");
	Texture_Marble = LoadBitmapAsTexture("./marble.bmp");

	perspectiveProjectionMatrix = mat4::identity();

	Resize(giWindowWidth,giWindowHeight);
}

GLuint LoadBitmapAsTexture(const char *path)
{
	int width,height;
	unsigned char *imagedata = NULL;
	GLuint textureID;

	imagedata = SOIL_load_image(path,&width,&height,NULL,SOIL_LOAD_RGB);

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
//	gluBuild2DMipmaps(GL_TEXTURE_2D,3,width,height,GL_RGB,GL_UNSIGNED_BYTE,imagedata);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,imagedata);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(imagedata);

	return(textureID);
}

void Resize(int width,int height)
{
	if(height == 1)
		height = 0;

	glViewport(0,0,(GLsizei)width,(GLsizei)height);

        perspectiveProjectionMatrix = vmath::perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}

void Draw(void)
{
	static GLfloat pyr_angle_BDJ = 0.0f;
	static GLfloat cu_angle_BDJ = 0.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Start Using OpenGL Program
    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        glUniform1i(LKeyPressedUniform,1);
        glUniform3f(LdUniform,1.0f,1.0f,1.0f);
        glUniform3f(KdUniform,0.5f,0.5f,0.5f);
        GLfloat lightPosition[] = {0.0f,0.0f,2.0f,1.0f};
        glUniform4fv(lightPositionUniform,1,lightPosition);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    //FOR TRIANGLE

    mat4 modelViewMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();
    mat4 translateMatrix = vmath::translate(0.0f,0.0f,-4.0f);
    mat4 rotationMatrix1 = mat4::identity();
    mat4 rotationMatrix2 = mat4::identity();
    mat4 rotationMatrix3 = mat4::identity();
    rotationMatrix1 = vmath::rotate((GLfloat)cu_angle_BDJ,1.0f,0.0f,0.0f);
    rotationMatrix2 = vmath::rotate((GLfloat)cu_angle_BDJ,0.0f,1.0f,0.0f);
    rotationMatrix3 = vmath::rotate((GLfloat)cu_angle_BDJ,0.0f,0.0f,1.0f);

    modelViewMatrix = translateMatrix * rotationMatrix1 * rotationMatrix2 * rotationMatrix3 ;

    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(modelViewMatrixUniform,1,GL_FALSE,modelViewMatrix);

    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,perspectiveProjectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,Texture_Marble);
    glUniform1i(textureSamplerUniform,0);

    glBindVertexArray(vao_cube_BDJ);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDrawArrays(GL_TRIANGLE_FAN,4,4);
    glDrawArrays(GL_TRIANGLE_FAN,8,4);
    glDrawArrays(GL_TRIANGLE_FAN,16,4);
    glDrawArrays(GL_TRIANGLE_FAN,20,4);
    glDrawArrays(GL_TRIANGLE_FAN,24,4);

    glBindVertexArray(0);

//--------------------------------------------------------------------------------------------
/*
	translateMatrix = vmath::translate(1.5f,0.0f,-4.0f);
	rotateMatrix1 = vmath::rotate(pyr_angle_BDJ,0.0f,1.0f,0.0f);

	modelViewMatrix = translateMatrix * rotateMatrix1;
	modelViewProjectionMatrix =  perspectiveProjectionMatrix * modelViewMatrix;
	glUniformMatrix4fv(mvpMatrix,1,GL_FALSE,modelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,Texture_Stone);
	glUniform1i(textureSamplerUniform,0);

	glBindVertexArray(vao_pyramid_BDJ);
	glDrawArrays(GL_TRIANGLES,0,12);
	glBindVertexArray(0);
*/
	glUseProgram(0);


	pyr_angle_BDJ += 1.0f;
	if(pyr_angle_BDJ >= 360.0f)
	{
		pyr_angle_BDJ = 0.0f;
	}

	cu_angle_BDJ += 1.0f;
	if(cu_angle_BDJ >= 360.0f)
	{
		cu_angle_BDJ = 0.0f;
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

	if(vao_pyramid_BDJ)
	{
		glDeleteVertexArrays(1,&vao_pyramid_BDJ);
		vao_pyramid_BDJ = 0;
	}

	if(vao_cube_BDJ)
	{
		glDeleteVertexArrays(1,&vao_cube_BDJ);
		vao_cube_BDJ = 0;
	}

	if(vbo_pyramid_Position_BDJ)
	{
		glDeleteBuffers(1,&vbo_pyramid_Position_BDJ);
		vbo_pyramid_Position_BDJ = 0;
	}

	if(vbo_pyramid_Texture_BDJ)
	{
		glDeleteBuffers(1,&vbo_pyramid_Texture_BDJ);
		vbo_pyramid_Texture_BDJ = 0;
	}

	if(vbo_cube_Position_BDJ)
	{
		glDeleteBuffers(1,&vbo_cube_Position_BDJ);
		vbo_cube_Position_BDJ = 0;
	}

	if(vbo_cube_Texture_BDJ)
	{
		glDeleteBuffers(1,&vbo_cube_Texture_BDJ);
		vbo_cube_Texture_BDJ = 0;
	}

	if(Texture_Stone)
	{
		glDeleteTextures(1,&Texture_Stone);
		Texture_Stone = 0;
	}

	if(Texture_Marble)
	{
		glDeleteTextures(1,&Texture_Marble);
		Texture_Marble = 0;
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
