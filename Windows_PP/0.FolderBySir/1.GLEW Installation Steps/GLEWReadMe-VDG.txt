ASSUMING YOU ALREADY HAVE : Windows 10, 64-Bit
======================================================================

- Get Binaries ( .Dll And .Lib ) And Headers ( .h ) From http://glew.sourceforge.net/index.html
  As A Zip File Named : glew-2.1.0.zip

- Extract The Zip File To Its Default Directory : glew-2.1.0.zip

- Copy This Whole Directory As It Is To C:\

- So That Now You Have => C:\glew-2.1.0

- In A Win32 Project > In Visual Studio > Solution Explorer > Right Click On Project Name > 
  Properties > Left Pane > Configuration Properties :

  From Above Step, There Are 2 Methods. Do One At A Time Only, Then Try To Build The Project.
  Keep That Works.

  * Method 1
  ==========
  a) VC++ Directories > Right Pane > Include Directories > 
     Go To End Of Line, Type Semicolon If Already Not There, Type => C:\glew-2.1.0\include

  b) VC++ Directories > Right Pane > Library Directories > 
      Go To End Of Line, Type Semicolon If Already Not There, And Then
      i) If Project Is 32 bit, Type => C:\glew-2.1.0\lib\Release\Win32
     ii) If Project Is 64 bit, Type => C:\glew-2.1.0\lib\Release\x64

  * Method 2
  ==========
  a) C/C++ > General > Right Pane > Additional Include Directories > Type => C:\glew-2.1.0\include

  b) Linker > General > Right Pane > Additional Library Directories >
      i) If Project Is 32 bit, Type => C:\glew-2.1.0\lib\Release\Win32
     ii) If Project Is 64 bit, Type => C:\glew-2.1.0\lib\Release\x64

- For 32 Bit PROJECT:
  ===================
  From C:\glew-2.1.0\bin\Release\Win32 Directory ( Not From x64 Directory Though Our OS Is 64 Bit ), 
  Copy The Only 1 .dll File glew32.dll 
  To Our C:\Windows\SysWOW64 Directory ( Not To System32 Directory ).

  For 64 Bit PROJECT:
  ===================
  From C:\glew-2.1.0\bin\Release\x64 Directory ( Not From Win32 Directory ), 
  Copy The Only 1 .dll File glew32.dll 
  To Our C:\Windows\System32 Directory ( Not To SysWOW64 Directory ).

- In Any Program's Source Code #include <glew.h>
  IMPORTANT : This MUST Be Done Before #include For gl\gl.h

- Link Any Program To 'glew32.lib'
  // opengl import libraries
  #pragma comment(lib, "glew32.lib")

- In Source Code Add GLEW Initialization Code.
  IMPORTANT : It Must Be Added After Creating OpenGL Context ( i.e. After wglMakeCurrent() )
              But Before Using Any OpenGL Function ( Say glClearColor() )
