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

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

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

bool bLight = false;
GLfloat factor = 10.0f;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.0f;

GLfloat lightPosition_Red[] = {0.0f,0.0,0.0f,1.0f};
GLfloat lightAmbient_Red[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Red[] = {1.0f,0.0f,0.0f,1.0f};
GLfloat lightSpecular_Red[] = {1.0f,0.0f,0.0f,1.0f};

GLfloat lightPosition_Blue[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightAmbient_Blue[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Blue[] = {0.0f,0.0f,1.0f,1.0f};
GLfloat lightSpecular_Blue[] = {0.0f,0.0f,1.0f,1.0f};

GLfloat lightPosition_Green[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightAmbient_Green[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Green[] = {0.0f,1.0f,0.0f,1.0f};
GLfloat lightSpecular_Green[] = {0.0f,1.0f,0.0f,1.0f};

GLfloat MaterialAmbient[] = {0.0f,0.0f,0.0f,0.0f};
GLfloat MaterialDiffuse[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat MaterialSpecular[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat MaterialShininess = 128.0f;

GLuint modelMatrixUniform;                            
GLuint viewMatrixUniform;
GLuint perspectiveProjectionUniform;
GLuint LaUniform_Red;
GLuint LdUniform_Red;
GLuint LsUniform_Red;
GLuint lightPositionUniform_Red;
GLuint LaUniform_Blue;
GLuint LdUniform_Blue;
GLuint LsUniform_Blue;
GLuint lightPositionUniform_Blue;
GLuint LaUniform_Green;
GLuint LdUniform_Green;
GLuint LsUniform_Green;
GLuint lightPositionUniform_Green;
GLuint KaUniform;
GLuint KdUniform;
GLuint KsUniform;
GLuint KshineUniform;
GLuint LKeyPressedUniform;

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

	XStoreName(gpDisplay,gWindow,"Three Lights on Sphere in PP : Bhavesh Joshi!!");

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

	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *vertexShaderSourceCode = 
            "#version 440 core" \
            "\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_perspective_projection_matrix;" \
            "uniform vec3 u_la_red;" \
            "uniform vec3 u_ld_red;" \
            "uniform vec3 u_ls_red;" \
            "uniform vec4 u_light_position_red;" \
            "uniform vec3 u_la_blue;" \
            "uniform vec3 u_ld_blue;" \
            "uniform vec3 u_ls_blue;" \
            "uniform vec4 u_light_position_blue;" \
            "uniform vec3 u_la_green;" \
            "uniform vec3 u_ld_green;" \
            "uniform vec3 u_ls_green;" \
            "uniform vec4 u_light_position_green;" \
            "uniform vec3 u_ka;" \
            "uniform vec3 u_kd;" \
            "uniform vec3 u_ks;" \
            "uniform int u_LKeyPressed;" \
            "uniform float u_shininess;" \
            "out vec3 phong_ads_light;" \
            "void main(void)" \
            "{" \
            "if(u_LKeyPressed == 1)" \
            "{" \
            "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" \
            "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
            "vec3 view_vector = normalize(-eye_coordinates.xyz);" \

            "vec3 light_direction_red = normalize(vec3(u_light_position_red - eye_coordinates));" \
            "vec3 reflection_vector_red = reflect(-light_direction_red,transformed_normal);" \
            "vec3 ambient_red = u_la_red * u_ka;" \
            "vec3 diffuse_red = u_ld_red * u_kd * max(dot(light_direction_red,transformed_normal),0.0f);" \
            "vec3 specular_red = u_ls_red * u_ks * pow(max(dot(reflection_vector_red,view_vector),0.0f),u_shininess);" \

            "vec3 light_direction_blue = normalize(vec3(u_light_position_blue - eye_coordinates));" \
            "vec3 reflection_vector_blue = reflect(-light_direction_blue,transformed_normal);" \
            "vec3 ambient_blue = u_la_blue * u_ka;" \
            "vec3 diffuse_blue = u_ld_blue * u_kd * max(dot(light_direction_blue,transformed_normal),0.0f);" \
            "vec3 specular_blue = u_ls_blue * u_ks * pow(max(dot(reflection_vector_blue,view_vector),0.0f),u_shininess);" \

            "vec3 light_direction_green = normalize(vec3(u_light_position_green - eye_coordinates));" \
            "vec3 reflection_vector_green = reflect(-light_direction_green,transformed_normal);" \
            "vec3 ambient_green = u_la_green * u_ka;" \
            "vec3 diffuse_green = u_ld_green * u_kd * max(dot(light_direction_green,transformed_normal),0.0f);" \
            "vec3 specular_green = u_ls_green * u_ks * pow(max(dot(reflection_vector_green,view_vector),0.0f),u_shininess);" \

            "phong_ads_light = ambient_red + ambient_blue + ambient_green + diffuse_red + diffuse_blue + diffuse_green + specular_red + specular_blue + specular_green;" \
            "}" \
            "else" \
            "{" \
            "phong_ads_light = vec3(1.0f,1.0f,1.0f);" \
            "}" \
            "gl_Position = u_perspective_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
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
        "in vec3 phong_ads_light;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "FragColor = vec4(phong_ads_light,1.0f);" \
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
	glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_POSITION,"vNormal");
	
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
	
    modelMatrixUniform = glGetUniformLocation(gShaderProgramObject,"u_model_matrix");
    viewMatrixUniform = glGetUniformLocation(gShaderProgramObject,"u_view_matrix");
    perspectiveProjectionUniform = glGetUniformLocation(gShaderProgramObject,"u_perspective_projection_matrix");
    LaUniform_Red = glGetUniformLocation(gShaderProgramObject,"u_la_red");
    LdUniform_Red = glGetUniformLocation(gShaderProgramObject,"u_ld_red");
    LsUniform_Red = glGetUniformLocation(gShaderProgramObject,"u_ls_red");
    lightPositionUniform_Red = glGetUniformLocation(gShaderProgramObject,"u_light_position_red");
    LaUniform_Blue = glGetUniformLocation(gShaderProgramObject,"u_la_blue");
    LdUniform_Blue = glGetUniformLocation(gShaderProgramObject,"u_ld_blue");
    LsUniform_Blue = glGetUniformLocation(gShaderProgramObject,"u_ls_blue");
    lightPositionUniform_Blue = glGetUniformLocation(gShaderProgramObject,"u_light_position_blue");
    LaUniform_Green = glGetUniformLocation(gShaderProgramObject,"u_la_green");
    LdUniform_Green = glGetUniformLocation(gShaderProgramObject,"u_ld_green");
    LsUniform_Green = glGetUniformLocation(gShaderProgramObject,"u_ls_green");
    lightPositionUniform_Green = glGetUniformLocation(gShaderProgramObject,"u_light_position_green");
    KaUniform = glGetUniformLocation(gShaderProgramObject,"u_ka");
    KdUniform = glGetUniformLocation(gShaderProgramObject,"u_kd");
    KsUniform = glGetUniformLocation(gShaderProgramObject,"u_ks");
    KshineUniform = glGetUniformLocation(gShaderProgramObject,"u_shininess");
    LKeyPressedUniform = glGetUniformLocation(gShaderProgramObject,"u_LKeyPressed");

	getSphereVertexData(sphere_vertices,sphere_normals,sphere_textures,sphere_elements);

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

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f,0.0f,0.0f,1.0f);

	bLight = false;

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
	static GLfloat angle_pyramid = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgramObject);

        if(bLight == true)
    	{
        	lightPosition_Red[0] = 0.0f;
        	lightPosition_Red[1] = factor * sin(angleX);
        	lightPosition_Red[2] = factor * cos(angleX);
        	lightPosition_Red[3] = 1.0f;

        	lightPosition_Green[0] = factor * sin(angleY);
        	lightPosition_Green[1] = 0.0f;
        	lightPosition_Green[2] = factor * cos(angleY);
        	lightPosition_Green[3] = 1.0f;

        	lightPosition_Blue[0] = factor * sin(angleZ);
        	lightPosition_Blue[1] = factor * cos(angleZ);
        	lightPosition_Blue[2] = 0.0f;
        	lightPosition_Blue[3] = 1.0f;

        	glUniform1i(LKeyPressedUniform,1);
        	glUniform1f(KshineUniform,MaterialShininess);
        	glUniform4fv(lightPositionUniform_Red,1,lightPosition_Red);
        	glUniform3fv(LaUniform_Red,1,lightAmbient_Red);
        	glUniform3fv(LdUniform_Red,1,lightDiffuse_Red);
        	glUniform3fv(LsUniform_Red,1,lightSpecular_Red);
        	glUniform4fv(lightPositionUniform_Blue,1,lightPosition_Blue);
        	glUniform3fv(LaUniform_Blue,1,lightAmbient_Blue);
        	glUniform3fv(LdUniform_Blue,1,lightDiffuse_Blue);
        	glUniform3fv(LsUniform_Blue,1,lightSpecular_Blue);
        	glUniform4fv(lightPositionUniform_Green,1,lightPosition_Green);
        	glUniform3fv(LaUniform_Green,1,lightAmbient_Green);
        	glUniform3fv(LdUniform_Green,1,lightDiffuse_Green);
        	glUniform3fv(LsUniform_Green,1,lightSpecular_Green);
        	glUniform3fv(KaUniform,1,MaterialAmbient);
        	glUniform3fv(KdUniform,1,MaterialDiffuse);
        	glUniform3fv(KsUniform,1,MaterialSpecular);
   	 }
    	else
    	{
        	glUniform1i(LKeyPressedUniform,0);
    	}
	
    	mat4 modelMatrix = mat4::identity();
    	mat4 viewMatrix = mat4::identity();     
    	mat4 projectionMatrix = mat4::identity();
    	mat4 translateMatrix = vmath::translate(0.0f,0.0f,-3.0f);

    	modelMatrix = translateMatrix  ;   
    	projectionMatrix = perspectiveProjectionMatrix;  

    	glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    	glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    	glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);


        glBindVertexArray(vao_sphere);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
        glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);

	    angle_pyramid = angle_pyramid + 0.1f;
    if(angle_pyramid >= 360.0f)
    {
        angle_pyramid = 0.0f;
    }

    angleX = angleX + 0.01f;
    if(angleX >= 360.0f)
    {
        angleX = 0.0f;
    }

    angleY = angleY + 0.01f;
    if(angleY >= 360.0f)
    {
        angleY = 0.0f;
    }

    angleZ = angleZ + 0.01f;
    if(angleZ >= 360.0f)
    {
        angleZ = 0.0f;
    }

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
