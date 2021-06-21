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

bool bLight = false;
bool gbFullScreen = false;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
HWND ghwnd = NULL;

bool gbActiveWindow = false;

GLuint Marble_Texture;
GLuint textureSamplerUniform;

HDC ghdc = NULL;
HGLRC ghrc = NULL;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint vao_cube;
GLuint vbo_PCNT_cube;

GLuint modelViewMatrixUniform;
GLuint perspectiveProjectionUniform;
GLuint LKeyPressedUniform;
GLuint LdUniform;
GLuint KdUniform;
GLuint lightPositionUniform;

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
                          TEXT("Black and White Cube in PP : Bhavesh Joshi !!"),
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
            case 'L':
            case 'l':
                if(bLight == false)
                    bLight = true;
                else
                    bLight = false;
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
    bool LoadGLTexture(GLuint*,TCHAR[]);

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
    glBindAttribLocation(gShaderProgramObject,BDJ_ATTRIBUTE_TEXTURE0,"vTexCoord");

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

    //mvpUniform = glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");
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

    glGenVertexArrays(1,&vao_cube);
    glBindVertexArray(vao_cube);

    glGenBuffers(1,&vbo_PCNT_cube);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_PCNT_cube);
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

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    LoadGLTexture(&Marble_Texture,MAKEINTRESOURCE(MARBLE_BITMAP));     //Texture Code
	glEnable(GL_TEXTURE_2D);   //Texture Code

    glClearColor(0.0f,0.0f,0.0f,0.0f);

    perspectiveProjectionMatrix = mat4::identity();

    Resize(WIN_WIDTH,WIN_HEIGHT);
}

bool LoadGLTexture(GLuint *Texture,TCHAR resourceID[])
{
    //Variable Declartions
    bool bResult = false;
    HBITMAP hBitmap = NULL;    // OS Image Lading
    BITMAP bmp;                // OS Image Lading

    //code  :  Real Texture Code : Very Important

    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL),
                                 resourceID,
                                 IMAGE_BITMAP,
                                 0,
                                 0,
                                 LR_CREATEDIBSECTION); // KARAN he function handel return karat, : GetModuleHandel Hinstance deto...

    if(hBitmap)
    {
        bResult = true;
        GetObject(hBitmap,sizeof(BITMAP),&bmp);   // Ithe Image loading code sampla

        //From Here Start OpenGl code

        glPixelStorei(GL_UNPACK_ALIGNMENT,1);

        glGenTextures(1,Texture);   // GPU side la ek target pointer tayar zala; ani aplyala gattu milala : Ithe Address aahe
        glBindTexture(GL_TEXTURE_2D,*Texture); // Ithe Value aahe

        //Setting of Texture Param
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

        //Atta Data Tkaycha ahe Graphics card side la with the help of Graphic driver
        //gluBuild2DMipmaps(GL_TEXTURE_2D,3,bmp.bmWidth,bmp.bmHeight,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmp.bmBits);

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmp.bmWidth,bmp.bmHeight,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmp.bmBits);
        glGenerateMipmap(GL_TEXTURE_2D);

        DeleteObject(hBitmap);  // This is OS Fuction  : AApan texture use kela pn nahiye tari delete kela karan to load zalay...
    }

    return bResult;
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
    static GLfloat angle_pyramid = 0.0f;
    static GLfloat angle_cube = 0.0f;

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
    rotationMatrix1 = vmath::rotate((GLfloat)angle_cube,1.0f,0.0f,0.0f);
    rotationMatrix2 = vmath::rotate((GLfloat)angle_cube,0.0f,1.0f,0.0f);
    rotationMatrix3 = vmath::rotate((GLfloat)angle_cube,0.0f,0.0f,1.0f);

    modelViewMatrix = translateMatrix * rotationMatrix1 * rotationMatrix2 * rotationMatrix3 ;

    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(modelViewMatrixUniform,1,GL_FALSE,modelViewMatrix);

    glUniformMatrix4fv(perspectiveProjectionUniform,1,GL_FALSE,perspectiveProjectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,Marble_Texture);
    glUniform1i(textureSamplerUniform,0);

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

    if(vbo_PCNT_cube)
    {
        glDeleteBuffers(1,&vbo_PCNT_cube);
        vbo_PCNT_cube = 0;
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
