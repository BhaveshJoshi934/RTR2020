package com.example.smiley;

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
		
	private GestureDetector gestureDetector;

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

	private int[] vao_cube = new int[1];
	private int[] vbo_position_cube = new int[1];
	private int[] vbo_texture_cube = new int[1]; 

	private int Smiley_Texture;

	private int textureSamplerUniform;

	private int mvpUniform;

	public int tap;

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

         final float cubeVertices[] = new float[]
         {
		1.0f,1.0f,0.0f,
		-1.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f
	 };

         final float cubeTexCoord[] =new float[]
         {
                    
		        1.0f, 1.0f,
		        0.0f, 1.0f,
		       	0.0f,0.0f,
		        1.0f, 0.0f,

         };

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

		//GLES32.glBindVertexArray(0);

//--------------------------------------------------------------------------------------------------------------------
		
		GLES32.glGenBuffers(1,vbo_texture_cube,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_texture_cube[0]);

		byteBuffer = ByteBuffer.allocateDirect(cubeTexCoord.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer colorBuffer_cube = byteBuffer.asFloatBuffer();
		colorBuffer_cube.put(cubeTexCoord);
		colorBuffer_cube.position(0);
	
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    cubeTexCoord.length*4,	
				    colorBuffer_cube,
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
		//GLES32.glEnable(GLES32.GL_CULL_FACE);

		Smiley_Texture = LoadGLTexture(R.raw.smiley);

		GLES32.glClearColor(0.0f,0.0f,0.0f,0.0f);

		Matrix.setIdentityM(perspectiveProjectionMatrix,0);
	}

	private int LoadGLTexture(int imageFileResourceID)
	{
           
            //constructor call
            BitmapFactory.Options options = new BitmapFactory.Options();

            options.inScaled = false;

            //variable declaration

	    //OS bitmap loading.
	    Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(),imageFileResourceID,options);

            int texture[] = new int[1];

	        //OpenGL code
	        GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 4);
	        GLES32.glGenTextures(1, texture,0);
	        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);

	        //setting  texture parameter
	        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
	        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR_MIPMAP_LINEAR);

	        //follwing call will actually push the data into graphic memory
            	GLUtils.texImage2D(GLES32.GL_TEXTURE_2D,0,bitmap,0);
	        GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);
	        GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);

	        return(texture[0]);	
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

        	Matrix.translateM(modelViewMatrix,0,0.0f,0.0f,-3.0f);

		Matrix.multiplyMM(modelViewProjectionMatrix,0,perspectiveProjectionMatrix,0,modelViewMatrix,0);
		GLES32.glUniformMatrix4fv(mvpUniform,1,false,modelViewProjectionMatrix,0);

        	GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
	    	GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, Smiley_Texture);
	    	GLES32.glUniform1i(textureSamplerUniform, 0);

		GLES32.glBindVertexArray(vao_cube[0]);
                GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,0,4);	
		GLES32.glBindVertexArray(0);

		GLES32.glUseProgram(0);	
		
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

		if(vbo_texture_cube[0] != 0)
		{
			GLES32.glDeleteBuffers(1,vbo_texture_cube,0);
			vbo_texture_cube[0] = 0;
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