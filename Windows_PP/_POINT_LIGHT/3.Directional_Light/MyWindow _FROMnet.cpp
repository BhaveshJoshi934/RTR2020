//HEADER FILES

#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"MyWindow.h"
#include<gl/glew.h>
#include<gl/gl.h>

#include"vmath.h"

#define       WIN_WIDTH      800
#define       WIN_HEIGHT     600

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")

using namespace vmath;

enum
{
    BDJ_ATTRIBUTE_POSITION = 0,
    BDJ_ATTRIBUTE_COLOR,
    BDJ_ATTRIBUTE_NORMAL,
    BDJ_ATTRIBUTE_TEXTURE0
};

FILE* gpFile = NULL;

bool gbFullScreen = false;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
HWND ghwnd = NULL;

bool gbActiveWindow = false;

HDC ghdc = NULL;
HGLRC ghrc = NULL;


GLfloat lightAmbient[] = {0.2f,0.2f,0.2f,1.0f};
GLfloat lightColor[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat lightDirection[] = {370.645f,163.064f,314.724f};
GLfloat halfVector[] = {0.0f,0.0f,0.0f};
GLfloat shininess = 20.0f;
GLfloat strength = 10.0f;

GLfloat z = -4.0f;


GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint vao_cube;
GLuint vbo_Position_cube;
GLuint vbo_Color_cube;
GLuint vbo_Normal_cube;

GLuint mvpUniform;
GLuint AmbientUniform;
GLuint LightColorUniform;
GLuint LightDirectionUniform;
GLuint HalfVectorUniform;
GLuint ShininessUniform;
GLuint StrengthUniform;

mat4 perspectiveProjectionMatrix;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow)
{
    void Initialize(void);
    void Display(void);

    WNDCLASSEX wndclass;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");
    HWND hwnd;
    bool bDone = false;
    int X = 0;
    int Y = 0;

    if(fopen_s(&gpFile,"Log.txt","w") != 0)
    {
        printf("Can't Open File!!!Exitting Now!!\n\n");
        exit(1);
    }
    fprintf(gpFile,"Code Starts Here!!!\n\n");

    X = GetSystemMetrics(SM_CXSCREEN)  /  2  -  WIN_WIDTH  /  2;
    Y = GetSystemMetrics(SM_CYSCREEN)  /  2  -  WIN_HEIGHT /  2;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    wndclass.cbWndExtra = 0;
    wndclass.cbClsExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));

    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                          szAppName,
                          TEXT("AMBIENT Cube in PP : Bhavesh Joshi !!"),
                          WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                          X,
                          Y,
                          WIN_WIDTH,
                          WIN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ghwnd = hwnd;

    Initialize();

    ShowWindow(hwnd,iCmdShow);

    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    while(bDone == false)
    {
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                bDone = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if(gbActiveWindow == true)
            {
                Display();
            }
        }
    }

    return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
    void ToggleFullScreen(void);
    void Resize(int,int);
    void uninitialize(void);

    switch(iMsg)
    {
    case WM_CREATE:
        break;

    case WM_KEYDOWN:
        switch(wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hwnd);
            break;

        case 0x46:
        case 0x66:
            ToggleFullScreen();
            break;

        default:
            break;
        }
        break;

    case WM_CHAR:
        switch(wParam)
        {
        case 'S':
            //lightAmbient[0] += 0.1f;
            //lightAmbient[1] += 0.1f;
            lightAmbient[2] += 0.1f;
            fprintf(gpFile,"lightAmbient[2] = %0.2f\n",lightAmbient[2]);
            break;

        case 's':
            //lightAmbient[0] -=  0.1f;
            //lightAmbient[1] -= 0.1f;
            lightAmbient[2] -= 0.1f;
            fprintf(gpFile,"lightAmbient[2] = %0.2f\n",lightAmbient[2]);
            break;

        case 'Z':
             z = z + 1.0f;
             break;

        case 'z':
            z = z - 1.0f;
            break;

        case 'D':
            lightDirection[0] += 1.0f;
            //lightDirection[1] += 1.0f;
            //lightDirection[2] += 1.0f;
            fprintf(gpFile,"X = %f\t, Y = %f\t , Z = %f\n",lightDirection[0],lightDirection[1],lightDirection[2]);
            break;

        case 'd':
            lightDirection[0] -= 1.0f;
            //lightDirection[1] -=1.0f;
            //lightDirection[2] -= 1.0f;
            fprintf(gpFile,"X = %f\t, Y = %f\t , Z = %f\n",lightDirection[0],lightDirection[1],lightDirection[2]);
            break;

        case 'H':
            halfVector[0] += 1.0f;
            halfVector[1] += 1.0f;
            halfVector[2] += 1.0f;
            fprintf(gpFile,"HV-X = %f\t, HV-Y = %f\t , HV-Z = %f\n",lightDirection[0],lightDirection[1],lightDirection[2]);
            break;

        case 'h':
            halfVector[0] -= 1.0f;
            halfVector[1] -= 1.0f;
            halfVector[2] -= 1.0f;
            fprintf(gpFile,"HV-X = %f\t, HV-Y = %f\t , HV-Z = %f\n",lightDirection[0],lightDirection[1],lightDirection[2]);
            break;

        case 'A':
            shininess += 1.0f;
            fprintf(gpFile,"Shininess = %f\n",shininess);
            break;

        case 'a':
            shininess -= 1.0f;
            fprintf(gpFile,"Shininess = %f\n",shininess);
            break;

        case 'B':
            strength += 1.0f;
            fprintf(gpFile,"Strength= %f\n",shininess);
            break;

        case 'b':
            strength -= 1.0f;
            fprintf(gpFile,"Strength = %f\n",shininess);
            break;
        }
        break;


    case WM_SIZE:
            Resize(LOWORD(lParam),HIWORD(lParam));
            break;

    case WM_SETFOCUS:
        gbActiveWindow = true;
        break;

    case WM_KILLFOCUS:
        gbActiveWindow = false;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        uninitialize();
        break;
    }

    return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}

void ToggleFullScreen()
{
    MONITORINFO mi = {sizeof(MONITORINFO)};

    if(gbFullScreen == false)
    {
        dwStyle = GetWindowLong(ghwnd,GWL_STYLE);

        if(dwStyle & WS_OVERLAPPEDWINDOW)
        {
            if(GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY),&mi))
            {
                SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos(ghwnd,HWND_TOP,mi.rcMonitor.left,mi.rcMonitor.top,mi.rcMonitor.right - mi.rcMonitor.left,mi.rcMonitor.bottom - mi.rcMonitor.top,SWP_FRAMECHANGED | SWP_NOZORDER);
            }
        }
        ShowCursor(FALSE);
        gbFullScreen = true;
    }
    else
    {
        SetWindowLong(ghwnd,GWL_STYLE,dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd,&wpPrev);
        SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE|SWP_FRAMECHANGED|SWP_NOOWNERZORDER);
        ShowCursor(TRUE);
        gbFullScreen = false;
    }
}

void Initialize()
{
    void uninitialize();
    void Resize(int,int);

    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex;

    ghdc = GetDC(ghwnd);
    ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cBlueBits = 8;
    pfd.cGreenBits = 8;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;

    iPixelFormatIndex = ChoosePixelFormat(ghdc,&pfd);
    if(iPixelFormatIndex == 0)
    {
        fprintf(gpFile,"ChoosePixelFormat() Failed!!!");
        DestroyWindow(ghwnd);
    }

    if(SetPixelFormat(ghdc,iPixelFormatIndex,&pfd) == FALSE)
    {
        fprintf(gpFile,"SetPixelFormat() Failed!!!");
        DestroyWindow(ghwnd);
    }

    ghrc = wglCreateContext(ghdc);

    if(ghrc == NULL)
    {
        fprintf(gpFile,"wglCreateContext() Failed!!!");
        DestroyWindow(ghwnd);
    }

    if(wglMakeCurrent(ghdc,ghrc) == FALSE)
    {
        fprintf(gpFile,"wglMakeCurrent() Failed!!!");
        DestroyWindow(ghwnd);
    }

    GLenum glew_error = glewInit();
    if(glew_error != GLEW_OK)
    {
        wglDeleteContext(ghrc);
        ghrc = NULL;
        ReleaseDC(ghwnd,ghdc);
        ghdc = NULL;
    }

    fprintf(gpFile,"OpenGL Vendor : %s\n\n",glGetString(GL_VENDOR));
    fprintf(gpFile,"OpenGL Renderer : %s\n\n",glGetString(GL_RENDERER));
    fprintf(gpFile,"OpenGL Version : %s\n\n",glGetString(GL_VERSION));
    fprintf(gpFile,"OpenGL GLSL Version : %s\n\n\n\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

    GLint numExt;

    glGetIntegerv(GL_NUM_EXTENSIONS,&numExt);

    for(int i = 0 ; i < numExt ; i++)
    {
        fprintf(gpFile,"%s\n\n",glGetStringi(GL_EXTENSIONS,i));
    }

    /*--------------------------------------------VERTEX SHADER-------------------------------------------------------------*/

    //Create Shader
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    //Feed Shader
    const GLchar *vertexShaderSourceCode =
            "#version 440 core" \
            "\n" \
            "in vec4 vPosition;" \
            "in vec4 vColor;" \
            "in vec3 vNormal;" \
            "uniform mat4 u_mvp_matrix;" \
            "out vec4 out_color;" \
            "out vec3 transformed_normal;" \
            "void main(void)" \
            "{" \
            "transformed_normal = mat3(u_mvp_matrix) * vNormal;" \
            "gl_Position = u_mvp_matrix * vPosition;" \
            "out_color = vColor;" \
            "}";

    glShaderSource(gVertexShaderObject,1,(const GLchar **)&vertexShaderSourceCode,NULL);

    //Compile Shader
    glCompileShader(gVertexShaderObject);

    GLint infoLogLength = 0;
    GLint shaderCompiledStatus = 0;
    char *szInfoLog = NULL;
    glGetShaderiv(gVertexShaderObject,GL_COMPILE_STATUS,&shaderCompiledStatus);
    if(shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(gVertexShaderObject,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gVertexShaderObject,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Vertex Shader Compilation Log = %s\n\n",szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    /*--------------------------------------------FRAGMENT SHADER-------------------------------------------------------------*/

    //Create Shader
    gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    //Feed Shader
    const GLchar *fragmentShaderSourceCode =
        "#version 440 core" \
        "\n" \
        "in vec3 transformed_normal;" \
        "in vec4 out_color;" \
        "uniform vec3 u_Ambient;" \
        "uniform vec3 u_LightColor;" \
        "uniform vec3 u_LightDirection;" \
        "uniform vec3 u_HalfVector;" \
        "uniform float u_Shininess;" \
        "uniform float u_Strength;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "vec3 normalized_transformed_normal = normalize(transformed_normal);" \
        "float diffuse = max(0.0f,dot(normalized_transformed_normal,u_LightDirection));" \
        "float specular = max(0.0f,dot(normalized_transformed_normal,u_HalfVector));" \
        "if(diffuse == 0.0f)" \
        "{" \
        "specular = 0.0f;" \
        "}" \
        "else" \
        "{" \
        "specular = pow(specular,u_Shininess);" \
        "}" \
        "vec3 scatteredLight = u_Ambient + u_LightColor * diffuse;" \
        "vec3 reflectedLight = u_LightColor * specular * u_Strength;" \
        "vec3 rgb = min(out_color.rgb * scatteredLight + reflectedLight,vec3(1.0f));" \
        "FragColor = vec4(rgb,out_color.a);" \
        "}";

    glShaderSource(gFragmentShaderObject,1,(const char **)&fragmentShaderSourceCode,NULL);

    //Compile
    glCompileShader(gFragmentShaderObject);

    glGetShaderiv(gFragmentShaderObject,GL_COMPILE_STATUS,&shaderCompiledStatus);
    if(shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(gFragmentShaderObject,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Fragment Shader Compilation Log = %s\n\n",szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }


    /*--------------------------------------------SHADER PROGRAM-------------------------------------------------------------*/

    //Create
    gShaderProgramObject = glCreateProgram();

    //Attach
    glAttachShader(gShaderProgramObject,gVertexShaderObject);
    glAttachShader(gShaderProgramObject,gFragmentShaderObject);

    glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_POSITION,"vPosition");
    glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_COLOR,"vColor");
    glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_NORMAL,"vNormal");

    //Link
    glLinkProgram(gShaderProgramObject);

    GLint shaderProgramLinkStatus = 0;
    glGetProgramiv(gShaderProgramObject,GL_LINK_STATUS,&shaderProgramLinkStatus);
    if(shaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gShaderProgramObject,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(gFragmentShaderObject,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Shader Program Link Log = %s\n\n",szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    mvpUniform = glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");
    AmbientUniform = glGetUniformLocation(gShaderProgramObject,"u_Ambient");
    LightColorUniform = glGetUniformLocation(gShaderProgramObject,"u_LightColor");
    LightDirectionUniform = glGetUniformLocation(gShaderProgramObject,"u_LightDirection");
    HalfVectorUniform = glGetUniformLocation(gShaderProgramObject,"u_HalfVector");
    ShininessUniform = glGetUniformLocation(gShaderProgramObject,"u_Shininess");
    StrengthUniform = glGetUniformLocation(gShaderProgramObject,"u_Strength");

    const GLfloat cubeVertices[] =
            {
                0.5f,0.5f,0.5f,
                -0.5f,0.5f,0.5f,
                -0.5f,-0.5f,0.5f,
                0.5f,-0.5f,0.5f,

                0.5f,0.5f,-0.5f,
                0.5f,0.5f,0.5f,
                0.5f,-0.5f,0.5f,
                0.5f,-0.5f,-0.5f,

                -0.5f,0.5f,-0.5f,
                0.5f,0.5f,-0.5f,
                0.5f,-0.5f,-0.5f,
                -0.5f,-0.5f,-0.5f,

                -0.5f,0.5f,0.5f,
                -0.5f,0.5f,-0.5f,
                -0.5f,-0.5f,-0.5f,
                -0.5f,-0.5f,0.5f,

                0.5f,0.5f,-0.5f,
                -0.5f,0.5f,-0.5f,
                -0.5f,0.5f,0.5f,
                0.5f,0.5f,0.5f,

                0.5f,-0.5f,-0.5f,
                -0.5f,-0.5f,-0.5f,
                -0.5f,-0.5f,0.5f,
                0.5f,-0.5f,0.5f
            };

    const GLfloat CubeColor[] =
    {
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,

        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,

        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,

        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,

        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,

        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
    };

        const GLfloat cubeNormal[] =
        {
        0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
        };

    glGenVertexArrays(1,&vao_cube);
    glBindVertexArray(vao_cube);

    //POSITION
    glGenBuffers(1,&vbo_Position_cube);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_Position_cube);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(BDJ_ATTRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenBuffers(1,&vbo_Normal_cube);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_Normal_cube);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeNormal),cubeNormal,GL_STATIC_DRAW);
    glVertexAttribPointer(BDJ_ATTRIBUTE_NORMAL,3,GL_FLOAT,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenBuffers(1,&vbo_Color_cube);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_Color_cube);
    glBufferData(GL_ARRAY_BUFFER,sizeof(CubeColor),CubeColor,GL_STATIC_DRAW);
    glVertexAttribPointer(BDJ_ATTRIBUTE_COLOR,3,GL_FLOAT,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glClearColor(0.0f,0.0f,0.0f,0.0f);

    perspectiveProjectionMatrix = mat4::identity();

    Resize(WIN_WIDTH,WIN_HEIGHT);
}

void Resize(int width,int height)
{
    if(height == 0)
        height = 1;

    glViewport(0,0,(GLsizei)width,(GLsizei)height);

    perspectiveProjectionMatrix = vmath::perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}

void Display()
{
    static GLfloat angle_cube = 0.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Start Using OpenGL Program
    glUseProgram(gShaderProgramObject);

    //FOR TRIANGLE

    mat4 modelViewMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();
    mat4 translateMatrix = vmath::translate(0.0f,0.0f,z);
    mat4 rotationMatrix1 = mat4::identity();
    mat4 rotationMatrix2 = mat4::identity();
    mat4 rotationMatrix3 = mat4::identity();
    rotationMatrix1 = vmath::rotate((GLfloat)angle_cube,1.0f,0.0f,0.0f);
    rotationMatrix2 = vmath::rotate((GLfloat)angle_cube,0.0f,1.0f,0.0f);
    rotationMatrix3 = vmath::rotate((GLfloat)angle_cube,0.0f,0.0f,1.0f);

    modelViewMatrix = translateMatrix * rotationMatrix1 * rotationMatrix2 * rotationMatrix3 ;
    //modelViewMatrix = translateMatrix  ;
    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    //glUniform4fv(LightPositionUniform,1,lightPosition);
    glUniform3fv(AmbientUniform,1,lightAmbient);
    glUniform3fv(LightDirectionUniform,1,lightDirection);
    glUniform3fv(LightColorUniform,1,lightColor);
    glUniform3fv(HalfVectorUniform,1,halfVector);
    glUniform1f(ShininessUniform,shininess);
    glUniform1f(StrengthUniform,strength);
    glUniformMatrix4fv(mvpUniform,1,GL_FALSE,modelViewProjectionMatrix);
    //glUniformMatrix4fv(AmbientUniform,1,GL_FALSE,modelViewProjectionMatrix);

    glBindVertexArray(vao_cube);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDrawArrays(GL_TRIANGLE_FAN,4,4);
    glDrawArrays(GL_TRIANGLE_FAN,8,4);
    glDrawArrays(GL_TRIANGLE_FAN,16,4);
    glDrawArrays(GL_TRIANGLE_FAN,20,4);
    glDrawArrays(GL_TRIANGLE_FAN,24,4);

    glBindVertexArray(0);

    //Stop OpenGL Program
    glUseProgram(0);

    angle_cube = angle_cube + 0.1f;
    if(angle_cube >= 360.0f)
    {
        angle_cube = 0.0f;
    }

    SwapBuffers(ghdc);
}

void uninitialize()
{
	if (gbFullScreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
    /*
    glDetachShader(gShaderProgramObject,gVertexShaderObject);
    glDetachShader(gShaderProgramObject,gFragmentShaderObject);

    glDeleteShader(gVertexShaderObject);
    gVertexShaderObject = 0;

    glDeleteShader(gFragmentShaderObject);
    gFragmentShaderObject = 0;

    glUseProgram(0);
    */

    if(vao_cube)
    {
        glDeleteVertexArrays(1,&vao_cube);
        vao_cube = 0;
    }

    if(vbo_Position_cube)
    {
        glDeleteBuffers(1,&vbo_Position_cube);
        vbo_Position_cube = 0;
    }

    if(vbo_Color_cube)
    {
        glDeleteBuffers(1,&vbo_Color_cube);
        vbo_Color_cube = 0;
    }

    if(gShaderProgramObject)
    {
        glUseProgram(gShaderProgramObject);
        GLsizei shaderCount;

        glGetProgramiv(gShaderProgramObject,GL_ATTACHED_SHADERS,&shaderCount);

        GLuint *pShaders = NULL;

        pShaders = (GLuint*)malloc(shaderCount * sizeof(GLuint));
        if(pShaders == NULL)
        {
            printf("Malloc Failed!!!Exitting Now!!\n\n");
            exit(0);
        }

        glGetAttachedShaders(gShaderProgramObject,shaderCount,&shaderCount,pShaders);

        for(GLsizei i = 0 ; i < shaderCount ; i++)
        {
            glDetachShader(gShaderProgramObject,pShaders[i]);
            glDeleteShader(pShaders[i]);
            pShaders[i] = 0;
            free(pShaders);

            glDeleteProgram(gShaderProgramObject);
            gShaderProgramObject = 0;
            glUseProgram(0);
        }
    }

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	if (gpFile)
	{
	    fprintf(gpFile,"Code Ends Here !!!\n\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}
