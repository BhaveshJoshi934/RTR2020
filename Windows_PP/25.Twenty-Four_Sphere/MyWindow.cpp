//HEADER FILES

#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"MyWindow.h"
#include<gl/glew.h>
#include<gl/gl.h>
#include"Sphere.h"

#include"vmath.h"

#define       WIN_WIDTH      800
#define       WIN_HEIGHT     600

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Sphere.lib")

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

int Key_1;
int Key_2;
int Key_3;

HDC ghdc = NULL;
HGLRC ghrc = NULL;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

bool bLight;
bool FragmentShader = false;

GLfloat materialAmbient[4];
GLfloat materialDiffused[4];
GLfloat materialSpecular[4];
GLfloat materialShininess;

GLuint gVao_sphere;
GLuint gNumVertices;
GLuint gNumElements;
GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

GLfloat factor = 10.0f;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.0f;

GLfloat lightPosition[] = {0.0f,0.0,0.0f,1.0f};
GLfloat lightAmbient[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse[] = {1.0f,1.0f,1.0f,1.0f};

GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint perspectiveProjectionUniform;
GLuint LaUniform;
GLuint LdUniform;
GLuint LsUniform;
GLuint lightPositionUniform;
GLuint KaUniform;
GLuint KdUniform;
GLuint KsUniform;
GLuint KshineUniform;
GLuint LKeyPressedUniform;

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
                          TEXT("Twenty-Four Sphere in PP : Bhavesh Joshi !!"),
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

    case WM_CHAR:
        switch(wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hwnd);
            break;

        case 0X46:
        case 0X66:
            ToggleFullScreen();
            break;

        case 'L':
        case 'l':
            if(bLight == true)
            {
                bLight = false;
            }
            else
            {
                bLight = true;
            }
            break;

        case 'X':
        case 'x':
            Key_1 = 1;
            angleX = 0.0f;
            break;

        case 'Y':
        case 'y':
            Key_2 = 2;
            angleY = 0.0f;
            break;

        case 'Z':
        case 'z':
            Key_3 = 3;
            angleZ = 0.0f;
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

 //    /*--------------------------------------------VERTEX SHADER-------------------------------------------------------------*/

    //Create Shader
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    //Feed Shader
    const GLchar *vertexShaderSourceCode =
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
            "out vec3 view_vector;" \
            "out vec3 light_direction;" \
            "void main(void)" \
            "{" \
            "if(u_LKeyPressed == 1)" \
            "{" \
            "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" \
            "transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal;" \
            "view_vector = -eye_coordinates.xyz;" \
            "light_direction = vec3(u_light_position - eye_coordinates);" \
            "}" \
            "gl_Position = u_perspective_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
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

 //   /*--------------------------------------------FRAGMENT SHADER-------------------------------------------------------------

    //Create Shader
    gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    //Feed Shader
    const GLchar *fragmentShaderSourceCode =
        "#version 440 core" \
        "\n" \
        "in vec3 transformed_normal;" \
        "in vec3 view_vector;" \
        "in vec3 light_direction;" \
        "uniform vec3 u_la;" \
        "uniform vec3 u_ld;" \
        "uniform vec3 u_ls;" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform int u_LKeyPressed;" \
        "uniform float u_shininess;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
        "vec3 phong_ads_light;" \
        "if(u_LKeyPressed == 1)" \
        "{" \
        "vec3 normalized_transformed_normal      = normalize(transformed_normal);" \
        "vec3 normalized_light_direction         = normalize(light_direction);" \
        "vec3 normalized_view_vector             = normalize(view_vector);" \

        "vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normal);" \
        "vec3 ambient = u_la * u_ka;" \
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction,normalized_transformed_normal),0.0f);" \
        "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector,normalized_view_vector ),0.0f),u_shininess);" \

        "phong_ads_light = ambient + diffuse + specular;" \
        "}" \
        "else" \
        "{" \
        "phong_ads_light = vec3(1.0f,1.0f,1.0f);" \
        "}" \
        "FragColor = vec4(phong_ads_light,1.0f);" \
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


//    /*--------------------------------------------SHADER PROGRAM-------------------------------------------------------------

    //Create
    gShaderProgramObject = glCreateProgram();

    //Attach
    glAttachShader(gShaderProgramObject,gVertexShaderObject);
    glAttachShader(gShaderProgramObject,gFragmentShaderObject);

    glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_POSITION,"vPosition");
    glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_NORMAL,"vNormal");

    //Link
    glLinkProgram(gShaderProgramObject);

    //-------------------------------------
    GLint shaderProgramLinkStatus = 0;
    //-------------------------------------
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

//    mvpUniform = glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");

    modelMatrixUniform = glGetUniformLocation(gShaderProgramObject,"u_model_matrix");
    viewMatrixUniform = glGetUniformLocation(gShaderProgramObject,"u_view_matrix");
    perspectiveProjectionUniform = glGetUniformLocation(gShaderProgramObject,"u_perspective_projection_matrix");
    LaUniform = glGetUniformLocation(gShaderProgramObject,"u_la");
    LdUniform = glGetUniformLocation(gShaderProgramObject,"u_ld");
    LsUniform = glGetUniformLocation(gShaderProgramObject,"u_ls");
    lightPositionUniform = glGetUniformLocation(gShaderProgramObject,"u_light_position");
    KaUniform = glGetUniformLocation(gShaderProgramObject,"u_ka");
    KdUniform = glGetUniformLocation(gShaderProgramObject,"u_kd");
    KsUniform = glGetUniformLocation(gShaderProgramObject,"u_ks");
    KshineUniform = glGetUniformLocation(gShaderProgramObject,"u_shininess");
    LKeyPressedUniform = glGetUniformLocation(gShaderProgramObject,"u_LKeyPressed");

    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    gNumVertices = getNumberOfSphereVertices();
    gNumElements = getNumberOfSphereElements();

    // vao
    glGenVertexArrays(1, &gVao_sphere);
    glBindVertexArray(gVao_sphere);

    // position vbo
    glGenBuffers(1, &gVbo_sphere_position);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(BDJ_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // normal vbo
    glGenBuffers(1, &gVbo_sphere_normal);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(BDJ_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // element vbo
    glGenBuffers(1, &gVbo_sphere_element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // unbind vao
    glBindVertexArray(0);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glClearColor(0.25f,0.25f,0.25f,0.25f);

    bLight = false;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.0215;
    materialAmbient[1] = 0.1745;
    materialAmbient[2] = 0.0215;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.07568;
    materialDiffused[1] = 0.61424;
    materialDiffused[2] = 0.07568;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.633;
    materialSpecular[1] = 0.727811;
    materialSpecular[2] = 0.633;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.6 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();     // view la Identity
    mat4 projectionMatrix = mat4::identity();
    mat4 translateMatrix = vmath::translate(-6.0f,3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);
//---------------------------------------------------------------------------------------------------------------------------
    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.135;
    materialAmbient[1] = 0.2225;
    materialAmbient[2] = 0.1575;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.54;
    materialDiffused[1] = 0.89;
    materialDiffused[2] = 0.63;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.316228;
    materialSpecular[1] = 0.316228;
    materialSpecular[2] = 0.316228;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.1 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-3.5f,3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);
//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.05375;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.06625;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.18275;
    materialDiffused[1] = 0.17;
    materialDiffused[2] = 0.22525;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.332741;
    materialSpecular[1] = 0.328634;
    materialSpecular[2] = 0.346435;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.3 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-1.0f,3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.25;
    materialAmbient[1] = 0.20725;
    materialAmbient[2] = 0.20725;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 1.0;
    materialDiffused[1] = 0.829;
    materialDiffused[2] = 0.829;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.296648;
    materialSpecular[1] = 0.296648;
    materialSpecular[2] = 0.296648;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.088 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(1.5f,3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.1745;
    materialAmbient[1] = 0.01175;
    materialAmbient[2] = 0.01175;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.61424;
    materialDiffused[1] = 0.04136;
    materialDiffused[2] = 0.04136;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.727811;
    materialSpecular[1] = 0.626959;
    materialSpecular[2] = 0.626959;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.6 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(3.7f,3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------
    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.1;
    materialAmbient[1] = 0.18725;
    materialAmbient[2] = 0.1745;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.396;
    materialDiffused[1] = 0.74151;
    materialDiffused[2] = 0.69102;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.297254;
    materialSpecular[1] = 0.30829;
    materialSpecular[2] = 0.306678;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.1 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(6.0f,3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------
    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.329412;
    materialAmbient[1] = 0.223529;
    materialAmbient[2] = 0.027451;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.780392;
    materialDiffused[1] = 0.568627;
    materialDiffused[2] = 0.113725;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.992157;
    materialSpecular[1] = 0.941176;
    materialSpecular[2] = 0.807843;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.21794872 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-6.0f,1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.1;
    materialAmbient[1] = 0.18725;
    materialAmbient[2] = 0.1745;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.396;
    materialDiffused[1] = 0.74151;
    materialDiffused[2] = 0.69102;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.297254;
    materialSpecular[1] = 0.30829;
    materialSpecular[2] = 0.306678;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.1 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-3.5f,1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.2125;
    materialAmbient[1] = 0.1275;
    materialAmbient[2] = 0.054;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.714;
    materialDiffused[1] = 0.4284;
    materialDiffused[2] = 0.18144;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.393548;
    materialSpecular[1] = 0.271906;
    materialSpecular[2] = 0.166721;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.2 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-1.0f,1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.25;
    materialAmbient[1] = 0.25;
    materialAmbient[2] = 0.25;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.4;
    materialDiffused[1] = 0.4;
    materialDiffused[2] = 0.4;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.774597;
    materialSpecular[1] = 0.774597;
    materialSpecular[2] = 0.774597;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.6 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(1.5f,1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.19125;
    materialAmbient[1] = 0.0735;
    materialAmbient[2] = 0.0225;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.7038;
    materialDiffused[1] = 0.27048;
    materialDiffused[2] = 0.0828;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.256777;
    materialSpecular[1] = 0.137622;
    materialSpecular[2] = 0.086014;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.1 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(3.7f,1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.24725;
    materialAmbient[1] = 0.1995;
    materialAmbient[2] = 0.0745;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.75164;
    materialDiffused[1] = 0.60648;
    materialDiffused[2] = 0.22648;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.628281;
    materialSpecular[1] = 0.555802;
    materialSpecular[2] = 0.366065;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.4 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(6.0f,1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.19225;
    materialAmbient[1] = 0.19225;
    materialAmbient[2] = 0.19225;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.50754;
    materialDiffused[1] = 0.50754;
    materialDiffused[2] = 0.50754;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.508273;
    materialSpecular[1] = 0.508273;
    materialSpecular[2] = 0.508273;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.4 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-6.0f,-1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.01;
    materialDiffused[1] = 0.01;
    materialDiffused[2] = 0.01;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.50;
    materialSpecular[1] = 0.50;
    materialSpecular[2] = 0.50;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-3.5f,-1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.1;
    materialAmbient[2] = 0.06;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.0;
    materialDiffused[1] = 0.50980392;
    materialDiffused[2] = 0.50980392;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.50196078;
    materialSpecular[1] = 0.50196078;
    materialSpecular[2] = 0.50196078;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-1.0f,-1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.1;
    materialDiffused[1] = 0.35;
    materialDiffused[2] = 0.1;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.45;
    materialSpecular[1] = 0.55;
    materialSpecular[2] = 0.45;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(1.5f,-1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(3.7f,-1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.55;
    materialDiffused[1] = 0.55;
    materialDiffused[2] = 0.55;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.70;
    materialSpecular[1] = 0.70;
    materialSpecular[2] = 0.70;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(6.0f,-1.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.5;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.0;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.60;
    materialSpecular[1] = 0.60;
    materialSpecular[2] = 0.50;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.25 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-6.0f,-3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

     materialAmbient[0] = 0.02;
    materialAmbient[1] = 0.02;
    materialAmbient[2] = 0.02;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.01;
    materialDiffused[1] = 0.01;
    materialDiffused[2] = 0.01;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.4;
    materialSpecular[1] = 0.4;
    materialSpecular[2] = 0.4;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    materialShininess = 0.25 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-3.5f,-3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.05;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.4;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.5;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.04;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.7;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(-1.0f,-3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.0;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.4;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.4;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.04;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.04;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(1.5f,-3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.0;
    materialAmbient[2] = 0.0;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.5;
    materialDiffused[1] = 0.4;
    materialDiffused[2] = 0.4;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.04;
    materialSpecular[2] = 0.04;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(4.0f,-3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------

    glUseProgram(gShaderProgramObject);

    if(bLight == true)
    {
        if(Key_1 == 1)
        {
            angleX = angleX + 0.01f;
            lightPosition[0] = 0.0f;
            lightPosition[1] = factor * sin(angleX);
            lightPosition[2] = factor * cos(angleX);
            lightPosition[3] = 1.0f;
        }

        else if(Key_2 == 2)
        {
            angleY = angleY + 0.01f;
            lightPosition[0] = factor * sin(angleY);
            lightPosition[1] = 0.0f;
            lightPosition[2] = factor * cos(angleY);
            lightPosition[3] = 1.0f;
        }

        else if(Key_3 == 3)
        {
            angleZ = angleZ + 0.01f;
            lightPosition[0] = factor * sin(angleZ);
            lightPosition[1] = factor * cos(angleZ);
            lightPosition[2] = 0.0f;
            lightPosition[3] = 1.0f;
        }

        glUniform1i(LKeyPressedUniform,1);
        glUniform1f(KshineUniform,materialShininess);
        glUniform4fv(lightPositionUniform,1,lightPosition);
        glUniform3fv(LaUniform,1,lightAmbient);
        glUniform3fv(LdUniform,1,lightDiffuse);
        glUniform3fv(KaUniform,1,materialAmbient);
        glUniform3fv(KdUniform,1,materialDiffused);
        glUniform3fv(KsUniform,1,materialSpecular);
    }
    else
    {
        glUniform1i(LKeyPressedUniform,0);
    }

    glBindVertexArray(gVao_sphere);

    materialAmbient[0] = 0.05;
    materialAmbient[1] = 0.05;
    materialAmbient[2] = 0.05;
    materialAmbient[3] = 1.0f;

    materialDiffused[0] = 0.5;
    materialDiffused[1] = 0.5;
    materialDiffused[2] = 0.5;
    materialDiffused[3] = 1.0f;

    materialSpecular[0] = 0.7;
    materialSpecular[1] = 0.7;
    materialSpecular[2] = 0.7;
    materialSpecular[3] = 1.0f;

    materialShininess = 0.078125 * 128;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    translateMatrix = vmath::translate(6.0f,-3.0f,-10.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,projectionMatrix);

    glBindVertexArray(0);

    glUseProgram(0);

//-------------------------------------------------------------------------------------------------------------


    if(angleX >= 360.0f)
    {
        angleX = 0.0f;
    }

    if(angleY >= 360.0f)
    {
        angleY = 0.0f;
    }

    if(angleZ >= 360.0f)
    {
        angleZ = 0.0f;
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

    if(gVao_sphere)
    {
        glDeleteBuffers(1,&gVao_sphere);
        gVao_sphere = 0;
    }

    if(gVbo_sphere_normal)
    {
        glDeleteBuffers(1,&gVbo_sphere_normal);
        gVbo_sphere_normal = 0;
    }

    if(gVbo_sphere_position)
    {
        glDeleteBuffers(1,&gVbo_sphere_position);
        gVbo_sphere_position = 0;
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
