package com.example.diffuse;

import android.content.Context;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

import android.opengl.GLES32;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,OnGestureListener,OnDoubleTapListener
{
	private final Context context;

	static float angle_cube = 0.0f;
		
	private GestureDetector gestureDetector;

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

	private int[] vao_cube = new int[1];
	private int[] vbo_position_cube = new int[1];
	private int[] vbo_normal_cube = new int[1]; 

	private int modelViewMatrixUniform;
	private int perspectiveProjectionUniform;
	private int LKeyPressedUniform;
	private int LdUniform;
	private int KdUniform;
	private int lightPositionUniform;
	
	private boolean light;
	private int tap;

	private float perspectiveProjectionMatrix[] = new float[16];
	
	public GLESView(Context drawingContext)
	{
		super(drawingContext);
		
		context = drawingContext;

		setEGLContextClientVersion(3);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		gestureDetector = new GestureDetector(context,this,null,false);
		gestureDetector.setOnDoubleTapListener(this);
	} 

	@Override
	public void onSurfaceCreated(GL10 gl,EGLConfig config)
	{
		String version = gl.glGetString(GL10.GL_VERSION);
		System.out.println("BDJ: OpenGL-ES Version:  " +version);
		String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
		System.out.println("BDJ: OpenGL-ES GLSL Version:  "+glslVersion);
		initialize(gl);
	}

	@Override
	public void onSurfaceChanged(GL10 unused,int width,int height)
	{
		resize(width,height);
	}

	@Override
	public void onDrawFrame(GL10 unused)
	{
		display();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent e)
	{	
		int eventaction = e.getAction();
		if(!gestureDetector.onTouchEvent(e))
			super.onTouchEvent(e);
		return(true);
	}

	@Override
	public boolean onDoubleTap(MotionEvent e)
	{	
		tap = 1;
		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{	
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e)
	{	
		//tap = 0;
	
		if(light == false)
			light = true;	
		else
			light = false;

		return(true);
	}

	@Override
	public boolean onDown(MotionEvent e)
	{	
		return(true);
	}

	@Override
	public boolean onFling(MotionEvent e1,MotionEvent e2,float velocityX, float velocityY)
	{	
		return(true);
	}

	@Override
	public void onLongPress(MotionEvent e)
	{	
		tap = 0;
		
	}

	@Override
	public boolean onScroll(MotionEvent e1,MotionEvent e2,float distanceX, float distanceY)
	{	
		uninitialize();
		System.exit(0);
		return(true);
	}

	@Override
	public void onShowPress(MotionEvent e)
	{	
	}

	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{	
		return(true);
	}

	private void initialize(GL10 gl)	
	{	
	
		vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
		
		final String vertexShaderSourceCode = 
	      	String.format(
		"#version 320 es"+
		"\n"+
		"in vec4 vPosition;"+
		"in vec3 vNormal;"+
		"uniform mat4 u_model_view_matrix;"+
		"uniform mat4 u_projection_matrix;"+
		"uniform int u_LKeyPressed;"+
		"uniform vec3 u_ld;"+
		"uniform vec3 u_kd;"+
		"uniform vec4 u_light_position;"+
		"out vec3 diffuse_light;"+
		"void main(void)"+
		"{"+
		"if(u_LKeyPressed == 1)"+
		"{"+
		"vec4 eye_coordinates = u_model_view_matrix * vPosition;"+
		"mat3 normal_matrix = mat3(transpose(inverse(u_model_view_matrix)));"+
		"vec3 tnorm = normalize(normal_matrix * vNormal);"+
		"vec3 s = normalize(vec3(u_light_position - eye_coordinates));"+
		"diffuse_light = u_ld * u_kd * max(dot(s,tnorm),0.0f);"+
		"}"+
		"gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;"+
		"}"
	       );

		GLES32.glShaderSource(vertexShaderObject,vertexShaderSourceCode);

		GLES32.glCompileShader(vertexShaderObject);
	
		int[] iShaderCompiledStatus = new int[1];
		int[] iInfoLogLength = new int[1];
		String szInfoLog = null;
		GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_COMPILE_STATUS,iShaderCompiledStatus,0);

		if(iShaderCompiledStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength,0);
			
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
				System.out.println("BDJ: Vertex Shader Compilation Log = "+szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
		
		final String fragmentShaderSourceCode = 
		String.format(
		  "#version 320 es"+
       		  "\n"+
		  "precision highp float;"+
        	  "in vec3 diffuse_light;"+
      		  "uniform highp int u_LKeyPressed;"+
     	          "out vec4 FragColor;"+
     		  "void main(void)"+
       		  "{"+
       		  "vec4 color;"+
       		  "if(u_LKeyPressed == 1)"+
      		  "{"+
      		  "color = vec4(diffuse_light,1.0f);"+
      		  "}"+
      		  "else"+
      		  "{"+
      		  "color = vec4(1.0f,1.0f,1.0f,1.0f);"+
      		  "}"+
      		  "FragColor = color;"+
     	          "}"
		);

		GLES32.glShaderSource(fragmentShaderObject,fragmentShaderSourceCode);

		GLES32.glCompileShader(fragmentShaderObject);

		iShaderCompiledStatus[0] = 0;
		iInfoLogLength[0] = 0;
		szInfoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_COMPILE_STATUS,iShaderCompiledStatus,0);

		if(iShaderCompiledStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength,0);
			
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
				System.out.println("BDJ: Fragment Shader Compilation Log = "+szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}
		
		shaderProgramObject = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObject,vertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject,fragmentShaderObject);
	
		GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.BDJ_ATTRIBUTE_POSITION,"vPosition");
		GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.BDJ_ATTRIBUTE_NORMAL,"vNormal");

		GLES32.glLinkProgram(shaderProgramObject);
	
		int[] iShaderProgramLinkStatus = new int[1];
		iInfoLogLength[0] = 0;
		szInfoLog = null;
		GLES32.glGetProgramiv(shaderProgramObject,GLES32.GL_LINK_STATUS,iShaderProgramLinkStatus,0);

		if(iShaderProgramLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(shaderProgramObject,GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength,0);
			
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
				System.out.println("BDJ: Shader Program Link Log = "+szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		modelViewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_model_view_matrix");
		perspectiveProjectionUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_projection_matrix");
		LKeyPressedUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_LKeyPressed");
		LdUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_ld");
		KdUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_kd");
		lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_light_position");		

         final float cubeVertices[] = new float[]
         {
            
		        //front
		         1.0f,1.0f,1.0f,
		        -1.0f,1.0f,1.0f,
		        -1.0f,-1.0f,1.0f,
		        1.0f,-1.0f,1.0f,

		        //right
		        1.0f,1.0f,-1.0f,
		        1.0f,1.0f,1.0f,
		        1.0f,-1.0f,1.0f,
		        1.0f,-1.0f,-1.0f,

		        //back
		        -1.0f,1.0f,-1.0f,
		        1.0f,1.0f,-1.0f,
		        1.0f,-1.0f,-1.0f,
		        -1.0f,-1.0f,-1.0f,

		        //left
		        -1.0f, 1.0f, 1.0f,
		        -1.0f, 1.0f, -1.0f,
		        -1.0f, -1.0f, -1.0f,
		        -1.0f, -1.0f, 1.0f,

		        //top
		        1.0f,1.0f,-1.0f,
		        -1.0f,1.0f,-1.0f,
		        -1.0f,1.0f,1.0f,
		        1.0f,1.0f,1.0f,

		        //bottom
		        1.0f,-1.0f,-1.0f,
		        -1.0f,-1.0f,-1.0f,
		        -1.0f,-1.0f,1.0f,
		        1.0f,-1.0f,1.0f
	
         };

	final float cubeNormal[] = new float[]
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

//---------------------------------------------------------------------------------------------------------

		GLES32.glGenVertexArrays(1,vao_cube,0);
		GLES32.glBindVertexArray(vao_cube[0]);

		GLES32.glGenBuffers(1,vbo_position_cube,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_position_cube[0]);

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubeVertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticescube = byteBuffer.asFloatBuffer();
		verticescube.put(cubeVertices);
		verticescube.position(0);
	
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    cubeVertices.length*4,	
				    verticescube,
				    GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_POSITION,
					     3,	
					     GLES32.GL_FLOAT,	
					     false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

//---------------------------------------------------------------------------------------------------------

		GLES32.glGenBuffers(1,vbo_normal_cube,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_normal_cube[0]);

		byteBuffer = ByteBuffer.allocateDirect(cubeNormal.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		verticescube = byteBuffer.asFloatBuffer();
		verticescube.put(cubeNormal);
		verticescube.position(0);
	
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    cubeNormal.length*4,	
				    verticescube,
				    GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_NORMAL,
					     3,	
					     GLES32.GL_FLOAT,	
					     false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_NORMAL);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
		GLES32.glBindVertexArray(0);
//---------------------------------------------------------------------------------------------------------
		
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		//GLES32.glClearDepth(1.0f);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);
		//GLES32.glEnable(GLES32.GL_CULL_FACE);

		GLES32.glClearColor(0.0f,0.0f,0.0f,0.0f);

		Matrix.setIdentityM(perspectiveProjectionMatrix,0);
	}

	private void resize(int width,int height)
	{
		
		if(height == 0)
		{
			height = 1;
		}	
		
		GLES32.glViewport(0,0,width,height);
		
		Matrix.perspectiveM(perspectiveProjectionMatrix , 0 , 45.0f , ((float)width / (float)height) , 0.1f , 100.0f);
	}

	private void display()
	{	
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
		
		GLES32.glUseProgram(shaderProgramObject);

		float modelViewMatrix[] = new float[16];
		float translateMatrix[] = new float[16];
		float rotateMatrix_X[] = new float[16];
		float rotateMatrix_Y[] = new float[16];
		float rotateMatrix_Z[] = new float[16];
		float lightPosition[] = new float[]{0.0f,0.0f,2.0f,1.0f};

		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(translateMatrix,0);
		Matrix.setIdentityM(rotateMatrix_X,0);
		Matrix.setIdentityM(rotateMatrix_Y,0);
		Matrix.setIdentityM(rotateMatrix_Z,0);

        	Matrix.translateM(translateMatrix,0,0.0f,0.0f,-6.0f);
        	Matrix.rotateM(rotateMatrix_X,0,angle_cube,1.0f,0.0f,0.0f);
		Matrix.rotateM(rotateMatrix_Y,0,angle_cube,0.0f,1.0f,0.0f);
		Matrix.rotateM(rotateMatrix_Z,0,angle_cube,0.0f,0.0f,1.0f);
	
		Matrix.multiplyMM(modelViewMatrix,0,modelViewMatrix,0,translateMatrix,0);
		Matrix.multiplyMM(modelViewMatrix,0,modelViewMatrix,0,rotateMatrix_X,0);
		Matrix.multiplyMM(modelViewMatrix,0,modelViewMatrix,0,rotateMatrix_Y,0);
		Matrix.multiplyMM(modelViewMatrix,0,modelViewMatrix,0,rotateMatrix_Z,0);
		
		if(light == true)
    		{
			
        		GLES32.glUniform1i(LKeyPressedUniform,1);
        		GLES32.glUniform3f(LdUniform,1.0f,1.0f,1.0f);
        		GLES32.glUniform3f(KdUniform,0.5f,0.5f,0.5f);
        		GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);
  		}
    		else
    		{
        		GLES32.glUniform1i(LKeyPressedUniform,0);
    		}	

		GLES32.glUniformMatrix4fv(modelViewMatrixUniform,1,false,modelViewMatrix,0);
		GLES32.glUniformMatrix4fv(perspectiveProjectionUniform,1,false,perspectiveProjectionMatrix,0);
	
		GLES32.glBindVertexArray(vao_cube[0]);
                GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,0,4);
                GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,4,4);       
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);	
		GLES32.glBindVertexArray(0);
//---------------------------------------------------------------------------------------------
		GLES32.glUseProgram(0);	
			
		if(tap == 1)
		{
			angle_cube = angle_cube + 1.0f;
		}
		
		if(angle_cube >= 360.0f)
		{
			angle_cube = 0.0f;
		}
		
		requestRender();
	}

	void uninitialize()
	{
		if(vao_cube[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1,vao_cube,0);
			vao_cube[0] = 0;
		}

		if(vbo_position_cube[0] != 0)
		{
			GLES32.glDeleteBuffers(1,vbo_position_cube,0);
			vbo_position_cube[0] = 0;
		}

		if(vbo_normal_cube[0] != 0)
		{
			GLES32.glDeleteBuffers(1,vbo_normal_cube,0);
			vbo_normal_cube[0] = 0;
		}

		if(shaderProgramObject != 0)
		{
			if(vertexShaderObject != 0)
			{
				GLES32.glDetachShader(shaderProgramObject,vertexShaderObject);
				GLES32.glDeleteShader(vertexShaderObject);
				vertexShaderObject = 0;
			}

			if(fragmentShaderObject != 0)
			{
				GLES32.glDetachShader(shaderProgramObject,fragmentShaderObject);
				GLES32.glDeleteShader(fragmentShaderObject);
				fragmentShaderObject = 0;
			}
		}

		if(shaderProgramObject != 0)
		{
			GLES32.glDeleteProgram(shaderProgramObject);
			shaderProgramObject = 0;
		}		
	}
} 