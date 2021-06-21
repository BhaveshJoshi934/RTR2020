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

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject_1;
GLuint gFragmentShaderObject_2;
GLuint gShaderProgramObject_1;
GLuint gShaderProgramObject_2;

int Width;
int Height;

GLuint vao;
GLuint position_buffer;
GLuint index_buffer;
GLuint vbo;
GLuint fbo;
GLuint color_texture;
GLuint depth_texture;
GLuint mvUniform1;
GLuint projUniform1;
GLuint mvUniform2;
GLuint projUniform2;
static GLfloat Cube_angle = 0.0f;

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

    if(fopen_s(&gpFile,"LogFile.txt","w") != 0)
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
                          TEXT("OpenGL SuperBible - Basic Framebuffer Object : Bhavesh Joshi !!"),
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
    static const char *vertexShaderSourceCode[]  =
    {
            "#version 440 core" \
            "\n" \
            "in vec4 vPosition;" \
            "in vec2 vTexcoord;" \
            "out vec4 out_color;" \
            "out vec2 out_texcoord;" \
            "uniform mat4 mv_matrix;" \
            "uniform mat4 proj_matrix;" \
            "void main(void)" \
            "{" \
            "gl_Position = proj_matrix * mv_matrix * vPosition;" \
            "out_color = vPosition * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);" \
            "out_texcoord = vTexcoord;" \
            "}"
    };

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

    //--------------------------------------------FRAGMENT SHADER-------------------------------------------------------------/

    //Create Shader
    gFragmentShaderObject_2 = glCreateShader(GL_FRAGMENT_SHADER);

    //Feed Shader
    static const char *fragmentShaderSourceCode_2[] =
    {
            "#version 440 core" \
            "\n" \
            "uniform sampler2D tex;" \
            "out vec4 FragColor;" \
            "in vec4 out_color;" \
            "in vec2 out_texcoord;" \
            "void main(void)" \
            "{" \
            "FragColor = mix(out_color, texture(tex,out_texcoord), 0.7);" \
            "}"
    };

    glShaderSource(gFragmentShaderObject_2,1,(const char **)&fragmentShaderSourceCode_2,NULL);

    //Compile
    glCompileShader(gFragmentShaderObject_2);

    glGetShaderiv(gFragmentShaderObject_2,GL_COMPILE_STATUS,&shaderCompiledStatus);
    if(shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(gFragmentShaderObject_2,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_2,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Fragment Shader_2 Compilation Log = %s\n\n",szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    gFragmentShaderObject_1 = glCreateShader(GL_FRAGMENT_SHADER);

    //Feed Shader
    const GLchar *fragmentShaderSourceCode_1[] =
    {
            "#version 440 core" \
            "\n" \
            "in vec4 out_color;"  \
            "in vec2 out_texcoord;" \
            "out vec4 FragColor;" \
            "void main(void)" \
            "{" \
            "FragColor = sin(out_color * vec4(40.0, 20.0, 30.0, 1.0)) * 0.5 + vec4(0.5);" \
            "}"
    };

    glShaderSource(gFragmentShaderObject_1,1,(const char **)&fragmentShaderSourceCode_1,NULL);

    //Compile
    glCompileShader(gFragmentShaderObject_1);

    glGetShaderiv(gFragmentShaderObject_1,GL_COMPILE_STATUS,&shaderCompiledStatus);
    if(shaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(gFragmentShaderObject_1,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_1,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Fragment Shader_1 Compilation Log = %s\n\n",szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    //--------------------------------------------SHADER PROGRAM-------------------------------------------------------------/

    //Create
    gShaderProgramObject_1 = glCreateProgram();

    //Attach
    glAttachShader(gShaderProgramObject_1,gVertexShaderObject);
    glAttachShader(gShaderProgramObject_1,gFragmentShaderObject_1);

    glBindAttribLocation(gShaderProgramObject_1,BDJ_ATTRIBUTE_POSITION,"vPosition");
    glBindAttribLocation(gShaderProgramObject_1,BDJ_ATTRIBUTE_TEXTURE0,"vTexcoord");

    //Link
    glLinkProgram(gShaderProgramObject_1);

    GLint shaderProgramLinkStatus = 0;
    glGetProgramiv(gShaderProgramObject_1,GL_LINK_STATUS,&shaderProgramLinkStatus);
    if(shaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gShaderProgramObject_1,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(gFragmentShaderObject_1,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Shader Program_1 Link Log = %s\n\n",szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    mvUniform1 = glGetUniformLocation(gShaderProgramObject_1,"mv_matrix");
    projUniform1 = glGetUniformLocation(gShaderProgramObject_1,"proj_matrix");

   //--------------------------------------------SHADER PROGRAM-------------------------------------------------------------/

    //Create
    gShaderProgramObject_2 = glCreateProgram();

    //Attach
    glAttachShader(gShaderProgramObject_2,gVertexShaderObject);
    glAttachShader(gShaderProgramObject_2,gFragmentShaderObject_2);

    glBindAttribLocation(gShaderProgramObject_2,BDJ_ATTRIBUTE_POSITION,"vPosition");
    glBindAttribLocation(gShaderProgramObject_2,BDJ_ATTRIBUTE_TEXTURE0,"vTexcoord");

    //Link
    glLinkProgram(gShaderProgramObject_2);

    //GLint shaderProgramLinkStatus = 0;
    glGetProgramiv(gShaderProgramObject_2,GL_LINK_STATUS,&shaderProgramLinkStatus);
    if(shaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gShaderProgramObject_2,GL_INFO_LOG_LENGTH,&infoLogLength);
        if(infoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * infoLogLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(gFragmentShaderObject_2,infoLogLength,&written,szInfoLog);
                fprintf(gpFile,"Shader Program_2 Link Log = %s\n\n",szInfoLog);
                free(szInfoLog);
                DestroyWindow(ghwnd);
            }
        }
    }

    mvUniform2 = glGetUniformLocation(gShaderProgramObject_2,"mv_matrix");
    projUniform2 = glGetUniformLocation(gShaderProgramObject_2,"proj_matrix");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static const GLushort vertex_indices[] =
    {
            0, 1, 2,
            2, 1, 3,
            2, 3, 4,
            4, 3, 5,
            4, 5, 6,
            6, 5, 7,
            6, 7, 0,
            0, 7, 1,
            6, 0, 2,
            2, 4, 6,
            7, 5, 3,
            7, 3, 1
    };

    static const GLfloat vertex_data[] =
    {
              // Position                 Tex Coord
            -0.5f, -0.5f,  0.5f,      0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,      1.0f, 0.0f,

             0.5f, -0.5f, -0.5f,      1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,      1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,      0.0f, 1.0f,

             0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,      1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,      0.0f, 1.0f,

             0.5f,  0.5f, -0.5f,      1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,      1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,      0.0f, 1.0f,

             0.5f,  0.5f, -0.5f,      1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,      0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,      1.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,      0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,      0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,      1.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,      1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,      1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,      0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,      1.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,      0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,      1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,      1.0f, 0.0f,

             0.5f, -0.5f, -0.5f,      1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,      0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,      0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,      1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,      1.0f, 1.0f,

             0.5f,  0.5f,  0.5f,      1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,      0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,      0.0f, 0.0f,

    };

    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data),vertex_data,GL_STATIC_DRAW);
    glVertexAttribPointer(BDJ_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_POSITION);
    glVertexAttribPointer(BDJ_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(BDJ_ATTRIBUTE_TEXTURE0);

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vertex_indices),vertex_indices,GL_STATIC_DRAW);

    glBindVertexArray(0);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexStorage2D(GL_TEXTURE_2D, 9, GL_RGBA8, 512, 512);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexStorage2D(GL_TEXTURE_2D, 9, GL_DEPTH_COMPONENT32F, 512, 512);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

    glEnable(GL_TEXTURE_2D);

    static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glClearColor(0.5f,0.5f,0.5f,0.5f);

    perspectiveProjectionMatrix = mat4::identity();

    Resize(WIN_WIDTH,WIN_HEIGHT);
}

void Resize(int width,int height)
{
    if(height == 0)
        height = 1;

        Width = width;
        Height = height;

    glViewport(0,0,(GLsizei)width,(GLsizei)height);

   // perspectiveProjectionMatrix = vmath::perspective(50.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
    perspectiveProjectionMatrix = vmath::perspective(45.0f,(float)width / (float)height,0.1f,100.0f);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static const GLfloat green[] = { 0.0f, 0.3f, 0.0f, 1.0f };
    static const GLfloat blue[] = { 0.0f, 0.0f, 0.3f, 1.0f };
    static const GLfloat one = 1.0f;

    vmath::mat4 proj_matrix = perspectiveProjectionMatrix;

    vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -4.0f) * vmath::translate(sinf(2.1f) * 0.5f,
                                                    cosf(1.7f ) * 0.5f,
                                                    sinf(1.3f) * cosf(1.5f ) * 2.0f) *
                                vmath::rotate((float)Cube_angle, 0.0f, 0.0f, 1.0f) *
                                vmath::rotate((float)Cube_angle, 0.0f, 1.0f, 0.0f) *
                                vmath::rotate((float)Cube_angle , 1.0f, 0.0f, 0.0f);



       glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glViewport(0, 0, 512,512);
       glClearBufferfv(GL_COLOR, 0, green);
       glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);

        glUseProgram(gShaderProgramObject_1);

        glUniformMatrix4fv(mvUniform1, 1, GL_FALSE, mv_matrix);
        glUniformMatrix4fv(projUniform1, 1, GL_FALSE, proj_matrix);

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

         glUseProgram(0);

        glViewport(0, 0,Width, Height);
       glClearBufferfv(GL_COLOR, 0, blue);
       glClearBufferfv(GL_DEPTH, 0, &one);

        glBindTexture(GL_TEXTURE_2D, color_texture);

        glUseProgram(gShaderProgramObject_2);

        glUniformMatrix4fv(mvUniform2, 1, GL_FALSE, mv_matrix);
        glUniformMatrix4fv(projUniform2, 1, GL_FALSE, proj_matrix);

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

         glUseProgram(0);

         Cube_angle = Cube_angle + 0.1f;
         if(Cube_angle >= 360.0f)
         {
             Cube_angle = 0.0f;
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

    if(vao)
    {
        glDeleteVertexArrays(1,&vao);
        vao = 0;
    }

    if(fbo)
    {
        glDeleteVertexArrays(1,&fbo);
        fbo = 0;
    }

    if(position_buffer)
    {
        glDeleteVertexArrays(1,&position_buffer);
        position_buffer = 0;
    }

    if(index_buffer)
    {
        glDeleteVertexArrays(1,&index_buffer);
        index_buffer = 0;
    }

    if(gShaderProgramObject_1)
    {
        glUseProgram(gShaderProgramObject_1);
        GLsizei shaderCount;

        glGetProgramiv(gShaderProgramObject_1,GL_ATTACHED_SHADERS,&shaderCount);

        GLuint *pShaders = NULL;

        pShaders = (GLuint*)malloc(shaderCount * sizeof(GLuint));
        if(pShaders == NULL)
        {
            printf("Malloc Failed!!!Exitting Now!!\n\n");
            exit(0);
        }

        glGetAttachedShaders(gShaderProgramObject_1,shaderCount,&shaderCount,pShaders);

        for(GLsizei i = 0 ; i < shaderCount ; i++)
        {
            glDetachShader(gShaderProgramObject_1,pShaders[i]);
            glDeleteShader(pShaders[i]);
            pShaders[i] = 0;
            free(pShaders);

            glDeleteProgram(gShaderProgramObject_1);
            gShaderProgramObject_1 = 0;
            glUseProgram(0);
        }
    }

 if(gShaderProgramObject_2)
    {
        glUseProgram(gShaderProgramObject_2);
        GLsizei shaderCount;

        glGetProgramiv(gShaderProgramObject_2,GL_ATTACHED_SHADERS,&shaderCount);

        GLuint *pShaders = NULL;

        pShaders = (GLuint*)malloc(shaderCount * sizeof(GLuint));
        if(pShaders == NULL)
        {
            printf("Malloc Failed!!!Exitting Now!!\n\n");
            exit(0);
        }

        glGetAttachedShaders(gShaderProgramObject_2,shaderCount,&shaderCount,pShaders);

        for(GLsizei i = 0 ; i < shaderCount ; i++)
        {
            glDetachShader(gShaderProgramObject_2,pShaders[i]);
            glDeleteShader(pShaders[i]);
            pShaders[i] = 0;
            free(pShaders);

            glDeleteProgram(gShaderProgramObject_2);
            gShaderProgramObject_2 = 0;
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
