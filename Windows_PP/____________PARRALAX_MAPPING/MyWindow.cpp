//HEADER FILES

#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"MyWindow.h"
#include<gl/glew.h>
#include<gl/gl.h>

#include"vmath.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#define       WIN_WIDTH      800
#define       WIN_HEIGHT     600

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")

using namespace vmath;

enum PARALLAX_MAPPING_ATTRIBS
{
    BDJ_PARALLAX_MAPPING_ATTRIBUTE_POSITION = 0,
    BDJ_PARALLAX_MAPPING_ATTRIBUTE_NORMAL,
    BDJ_PARALLAX_MAPPING_ATTRIBUTE_TEXTURE0,
    BDJ_PARALLAX_MAPPING_ATTRIBUTE_TANGENT,
    BDJ_PARALLAX_MAPPING_ATTRIBUTE_BITANGENT
};

FILE* gpFile = NULL;

bool gbFullScreen = false;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
HWND ghwnd = NULL;

bool gbActiveWindow = false;

HDC ghdc = NULL;
HGLRC ghrc = NULL;

//GLuint Smiley_Texture;

vmath::vec3 lightPos(0.5f, 1.0f, 0.3f);

//GLuint lightPos[] = {0.5f, 1.0f, 0.3f};

//Vertex Shader Uniform
GLuint projectionUniform;
GLuint viewUniform;
GLuint modelUniform;
GLuint lightPosUniform;
//GLuint viewPosUniform;
//Fragment Shader Uniforms
GLuint textureSamplerUniformDiffuse;
GLuint textureSamplerUniformNormal;
GLuint textureSamplerUniformDepth;
GLuint heightScaleUniform;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint vao_quad;
GLuint vbo_quad;
//GLuint vbo_Position_smiley;
//GLuint vbo_texture_smiley;
//GLuint vbo_Normal_smiley;

unsigned int diffuseMap;
unsigned int normalMap;
unsigned int heightMap;

float heightScale = 0.1;

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
                          TEXT("Parallax Mapping : Bhavesh Joshi !!"),
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

	glEnable(GL_TEXTURE_2D);   //Texture Code

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
    unsigned int loadTexture(const char *path);

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

    //fprintf(gpFile,"1\n");

    /*--------------------------------------------VERTEX SHADER-------------------------------------------------------------*/

    //Create Shader
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    //Feed Shader
    const GLchar *vertexShaderSourceCode =
            "#version 440 core" \
            "\n" \
            "in vec3 aPos;" \
            "in vec3 aNormal;" \
            "in vec2 aTexCoords;" \
            "in vec3 aTangent;" \
            "in vec3 aBitangent;" \
            "out vec3 FragPos;" \
            "out vec2 TexCoords;" \
            "out vec3 TangentLightPos;" \
            "out vec3 TangentViewPos;" \
            "out vec3 TangentFragPos;" \
            "uniform mat4 projection;" \
            "uniform mat4 view;" \
            "uniform mat4 model;" \
            "uniform vec3 lightPos;" \
            "uniform vec3 viewPos;" \
            "void main()" \
            "{" \
            "FragPos = vec3(model * vec4(aPos, 1.0));" \
            "TexCoords = aTexCoords;" \
            "vec3 T = normalize(mat3(model) * aTangent);" \
            "vec3 B = normalize(mat3(model) * aBitangent);" \
            "vec3 N = normalize(mat3(model) * aNormal);" \
            "mat3 TBN = transpose(mat3(T, B, N));" \
            "TangentLightPos = TBN * lightPos;" \
            "TangentViewPos  = TBN * viewPos;" \
            "TangentFragPos  = TBN * FragPos;" \
            "gl_Position = projection * view * model * vec4(aPos, 1.0);" \
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
        "out vec4 FragColor;" \
        "in vec3 FragPos;" \
        "in vec2 TexCoords;" \
        "in vec3 TangentLightPos;" \
        "in vec3 TangentViewPos;" \
        "in vec3 TangentFragPos;" \
        "uniform sampler2D diffuseMap;" \
        "uniform sampler2D normalMap;" \
        "uniform sampler2D depthMap;" \
        "uniform float height_scale;" \
        "vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)" \
        "{" \
        "float height =  texture(depthMap, texCoords).r;" \
        "return texCoords - viewDir.xy * (height * height_scale);" \
        "}" \
        "void main()" \
        "{" \
        "vec3 viewDir = normalize(TangentViewPos - TangentFragPos);" \
        "vec2 texCoords = TexCoords;" \
        "texCoords = ParallaxMapping(TexCoords,  viewDir);" \
        "if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)" \
        "discard;" \
        "vec3 normal = texture(normalMap, texCoords).rgb;" \
        "normal = normalize(normal * 2.0 - 1.0);" \
        "vec3 color = texture(diffuseMap, texCoords).rgb;" \
        "vec3 ambient = 0.1 * color;" \
        "vec3 lightDir = normalize(TangentLightPos - TangentFragPos);" \
        "float diff = max(dot(lightDir, normal), 0.0);" \
        "vec3 diffuse = diff * color;" \
        "vec3 reflectDir = reflect(-lightDir, normal);" \
        "vec3 halfwayDir = normalize(lightDir + viewDir);" \
        "float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);" \
        "vec3 specular = vec3(0.2) * spec;" \
        "FragColor = vec4(ambient + diffuse + specular, 1.0);" \
        "}";

        //"FragColor = vec4(ambient + diffuse + specular, 1.0);"
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

    glBindAttribLocation(gShaderProgramObject,BDJ_PARALLAX_MAPPING_ATTRIBUTE_POSITION,"aPos");
    glBindAttribLocation(gShaderProgramObject,BDJ_PARALLAX_MAPPING_ATTRIBUTE_TEXTURE0,"aTexCoords");
    glBindAttribLocation(gShaderProgramObject,BDJ_PARALLAX_MAPPING_ATTRIBUTE_NORMAL,"aNormal");
    glBindAttribLocation(gShaderProgramObject,BDJ_PARALLAX_MAPPING_ATTRIBUTE_TANGENT,"aTangent");
    glBindAttribLocation(gShaderProgramObject,BDJ_PARALLAX_MAPPING_ATTRIBUTE_BITANGENT,"aBitangent");

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

    //Vertex Shader Uniform
    projectionUniform = glGetUniformLocation(gShaderProgramObject,"projection");
    viewUniform = glGetUniformLocation(gShaderProgramObject,"view");
    modelUniform = glGetUniformLocation(gShaderProgramObject,"model");
    lightPosUniform = glGetUniformLocation(gShaderProgramObject,"lightPos");
    //viewPosUniform = glGetUniformLocation(gShaderProgramObject,"viewpos");
    //Fragment Shader Uniforms
    textureSamplerUniformDiffuse = glGetUniformLocation(gShaderProgramObject,"diffuseMap");
    textureSamplerUniformNormal = glGetUniformLocation(gShaderProgramObject,"normalMap");
    textureSamplerUniformDepth = glGetUniformLocation(gShaderProgramObject,"depthMap");
    heightScaleUniform = glGetUniformLocation(gShaderProgramObject,"height_scale");

    // positions
    vmath::vec3 pos1(-1.0f,  1.0f, 0.0f);
    vmath::vec3 pos2(-1.0f, -1.0f, 0.0f);
    vmath::vec3 pos3( 1.0f, -1.0f, 0.0f);
    vmath::vec3 pos4( 1.0f,  1.0f, 0.0f);
    // texture coordinates
    vmath::vec2 uv1(0.0f, 1.0f);
    vmath::vec2 uv2(0.0f, 0.0f);
    vmath::vec2 uv3(1.0f, 0.0f);
    vmath::vec2 uv4(1.0f, 1.0f);
    // normal vector
    vmath::vec3 nm(0.0f, 0.0f, 1.0f);
    // calculate tangent/bitangent vectors of both triangles
    vmath::vec3 tangent1, bitangent1;
    vmath::vec3 tangent2, bitangent2;
    // triangle 1
    vmath::vec3 edge1 = pos2 - pos1;
    vmath::vec3 edge2 = pos3 - pos1;
    vmath::vec2 deltaUV1 = uv2 - uv1;
    vmath::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

    tangent1[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
    tangent1[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
    tangent1[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
    tangent1 = vmath::normalize(tangent1);

    bitangent1[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
    bitangent1[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
    bitangent1[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);
    bitangent1 = vmath::normalize(bitangent1);

    // triangle 2
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);
    tangent2[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
    tangent2[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
    tangent2[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
    tangent2 = vmath::normalize(tangent2);


    bitangent2[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
    bitangent2[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
    bitangent2[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);
    bitangent2 = vmath::normalize(bitangent2);

    float quadVertices[] =
    {
    // positions            // normal         // texcoords  // tangent                          // bitangent
    pos1[0], pos1[1], pos1[2], nm[0], nm[1], nm[2], uv1[0], uv1[1], tangent1[0], tangent1[1], tangent1[2], bitangent1[0], bitangent1[1], bitangent1[2],
    pos2[0], pos2[1], pos2[2], nm[0], nm[1], nm[2], uv2[0], uv2[1], tangent1[0], tangent1[1], tangent1[2], bitangent1[0], bitangent1[1], bitangent1[2],
    pos3[0], pos3[1], pos3[2], nm[0], nm[1], nm[2], uv3[0], uv3[1], tangent1[0], tangent1[1], tangent1[2], bitangent1[0], bitangent1[1], bitangent1[2],

    pos1[0], pos1[1], pos1[2], nm[0], nm[1], nm[2], uv1[0], uv1[1], tangent2[0], tangent2[1], tangent2[2], bitangent2[0], bitangent2[1], bitangent2[2],
    pos3[0], pos3[1], pos3[2], nm[0], nm[1], nm[2], uv3[0], uv3[1], tangent2[0], tangent2[1], tangent2[2], bitangent2[0], bitangent2[1], bitangent2[2],
    pos4[0], pos4[1], pos4[2], nm[0], nm[1], nm[2], uv4[0], uv4[1], tangent2[0], tangent2[1], tangent2[2], bitangent2[0], bitangent2[1], bitangent2[2]
    };

    // configure plane VAO
    glGenVertexArrays(1, &vao_quad);
    glBindVertexArray(vao_quad);

    glGenBuffers(1, &vbo_quad);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(BDJ_PARALLAX_MAPPING_ATTRIBUTE_POSITION);
    glVertexAttribPointer(BDJ_PARALLAX_MAPPING_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(BDJ_PARALLAX_MAPPING_ATTRIBUTE_NORMAL);
    glVertexAttribPointer(BDJ_PARALLAX_MAPPING_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(BDJ_PARALLAX_MAPPING_ATTRIBUTE_TEXTURE0);
    glVertexAttribPointer(BDJ_PARALLAX_MAPPING_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(BDJ_PARALLAX_MAPPING_ATTRIBUTE_TANGENT);
    glVertexAttribPointer(BDJ_PARALLAX_MAPPING_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(BDJ_PARALLAX_MAPPING_ATTRIBUTE_BITANGENT);
    glVertexAttribPointer(BDJ_PARALLAX_MAPPING_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);
    //fprintf(gpFile,"2\n");

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	diffuseMap = loadTexture(".\\resources\\textures\\bricks2.jpg");
    normalMap  = loadTexture(".\\resources\\textures\\bricks2_normal.jpg");
    heightMap  = loadTexture(".\\resources\\textures\\bricks2_disp.jpg");
/*
    unsigned int diffuseMap = loadTexture("/resources/textures/bricks2.jpg");
    unsigned int normalMap  = loadTexture("/resources/textures/bricks2_normal.jpg");
    unsigned int heightMap  = loadTexture("/resources/textures/bricks2_disp.jpg");
*/
	glEnable(GL_TEXTURE_2D);   //Texture Code

    //fprintf(gpFile,"3\n");

    glClearColor(0.1f,0.1f,0.1f,1.0f);

    perspectiveProjectionMatrix = mat4::identity();

    Resize(WIN_WIDTH,WIN_HEIGHT);
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        fprintf(gpFile,"Texture Loaded Successfully!!!\n\n");

        stbi_image_free(data);
    }
    else
    {
        fprintf(gpFile,"Texture failed to load at path: %s \n\n",path);
        //std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
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
    GLfloat angle = 90.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Start Using OpenGL Program
    glUseProgram(gShaderProgramObject);

    //FOR TRIANGLE
    mat4 modelMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity();
    mat4 projectionMatrix = mat4::identity();
    mat4 translateMatrix = vmath::translate(0.0f,0.0f,-0.3f);
    mat4 rotateMatrix = vmath::rotate(angle,1.0f,1.0f,0.0f);
    mat4 scaleMatrix = vmath::scale(0.25f,0.25f,0.25f);

    modelMatrix = translateMatrix * scaleMatrix * rotateMatrix;
    projectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(modelUniform,1,GL_FALSE,modelMatrix);
    glUniformMatrix4fv(viewUniform,1,GL_FALSE,viewMatrix);
    glUniformMatrix4fv(projectionUniform,1,GL_FALSE,projectionMatrix);
    glUniform3fv(lightPosUniform,1,lightPos);
    glUniform1f(heightScaleUniform,heightScale);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,diffuseMap);
    glUniform1i(textureSamplerUniformDiffuse,0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,normalMap);
    glUniform1i(textureSamplerUniformNormal,1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,heightMap);
    glUniform1i(textureSamplerUniformDepth,2);

    glBindVertexArray(vao_quad);

    glDrawArrays(GL_TRIANGLES,0,6);

    glBindVertexArray(0);

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


    //glDeleteTextures(1,&Smiley_Texture);

    /*
    glDetachShader(gShaderProgramObject,gVertexShaderObject);
    glDetachShader(gShaderProgramObject,gFragmentShaderObject);

    glDeleteShader(gVertexShaderObject);
    gVertexShaderObject = 0;

    glDeleteShader(gFragmentShaderObject);
    gFragmentShaderObject = 0;

    glUseProgram(0);
    */

    if(vao_quad)
    {
        glDeleteVertexArrays(1,&vao_quad);
        vao_quad = 0;
    }
/*
    if(vbo_Position_smiley)
    {
        glDeleteBuffers(1,&vbo_Position_smiley);
        vbo_Position_smiley = 0;
    }

    if(vbo_texture_smiley)
    {
        glDeleteBuffers(1,&vbo_texture_smiley);
        vbo_texture_smiley = 0;
    }
*/
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
