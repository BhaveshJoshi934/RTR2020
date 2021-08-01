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
#include"Sphere.h"

#pragma comment (lib,libSphere.so)

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

GLuint gVertexShaderObject_PV;
GLuint gFragmentShaderObject_PV;
GLuint gShaderProgramObject_PV;

GLuint gVertexShaderObject_PF;
GLuint gFragmentShaderObject_PF;
GLuint gShaderProgramObject_PF;


bool FragmentShader = true;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint vao_sphere;
GLuint NumVertices;
GLuint NumElements;
GLuint vbo_sphere_position;
GLuint vbo_sphere_normal;
GLuint vbo_sphere_element;

GLfloat lightPosition[] = {100.0f,100.0f,100.0f,1.0f};
GLfloat lightAmbient[] = {0.1f,0.1f,0.1f,1.0f};
GLfloat lightDiffuse[] = {0.5f,0.2f,0.7f,1.0f};
GLfloat lightSpecular[] = {0.7f,0.7f,0.7f,1.0f};
GLfloat MaterialAmbient[] = {0.0f,0.0f,0.0f,0.0f};
GLfloat MaterialDiffuse[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat MaterialSpecular[] = {1.0f,1.0f,1.0f,1.0f};

GLfloat MaterialShininess = 128.0f;

GLuint modelMatrixUniform_PF;                             
GLuint viewMatrixUniform_PF;
GLuint perspectiveProjectionUniform_PF;
GLuint LaUniform_PF;
GLuint LdUniform_PF;
GLuint LsUniform_PF;
GLuint lightPositionUniform_PF;
GLuint KaUniform_PF;
GLuint KdUniform_PF;
GLuint KsUniform_PF;
GLuint KshineUniform_PF;
GLuint LKeyPressedUniform_PF;

GLuint modelMatrixUniform_PV;                            
GLuint viewMatrixUniform_PV;
GLuint perspectiveProjectionUniform_PV;
GLuint LaUniform_PV;
GLuint LdUniform_PV;
GLuint LsUniform_PV;
GLuint lightPositionUniform_PV;
GLuint KaUniform_PV;
GLuint KdUniform_PV;
GLuint KsUniform_PV;
GLuint KshineUniform_PV;
GLuint LKeyPressedUniform_PV;

bool bLight = false;

GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint perspectiveProjectionUniform;

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
						
							case XK_Q:
							case XK_q:
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

							case XK_F:
							case XK_f:
								FragmentShader = true;
								break;

							case XK_V:
							case XK_v:
								FragmentShader = false;
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

	XStoreName(gpDisplay,gWindow,"Toggle Lighting Sphere in PP : Bhavesh Joshi!!");

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

    //Create Shader
    gVertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

    //Feed Shader
    const GLchar *vertexShaderSourceCode_PV =
            "#version 440 core" \
            "\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_perspective_projection_matrix;"
            "uniform vec3 u_la;" \
            "uniform vec3 u_ld;" \
            "uniform vec3 u_ls;" \
            "uniform vec3 u_ka;" \
            "uniform vec3 u_kd;" \
            "uniform vec3 u_ks;" \
            "uniform vec4 u_light_position;" \
            "uniform int u_LKeyPressed;" \
            "uniform float u_shininess;" \
            "out vec3 phong_ads_light;" \
            "void main(void)" \
            "{" \
            "if(u_LKeyPressed == 1)" \
            "{" \
            "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" \
            "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
            "vec3 light_direction = normalize(vec3(u_light_position - eye_coordinates));" \
            "vec3 reflection_vector = reflect(-light_direction,transformed_normal);" \
            "vec3 view_vector = normalize(-eye_coordinates.xyz);" \
            "vec3 ambient = u_la * u_ka;" \
            "vec3 diffuse = u_ld * u_kd * max(dot(light_direction,transformed_normal),0.0f);" \
            "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector,view_vector),0.0f),u_shininess);" \
            "phong_ads_light = ambient + diffuse + specular;" \
            "}" \
            "else" \
            "{" \
            "phong_ads_light = vec3(1.0f,1.0f,1.0f);" \
            "}" \
            "gl_Position = u_perspective_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
            "}";

    glShaderSource(gVertexShaderObject_PV,1,(const GLchar **)&vertexShaderSourceCode_PV,NULL);

    //Compile Shader
    glCompileShader(gVertexShaderObject_PV);

    GLint infoLogLength = 0;
    GLint shaderCompiledStatus = 0;
    char *szInfoLog = NULL;
    glGetShaderiv(gVertexShaderObject_PV,GL_COMPILE_STATUS,&shaderCompiledStatus);
    if(shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(gVertexShaderObject_PV,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gVertexShaderObject_PV,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Vertex Shader Compilation Log = %s\n\n",szInfoLog);
                free(szInfoLog);
               // DestroyWindow(ghwnd);
            }
        }
    }

    /*--------------------------------------------FRAGMENT SHADER-------------------------------------------------------------*/

    //Create Shader
    gFragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);

    //Feed Shader
    const GLchar *fragmentShaderSourceCode_PV =
        "#version 440 core" \
        "\n" \
        "in vec3 phong_ads_light;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "FragColor = vec4(phong_ads_light,1.0f);" \
        "}";

    glShaderSource(gFragmentShaderObject_PV,1,(const char **)&fragmentShaderSourceCode_PV,NULL);

    //Compile
    glCompileShader(gFragmentShaderObject_PV);

    glGetShaderiv(gFragmentShaderObject_PV,GL_COMPILE_STATUS,&shaderCompiledStatus);
    if(shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(gFragmentShaderObject_PV,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_PV,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Fragment Shader Compilation Log = %s\n\n",szInfoLog);
                free(szInfoLog);
               // DestroyWindow(ghwnd);
            }
        }
    }

    /*--------------------------------------------SHADER PROGRAM-------------------------------------------------------------*/

    //Create
    gShaderProgramObject_PV = glCreateProgram();

    //Attach
    glAttachShader(gShaderProgramObject_PV,gVertexShaderObject_PV);
    glAttachShader(gShaderProgramObject_PV,gFragmentShaderObject_PV);

    glBindAttribLocation(gShaderProgramObject_PV,BDJ_ATTRIBUTE_POSITION,"vPosition");
    glBindAttribLocation(gShaderProgramObject_PV,BDJ_ATTRIBUTE_NORMAL,"vNormal");

    //Link
    glLinkProgram(gShaderProgramObject_PV);

    GLint shaderProgramLinkStatus = 0;
    glGetProgramiv(gShaderProgramObject_PV,GL_LINK_STATUS,&shaderProgramLinkStatus);
    if(shaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gShaderProgramObject_PV,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(gFragmentShaderObject_PV,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Shader Program Link Log = %s\n\n",szInfoLog);
                free(szInfoLog);
               // DestroyWindow(ghwnd);
            }
        }
    }

    modelMatrixUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_model_matrix");
    viewMatrixUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_view_matrix");
    perspectiveProjectionUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_perspective_projection_matrix");
    LaUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_la");
    LdUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ld");
    LsUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ls");
    lightPositionUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_light_position");
    KaUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ka");
    KdUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_kd");
    KsUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ks");
    KshineUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_shininess");
    LKeyPressedUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_LKeyPressed");

  //Create Shader
    gVertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

    //Feed Shader
    const GLchar *vertexShaderSourceCode_PF =
            "#version 440 core" \
            "\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_perspective_projection_matrix;" \
            "uniform vec4 u_light_position;" \
            "uniform int u_LKeyPressed;" \
            "out vec3 transformed_normal;" \
            "out vec3 light_direction;" \
            "out vec3 view_vector;" \
            "void main(void)" \
            "{" \
            "if(u_LKeyPressed == 1)" \
            "{" \
            "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" \
            "transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal;" \
            "light_direction = vec3(u_light_position - eye_coordinates);" \
            "view_vector = -eye_coordinates.xyz;" \
            "}" \
            "gl_Position = u_perspective_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
            "}";

    glShaderSource(gVertexShaderObject_PF,1,(const GLchar **)&vertexShaderSourceCode_PF,NULL);

    //Compile Shader
    glCompileShader(gVertexShaderObject_PF);

    //GLint infoLogLength = 0;
    //GLint shaderCompiledStatus = 0;
    //char *szInfoLog = NULL;
    glGetShaderiv(gVertexShaderObject_PF,GL_COMPILE_STATUS,&shaderCompiledStatus);
    if(shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(gVertexShaderObject_PF,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gVertexShaderObject_PF,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Vertex Shader Compilation Log = %s\n\n",szInfoLog);
                free(szInfoLog);
               // DestroyWindow(ghwnd);
            }
        }
    }

    //Create Shader
    gFragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

    //Feed Shader
    const GLchar *fragmentShaderSourceCode_PF =
        "#version 440 core" \
        "\n" \
        "in vec3 transformed_normal;" \
        "in vec3 light_direction;" \
        "in vec3 view_vector;" \
        "uniform vec3 u_la;" \
        "uniform vec3 u_ld;" \
        "uniform vec3 u_ls;" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform float u_shininess;" \
        "uniform int u_LKeyPressed;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "vec3 phong_ads_light;" \
        "if(u_LKeyPressed == 1)" \
        "{" \
        "vec3 normalized_transformed_normal = normalize(transformed_normal);" \
        "vec3 normalized_light_direction    = normalize(light_direction);" \
        "vec3 normalized_view_vector        = normalize(view_vector);" \
        "vec3 ambient = u_la * u_ka;" \
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction,normalized_transformed_normal),0.0f);" \
        "vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normal);" \
        "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector,normalized_view_vector),0.0f),u_shininess);" \
        "phong_ads_light = ambient + diffuse + specular;" \
        "}" \
        "else" \
        "{" \
        "phong_ads_light = vec3(1.0f,1.0f,1.0f);" \
        "}" \
        "FragColor = vec4(phong_ads_light,1.0f);" \
        "}";

    glShaderSource(gFragmentShaderObject_PF,1,(const char **)&fragmentShaderSourceCode_PF,NULL);

    //Compile
    glCompileShader(gFragmentShaderObject_PF);

    glGetShaderiv(gFragmentShaderObject_PF,GL_COMPILE_STATUS,&shaderCompiledStatus);
    if(shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(gFragmentShaderObject_PF,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_PF,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Fragment Shader Compilation Log = %s\n\n",szInfoLog);
                free(szInfoLog);
               // DestroyWindow(ghwnd);
            }
        }
    }

    /*--------------------------------------------SHADER PROGRAM-------------------------------------------------------------*/

    //Create
    gShaderProgramObject_PF = glCreateProgram();

    //Attach
    glAttachShader(gShaderProgramObject_PF,gVertexShaderObject_PF);
    glAttachShader(gShaderProgramObject_PF,gFragmentShaderObject_PF);

    glBindAttribLocation(gShaderProgramObject_PF,BDJ_ATTRIBUTE_POSITION,"vPosition");
    glBindAttribLocation(gShaderProgramObject_PF,BDJ_ATTRIBUTE_NORMAL,"vNormal");

    //Link
    glLinkProgram(gShaderProgramObject_PF);

    //GLint shaderProgramLinkStatus = 0;
    glGetProgramiv(gShaderProgramObject_PF,GL_LINK_STATUS,&shaderProgramLinkStatus);
    if(shaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gShaderProgramObject_PF,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(gFragmentShaderObject_PF,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Shader Program Link Log = %s\n\n",szInfoLog);
                free(szInfoLog);
              //  DestroyWindow(ghwnd);
            }
        }
    }

    modelMatrixUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_model_matrix");
    viewMatrixUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_view_matrix");
    perspectiveProjectionUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_perspective_projection_matrix");
    LaUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_la");
    LdUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ld");
    LsUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ls");
    lightPositionUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_light_position");
    KaUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ka");
    KdUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_kd");
    KsUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ks");
    KshineUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_shininess");
    LKeyPressedUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_LKeyPressed");

    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    NumVertices = getNumberOfSphereVertices();
    NumElements = getNumberOfSphereElements();

    glGenVertexArrays(1, &vao_sphere);
    glBindVertexArray(vao_sphere);

    glGenBuffers(1, &vbo_sphere_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(BDJ_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_sphere_normal);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(BDJ_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_sphere_element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    bLight = false;

	glClearColor(0.0f,0.0f,0.0f,1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	Resize(giWindowWidth,giWindowHeight);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

if(FragmentShader == true)
{
    glUseProgram(gShaderProgramObject_PV);
    if(bLight == true)
    {
        glUniform1i(LKeyPressedUniform_PV,1);
        glUniform1f(KshineUniform_PV,MaterialShininess);      // Ithe Error Yeu Shakate
        glUniform4fv(lightPositionUniform_PV,1,lightPosition);
        glUniform3fv(LaUniform_PV,1,lightAmbient);
        glUniform3fv(LdUniform_PV,1,lightDiffuse);
        glUniform3fv(LsUniform_PV,1,lightSpecular);
        glUniform3fv(KaUniform_PV,1,MaterialAmbient);
        glUniform3fv(KdUniform_PV,1,MaterialDiffuse);
        glUniform3fv(KsUniform_PV,1,MaterialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform_PV,0);
    }

    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();     
    mat4 projectionMatrix = mat4::identity();
    mat4 translateMatrix = vmath::translate(0.0f,0.0f,-3.0f);

    modelMatrix = translateMatrix;    

    projectionMatrix = perspectiveProjectionMatrix;  
    glUniformMatrix4fv(modelMatrixUniform_PV,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform_PV,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform_PV,1,GL_FALSE,projectionMatrix);
}
else
{
    glUseProgram(gShaderProgramObject_PF);
    if(bLight == true)
    {
        glUniform1i(LKeyPressedUniform_PF,1);
        glUniform1f(KshineUniform_PF,MaterialShininess);     
        glUniform4fv(lightPositionUniform_PF,1,lightPosition);
        glUniform3fv(LaUniform_PF,1,lightAmbient);
        glUniform3fv(LdUniform_PF,1,lightDiffuse);
        glUniform3fv(LsUniform_PF,1,lightSpecular);
        glUniform3fv(KaUniform_PF,1,MaterialAmbient);
        glUniform3fv(KdUniform_PF,1,MaterialDiffuse);
        glUniform3fv(KsUniform_PF,1,MaterialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform_PF,0);
    }

    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();     
    mat4 projectionMatrix = mat4::identity();
    mat4 translateMatrix = vmath::translate(0.0f,0.0f,-3.0f);

    modelMatrix = translateMatrix;   

    projectionMatrix = perspectiveProjectionMatrix;  
    glUniformMatrix4fv(modelMatrixUniform_PF,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform_PF,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform_PF,1,GL_FALSE,projectionMatrix);

}
        glBindVertexArray(vao_sphere);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
        glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);

	glUseProgram(0);

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

    	if(vao_sphere)
    	{
        	glDeleteBuffers(1,&vao_sphere);
       		vao_sphere = 0;
    	}

    	if(vbo_sphere_normal)
    	{
        	glDeleteBuffers(1,&vbo_sphere_normal);
        	vbo_sphere_normal = 0;
    	}

    	if(vbo_sphere_position)
    	{
        	glDeleteBuffers(1,&vbo_sphere_position);
        	vbo_sphere_position = 0;
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
