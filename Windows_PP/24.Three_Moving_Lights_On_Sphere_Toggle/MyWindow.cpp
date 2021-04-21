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

HDC ghdc = NULL;
HGLRC ghrc = NULL;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint gVertexShaderObject_PV;
GLuint gFragmentShaderObject_PV;
GLuint gShaderProgramObject_PV;

GLuint gVertexShaderObject_PF;
GLuint gFragmentShaderObject_PF;
GLuint gShaderProgramObject_PF;

bool bLight;
bool FragmentShader = false;

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

GLfloat lightPosition_Red_PV[] = {0.0f,0.0,0.0f,1.0f};
GLfloat lightAmbient_Red_PV[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Red_PV[] = {1.0f,0.0f,0.0f,1.0f};
GLfloat lightSpecular_Red_PV[] = {1.0f,0.0f,0.0f,1.0f};

GLfloat lightPosition_Blue_PV[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightAmbient_Blue_PV[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Blue_PV[] = {0.0f,0.0f,1.0f,1.0f};
GLfloat lightSpecular_Blue_PV[] = {0.0f,0.0f,1.0f,1.0f};

GLfloat lightPosition_Green_PV[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightAmbient_Green_PV[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Green_PV[] = {0.0f,1.0f,0.0f,1.0f};
GLfloat lightSpecular_Green_PV[] = {0.0f,1.0f,0.0f,1.0f};

GLfloat MaterialAmbient_PV[] = {0.0f,0.0f,0.0f,0.0f};
GLfloat MaterialDiffuse_PV[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat MaterialSpecular_PV[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat MaterialShininess_PV = 50.0f;

GLfloat lightPosition_Red_PF[] = {0.0f,0.0,0.0f,1.0f};
GLfloat lightAmbient_Red_PF[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Red_PF[] = {1.0f,0.0f,0.0f,1.0f};
GLfloat lightSpecular_Red_PF[] = {1.0f,0.0f,0.0f,1.0f};

GLfloat lightPosition_Blue_PF[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightAmbient_Blue_PF[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Blue_PF[] = {0.0f,0.0f,1.0f,1.0f};
GLfloat lightSpecular_Blue_PF[] = {0.0f,0.0f,1.0f,1.0f};

GLfloat lightPosition_Green_PF[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightAmbient_Green_PF[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat lightDiffuse_Green_PF[] = {0.0f,1.0f,0.0f,1.0f};
GLfloat lightSpecular_Green_PF[] = {0.0f,1.0f,0.0f,1.0f};

GLfloat MaterialAmbient_PF[] = {0.0f,0.0f,0.0f,0.0f};
GLfloat MaterialDiffuse_PF[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat MaterialSpecular_PF[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat MaterialShininess_PF = 128.0f;

GLuint modelMatrixUniform_PV;
GLuint viewMatrixUniform_PV;
GLuint perspectiveProjectionUniform_PV;
GLuint LaUniform_Red_PV;
GLuint LdUniform_Red_PV;
GLuint LsUniform_Red_PV;
GLuint lightPositionUniform_Red_PV;
GLuint LaUniform_Blue_PV;
GLuint LdUniform_Blue_PV;
GLuint LsUniform_Blue_PV;
GLuint lightPositionUniform_Blue_PV;
GLuint LaUniform_Green_PV;
GLuint LdUniform_Green_PV;
GLuint LsUniform_Green_PV;
GLuint lightPositionUniform_Green_PV;
GLuint KaUniform_PV;
GLuint KdUniform_PV;
GLuint KsUniform_PV;
GLuint KshineUniform_PV;
GLuint LKeyPressedUniform_PV;

GLuint modelMatrixUniform_PF;
GLuint viewMatrixUniform_PF;
GLuint perspectiveProjectionUniform_PF;
GLuint LaUniform_Red_PF;
GLuint LdUniform_Red_PF;
GLuint LsUniform_Red_PF;
GLuint lightPositionUniform_Red_PF;
GLuint LaUniform_Blue_PF;
GLuint LdUniform_Blue_PF;
GLuint LsUniform_Blue_PF;
GLuint lightPositionUniform_Blue_PF;
GLuint LaUniform_Green_PF;
GLuint LdUniform_Green_PF;
GLuint LsUniform_Green_PF;
GLuint lightPositionUniform_Green_PF;
GLuint KaUniform_PF;
GLuint KdUniform_PF;
GLuint KsUniform_PF;
GLuint KshineUniform_PF;
GLuint LKeyPressedUniform_PF;

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
                          TEXT("Three Rotating Lights on Steady Sphere Toggling in PP : Bhavesh Joshi !!"),
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
        case 'Q':
        case 'q':
            DestroyWindow(hwnd);
            break;

        case VK_ESCAPE:
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

        case 'V':
        case 'v':
            FragmentShader = false;
            break;

        case 'F':
        case 'f':
            FragmentShader = true ;
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

     //--------------------------------------------VERTEX SHADER-------------------------------------------------------------/

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

    fprintf(gpFile,"Vertex_PV\n\n");

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
                DestroyWindow(ghwnd);
            }
        }
    }
    fprintf(gpFile,"Vertex_PV_End\n\n");

//--------------------------------------------FRAGMENT SHADER-------------------------------------------------------------

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
    fprintf(gpFile,"Fragment_PV\n\n");
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
                DestroyWindow(ghwnd);
            }
        }
    }


//    *--------------------------------------------SHADER PROGRAM-------------------------------------------------------------

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
                DestroyWindow(ghwnd);
            }
        }
    }

//    mvpUniform = glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");

    modelMatrixUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_model_matrix");
    viewMatrixUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_view_matrix");
    perspectiveProjectionUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_perspective_projection_matrix");
    LaUniform_Red_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_la_red");
    LdUniform_Red_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ld_red");
    LsUniform_Red_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ls_red");
    lightPositionUniform_Red_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_light_position_red");
    LaUniform_Blue_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_la_blue");
    LdUniform_Blue_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ld_blue");
    LsUniform_Blue_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ls_blue");
    lightPositionUniform_Blue_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_light_position_blue");
    LaUniform_Green_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_la_green");
    LdUniform_Green_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ld_green");
    LsUniform_Green_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ls_green");
    lightPositionUniform_Green_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_light_position_green");
    KaUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ka");
    KdUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_kd");
    KsUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_ks");
    KshineUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_shininess");
    LKeyPressedUniform_PV = glGetUniformLocation(gShaderProgramObject_PV,"u_LKeyPressed");

    fprintf(gpFile,"Fragment_PV_End\n\n");


 //    /*--------------------------------------------VERTEX SHADER-------------------------------------------------------------*/

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
            "uniform vec4 u_light_position_red;" \
            "uniform vec4 u_light_position_blue;" \
            "uniform vec4 u_light_position_green;" \
            "uniform int u_LKeyPressed;" \
            "out vec3 transformed_normal;" \
            "out vec3 view_vector;" \
            "out vec3 light_direction_red;" \
            "out vec3 light_direction_blue;" \
            "out vec3 light_direction_green;" \
            "void main(void)" \
            "{" \
            "if(u_LKeyPressed == 1)" \
            "{" \
            "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" \
            "transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal;" \
            "view_vector = -eye_coordinates.xyz;" \
            "light_direction_red = vec3(u_light_position_red - eye_coordinates);" \
            "light_direction_blue = vec3(u_light_position_blue - eye_coordinates);" \
            "light_direction_green = vec3(u_light_position_green - eye_coordinates);" \
            "}" \
            "gl_Position = u_perspective_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
            "}";

            fprintf(gpFile,"Vertex_PF\n\n");

    glShaderSource(gVertexShaderObject_PF,1,(const GLchar **)&vertexShaderSourceCode_PF,NULL);

    //Compile Shader
    glCompileShader(gVertexShaderObject_PF);
/*-------------------------------------------------------
    GLint infoLogLength = 0;
    GLint shaderCompiledStatus = 0;
    char *szInfoLog = NULL;
*/
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
                DestroyWindow(ghwnd);
            }
        }
    }

    fprintf(gpFile,"Veretx_PF_End\n\n");
 //   /*--------------------------------------------FRAGMENT SHADER-------------------------------------------------------------

    //Create Shader
    gFragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

    //Feed Shader
    const GLchar *fragmentShaderSourceCode_PF =
        "#version 440 core" \
        "\n" \
        "in vec3 transformed_normal;" \
        "in vec3 view_vector;" \
        "in vec3 light_direction_red;" \
        "in vec3 light_direction_blue;" \
        "in vec3 light_direction_green;" \
        "uniform vec3 u_la_red;" \
        "uniform vec3 u_ld_red;" \
        "uniform vec3 u_ls_red;" \
        "uniform vec3 u_la_blue;" \
        "uniform vec3 u_ld_blue;" \
        "uniform vec3 u_ls_blue;" \
        "uniform vec3 u_la_green;" \
        "uniform vec3 u_ld_green;" \
        "uniform vec3 u_ls_green;" \
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
        "vec3 normalized_light_direction_red     = normalize(light_direction_red);" \
        "vec3 normalized_light_direction_blue    = normalize(light_direction_blue);" \
        "vec3 normalized_light_direction_green   = normalize(light_direction_green);" \
        "vec3 normalized_view_vector             = normalize(view_vector);" \

        "vec3 reflection_vector_red = reflect(-normalized_light_direction_red,normalized_transformed_normal);" \
        "vec3 ambient_red = u_la_red * u_ka;" \
        "vec3 diffuse_red = u_ld_red * u_kd * max(dot(normalized_light_direction_red,normalized_transformed_normal),0.0f);" \
        "vec3 specular_red = u_ls_red * u_ks * pow(max(dot(reflection_vector_red,normalized_view_vector ),0.0f),u_shininess);" \

        "vec3 reflection_vector_blue = reflect(-normalized_light_direction_blue,normalized_transformed_normal);" \
        "vec3 ambient_blue = u_la_blue * u_ka;" \
        "vec3 diffuse_blue = u_ld_blue * u_kd * max(dot(normalized_light_direction_blue,normalized_transformed_normal),0.0f);" \
        "vec3 specular_blue = u_ls_blue * u_ks * pow(max(dot(reflection_vector_blue,normalized_view_vector ),0.0f),u_shininess);" \

        "vec3 reflection_vector_green = reflect(-normalized_light_direction_green,normalized_transformed_normal);" \
        "vec3 ambient_green = u_la_green * u_ka;" \
        "vec3 diffuse_green = u_ld_green * u_kd * max(dot(normalized_light_direction_green,normalized_transformed_normal),0.0f);" \
        "vec3 specular_green = u_ls_green * u_ks * pow(max(dot(reflection_vector_green,normalized_view_vector ),0.0f),u_shininess);" \

        "phong_ads_light = ambient_red + ambient_blue + ambient_green + diffuse_red + diffuse_blue + diffuse_green + specular_red + specular_blue + specular_green;" \
        "}" \
        "else" \
        "{" \
        "phong_ads_light = vec3(1.0f,1.0f,1.0f);" \
        "}" \
        "FragColor = vec4(phong_ads_light,1.0f);" \
        "}";

    glShaderSource(gFragmentShaderObject_PF,1,(const char **)&fragmentShaderSourceCode_PF,NULL);
fprintf(gpFile,"Fragment_PF\n\n");
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
                DestroyWindow(ghwnd);
            }
        }
    }


//    /*--------------------------------------------SHADER PROGRAM-------------------------------------------------------------

    //Create
    gShaderProgramObject_PF = glCreateProgram();

    //Attach
    glAttachShader(gShaderProgramObject_PF,gVertexShaderObject_PF);
    glAttachShader(gShaderProgramObject_PF,gFragmentShaderObject_PF);

    glBindAttribLocation(gShaderProgramObject_PF,BDJ_ATTRIBUTE_POSITION,"vPosition");
    glBindAttribLocation(gShaderProgramObject_PF,BDJ_ATTRIBUTE_NORMAL,"vNormal");

    //Link
    glLinkProgram(gShaderProgramObject_PF);

    //-------------------------------------
    //GLint shaderProgramLinkStatus = 0;
    //-------------------------------------
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
                DestroyWindow(ghwnd);
            }
        }
    }

//    mvpUniform = glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");

    modelMatrixUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_model_matrix");
    viewMatrixUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_view_matrix");
    perspectiveProjectionUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_perspective_projection_matrix");
    LaUniform_Red_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_la_red");
    LdUniform_Red_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ld_red");
    LsUniform_Red_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ls_red");
    lightPositionUniform_Red_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_light_position_red");
    LaUniform_Blue_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_la_blue");
    LdUniform_Blue_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ld_blue");
    LsUniform_Blue_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ls_blue");
    lightPositionUniform_Blue_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_light_position_blue");
    LaUniform_Green_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_la_green");
    LdUniform_Green_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ld_green");
    LsUniform_Green_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ls_green");
    lightPositionUniform_Green_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_light_position_green");
    KaUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ka");
    KdUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_kd");
    KsUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_ks");
    KshineUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_shininess");
    LKeyPressedUniform_PF = glGetUniformLocation(gShaderProgramObject_PF,"u_LKeyPressed");

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

    glClearColor(0.0f,0.0f,0.0f,0.0f);

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

if(FragmentShader == false)
{
    glUseProgram(gShaderProgramObject_PV);

    if(bLight == true)
    {
        lightPosition_Red_PV[0] = 0.0f;
        lightPosition_Red_PV[1] = factor * sin(angleX);
        lightPosition_Red_PV[2] = factor * cos(angleX);
        lightPosition_Red_PV[3] = 1.0f;

        lightPosition_Green_PV[0] = factor * sin(angleY);
        lightPosition_Green_PV[1] = 0.0f;
        lightPosition_Green_PV[2] = factor * cos(angleY);
        lightPosition_Green_PV[3] = 1.0f;

        lightPosition_Blue_PV[0] = factor * sin(angleZ);
        lightPosition_Blue_PV[1] = factor * cos(angleZ);
        lightPosition_Blue_PV[2] = 0.0f;
        lightPosition_Blue_PV[3] = 1.0f;

        glUniform1i(LKeyPressedUniform_PV,1);
        glUniform1f(KshineUniform_PV,MaterialShininess_PV);
        glUniform4fv(lightPositionUniform_Red_PV,1,lightPosition_Red_PV);
        glUniform3fv(LaUniform_Red_PV,1,lightAmbient_Red_PV);
        glUniform3fv(LdUniform_Red_PV,1,lightDiffuse_Red_PV);
        glUniform3fv(LsUniform_Red_PV,1,lightSpecular_Red_PV);
        glUniform4fv(lightPositionUniform_Blue_PV,1,lightPosition_Blue_PV);
        glUniform3fv(LaUniform_Blue_PV,1,lightAmbient_Blue_PV);
        glUniform3fv(LdUniform_Blue_PV,1,lightDiffuse_Blue_PV);
        glUniform3fv(LsUniform_Blue_PV,1,lightSpecular_Blue_PV);
        glUniform4fv(lightPositionUniform_Green_PV,1,lightPosition_Green_PV);
        glUniform3fv(LaUniform_Green_PV,1,lightAmbient_Green_PV);
        glUniform3fv(LdUniform_Green_PV,1,lightDiffuse_Green_PV);
        glUniform3fv(LsUniform_Green_PV,1,lightSpecular_Green_PV);
        glUniform3fv(KaUniform_PV,1,MaterialAmbient_PV);
        glUniform3fv(KdUniform_PV,1,MaterialDiffuse_PV);
        glUniform3fv(KsUniform_PV,1,MaterialSpecular_PV);
    }
    else
    {
        glUniform1i(LKeyPressedUniform_PV,0);
    }

    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();     // view la Identity
    mat4 projectionMatrix = mat4::identity();
    mat4 translateMatrix = vmath::translate(0.0f,0.0f,-3.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform_PV,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform_PV,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform_PV,1,GL_FALSE,projectionMatrix);
}
else
{
    glUseProgram(gShaderProgramObject_PF);

    if(bLight == true)
    {
        lightPosition_Red_PF[0] = 0.0f;
        lightPosition_Red_PF[1] = factor * sin(angleX);
        lightPosition_Red_PF[2] = factor * cos(angleX);
        lightPosition_Red_PF[3] = 1.0f;

        lightPosition_Green_PF[0] = factor * sin(angleY);
        lightPosition_Green_PF[1] = 0.0f;
        lightPosition_Green_PF[2] = factor * cos(angleY);
        lightPosition_Green_PF[3] = 1.0f;

        lightPosition_Blue_PF[0] = factor * sin(angleZ);
        lightPosition_Blue_PF[1] = factor * cos(angleZ);
        lightPosition_Blue_PF[2] = 0.0f;
        lightPosition_Blue_PF[3] = 1.0f;

        glUniform1i(LKeyPressedUniform_PF,1);
        glUniform1f(KshineUniform_PF,MaterialShininess_PF);
        glUniform4fv(lightPositionUniform_Red_PF,1,lightPosition_Red_PF);
        glUniform3fv(LaUniform_Red_PF,1,lightAmbient_Red_PF);
        glUniform3fv(LdUniform_Red_PF,1,lightDiffuse_Red_PF);
        glUniform3fv(LsUniform_Red_PF,1,lightSpecular_Red_PF);
        glUniform4fv(lightPositionUniform_Blue_PF,1,lightPosition_Blue_PF);
        glUniform3fv(LaUniform_Blue_PF,1,lightAmbient_Blue_PF);
        glUniform3fv(LdUniform_Blue_PF,1,lightDiffuse_Blue_PF);
        glUniform3fv(LsUniform_Blue_PF,1,lightSpecular_Blue_PF);
        glUniform4fv(lightPositionUniform_Green_PF,1,lightPosition_Green_PF);
        glUniform3fv(LaUniform_Green_PF,1,lightAmbient_Green_PF);
        glUniform3fv(LdUniform_Green_PF,1,lightDiffuse_Green_PF);
        glUniform3fv(LsUniform_Green_PF,1,lightSpecular_Green_PF);
        glUniform3fv(KaUniform_PF,1,MaterialAmbient_PF);
        glUniform3fv(KdUniform_PF,1,MaterialDiffuse_PF);
        glUniform3fv(KsUniform_PF,1,MaterialSpecular_PF);
    }
    else
    {
        glUniform1i(LKeyPressedUniform_PF,0);
    }

    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();     // view la Identity
    mat4 projectionMatrix = mat4::identity();
    mat4 translateMatrix = vmath::translate(0.0f,0.0f,-3.0f);

    modelMatrix = translateMatrix  ;    // model la Translate
    projectionMatrix = perspectiveProjectionMatrix;  // perspective la Projection

    glUniformMatrix4fv(modelMatrixUniform_PF,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform_PF,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(perspectiveProjectionUniform_PF,1,GL_FALSE,projectionMatrix);
}
        // *** bind vao ***
    glBindVertexArray(gVao_sphere);

    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    // *** unbind vao ***
    glBindVertexArray(0);

    //Stop OpenGL Program
    glUseProgram(0);

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

    if(gShaderProgramObject_PV)
    {
        glUseProgram(gShaderProgramObject_PV);
        GLsizei shaderCount;

        glGetProgramiv(gShaderProgramObject_PV,GL_ATTACHED_SHADERS,&shaderCount);

        GLuint *pShaders = NULL;

        pShaders = (GLuint*)malloc(shaderCount * sizeof(GLuint));
        if(pShaders == NULL)
        {
            printf("Malloc Failed!!!Exitting Now!!\n\n");
            exit(0);
        }

        glGetAttachedShaders(gShaderProgramObject_PV,shaderCount,&shaderCount,pShaders);

        for(GLsizei i = 0 ; i < shaderCount ; i++)
        {
            glDetachShader(gShaderProgramObject_PV,pShaders[i]);
            glDeleteShader(pShaders[i]);
            pShaders[i] = 0;
            free(pShaders);

            glDeleteProgram(gShaderProgramObject_PV);
            gShaderProgramObject_PV = 0;
            glUseProgram(0);
        }
    }

    if(gShaderProgramObject_PF)
    {
        glUseProgram(gShaderProgramObject_PF);
        GLsizei shaderCount;

        glGetProgramiv(gShaderProgramObject_PF,GL_ATTACHED_SHADERS,&shaderCount);

        GLuint *pShaders = NULL;

        pShaders = (GLuint*)malloc(shaderCount * sizeof(GLuint));
        if(pShaders == NULL)
        {
            printf("Malloc Failed!!!Exitting Now!!\n\n");
            exit(0);
        }

        glGetAttachedShaders(gShaderProgramObject_PF,shaderCount,&shaderCount,pShaders);

        for(GLsizei i = 0 ; i < shaderCount ; i++)
        {
            glDetachShader(gShaderProgramObject_PF,pShaders[i]);
            glDeleteShader(pShaders[i]);
            pShaders[i] = 0;
            free(pShaders);

            glDeleteProgram(gShaderProgramObject_PF);
            gShaderProgramObject_PF = 0;
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
