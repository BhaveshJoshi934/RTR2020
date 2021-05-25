package com.example.checkers;

import android.content.Context;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

import android.opengl.GLES32;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import android.graphics.Bitmap;
import android.opengl.GLUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,OnGestureListener,OnDoubleTapListener
{
	private final Context context;
		
	private GestureDetector gestureDetector;

	private int CheckWidth =  64;
	private int CheckHeight =  64;	
	private byte[] CheckImage = new byte [CheckWidth * CheckHeight * 4];

	private int[] TextureID = new int[1];

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

	private float[] Vertices_straight = new float[12];
	private float[] Vertices_angular = new float[12];

	private int[] vao_checker = new int[1];
	private int[] vbo_position_checker = new int[1];
	private int[] vbo_texture_checker = new int[1];

	private int textureSamplerUniform;

	private int mvpUniform;

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
		"in vec2 vTexCoord;"+
		"uniform mat4 u_mvp_matrix;"+
		"out vec2 out_TexCoord;"+
		"void main(void)"+
		"{"+
		"gl_Position = u_mvp_matrix * vPosition;"+
		"out_TexCoord = vTexCoord;"+
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
		"in vec2 out_TexCoord;"+
		"uniform highp sampler2D u_texture_sampler;"+
		"out vec4 FragColor;"+
		"void main(void)"+
		"{"+
		"FragColor = texture(u_texture_sampler,out_TexCoord);"+
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
		GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.BDJ_ATTRIBUTE_TEXCOORD0,"vTexCoord");

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

		mvpUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_mvp_matrix");
		textureSamplerUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_texture_sampler");

         	final float checkerTexCoord[] = new float[]
         	{
			1.0f,1.0f,
			0.0f,1.0f,
			0.0f,0.0f,
			1.0f,0.0f
		};

		GLES32.glGenVertexArrays(1,vao_checker,0);
		GLES32.glBindVertexArray(vao_checker[0]);

		GLES32.glGenBuffers(1,vbo_position_checker,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_position_checker[0]);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    4*3*4,	
				    null,
				    GLES32.GL_DYNAMIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_POSITION,
					     3,	
					     GLES32.GL_FLOAT,	
					     false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

//--------------------------------------------------------------------------------------------------------------------
		GLES32.glGenBuffers(1,vbo_texture_checker,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_texture_checker[0]);
	
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(checkerTexCoord.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer colorBuffer = byteBuffer.asFloatBuffer();
		colorBuffer.put(checkerTexCoord);
		colorBuffer.position(0);
	
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    checkerTexCoord.length*4,	
				    colorBuffer,
				    GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_TEXCOORD0,
					     2,	
					     GLES32.GL_FLOAT,	
					     false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_TEXCOORD0);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
		GLES32.glBindVertexArray(0);
		
//---------------------------------------------------------------------------------------------------------------------

		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		TextureID[0] = LoadGLTexture();
		GLES32.glEnable(GLES32.GL_TEXTURE_2D);

		GLES32.glClearColor(0.0f,0.0f,0.0f,0.0f);

		Matrix.setIdentityM(perspectiveProjectionMatrix,0);
	}

	private int LoadGLTexture()
	{
		int TEXT_IMAGE[] = new int[1];
	    	Bitmap bitmap = Bitmap.createBitmap(CheckWidth,CheckHeight,Bitmap.Config.ARGB_8888);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(CheckWidth * CheckHeight * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		
		MakeCheckImage();
		byteBuffer.put(CheckImage);
		byteBuffer.position(0);
		bitmap.copyPixelsFromBuffer(byteBuffer);

		GLES32.glGenTextures(1,TEXT_IMAGE,0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D,TEXT_IMAGE[0]);
		
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D,GLES32.GL_TEXTURE_WRAP_S,GLES32.GL_REPEAT);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D,GLES32.GL_TEXTURE_WRAP_T,GLES32.GL_REPEAT); 
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D,GLES32.GL_TEXTURE_MAG_FILTER,GLES32.GL_NEAREST);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D,GLES32.GL_TEXTURE_MIN_FILTER,GLES32.GL_NEAREST);
		GLUtils.texImage2D(GLES32.GL_TEXTURE_2D,0,bitmap,0);	

		return(TEXT_IMAGE[0]);
		
	}

	private void MakeCheckImage()
	{
		int i,j,c;
		for(i = 0 ; i < CheckWidth ; i++)
		{
			for(j = 0 ; j < CheckHeight ; j++)
			{
				c = ((i & 8) ^ (j & 8)) * 255;
				CheckImage[(i * 64 + j) * 4 + 0] = (byte)c;
				CheckImage[(i * 64 + j) * 4 + 1] = (byte)c;
				CheckImage[(i * 64 + j) * 4 + 2] = (byte)c;
				CheckImage[(i * 64 + j) * 4 + 3] = (byte)0XFF;
			}
		}
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
		float modelViewProjectionMatrix[] = new float[16];
		float translateMatrix[] = new float[16];

		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);
		Matrix.setIdentityM(translateMatrix,0);

        	Matrix.translateM(modelViewMatrix,0,0.0f,0.0f,-3.6f);

		Matrix.multiplyMM(modelViewProjectionMatrix,0,perspectiveProjectionMatrix,0,modelViewMatrix,0);
		GLES32.glUniformMatrix4fv(mvpUniform,1,false,modelViewProjectionMatrix,0);

       		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
	    	GLES32.glBindTexture(GLES32.GL_TEXTURE_2D,TextureID[0]);
	    	GLES32.glUniform1i(textureSamplerUniform, 0);

//---------------------------------------------------------------------------------------------------------------------------------
		GLES32.glBindVertexArray(vao_checker[0]);

    		Vertices_straight[0] = -2.0f;
    		Vertices_straight[1] = -1.0f;
    		Vertices_straight[2] = 0.0f;
    		Vertices_straight[3] = -2.0f;
    		Vertices_straight[4] = 1.0f;
    		Vertices_straight[5] = 0.0f;
    		Vertices_straight[6] = 0.0f;
    		Vertices_straight[7] = 1.0f;
    		Vertices_straight[8] = 0.0f;
    		Vertices_straight[9] = 0.0f;
    		Vertices_straight[10] = -1.0f;
    		Vertices_straight[11] = 0.0f;

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_position_checker[0]);

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(Vertices_straight.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(Vertices_straight);
		verticesBuffer.position(0);

		//bitmap.copyPixelsFromBuffer(byteBuffer);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    Vertices_straight.length *4,	
				    verticesBuffer,
				    GLES32.GL_DYNAMIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_POSITION,
					     3,	
					     GLES32.GL_FLOAT,	
					     false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

                GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,0,4);	
		GLES32.glBindVertexArray(0);

//---------------------------------------------------------------------------------------------------------------------------------
		GLES32.glBindVertexArray(vao_checker[0]);

    		Vertices_angular[0] = 1.0f;
    		Vertices_angular[1] = -1.0f;
   		Vertices_angular[2] = 0.0f;
   		Vertices_angular[3] = 1.0f;
    		Vertices_angular[4] = 1.0f;
    		Vertices_angular[5] = 0.0f;
    		Vertices_angular[6] = 2.41421f;
    		Vertices_angular[7] = 1.0f;
    		Vertices_angular[8] = -1.41421f;
    		Vertices_angular[9] = 2.41421f;
    		Vertices_angular[10] = -1.0f;
    		Vertices_angular[11] = -1.41421f;

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_position_checker[0]);

		byteBuffer = ByteBuffer.allocateDirect(Vertices_angular.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(Vertices_angular);
		verticesBuffer.position(0);

		//bitmap.copyPixelsFromBuffer(byteBuffer);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    Vertices_angular.length *4,	
				    verticesBuffer,
				    GLES32.GL_DYNAMIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_POSITION,
					     3,	
					     GLES32.GL_FLOAT,	
					     false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

                GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,0,4);	
		GLES32.glBindVertexArray(0);
//---------------------------------------------------------------------------------------------------------------------------------
		GLES32.glUseProgram(0);	
		
		requestRender();
	}

	void uninitialize()
	{

		if(vao_checker[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1,vao_checker,0);
			vao_checker[0] = 0;
		}

		if(vbo_position_checker[0] != 0)
		{
			GLES32.glDeleteBuffers(1,vbo_position_checker,0);
			vbo_position_checker[0] = 0;
		}

		if(vbo_texture_checker[0] != 0)
		{
			GLES32.glDeleteBuffers(1,vbo_texture_checker,0);
			vbo_texture_checker[0] = 0;
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