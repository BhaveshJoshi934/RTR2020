package com.example.two_lights;

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

	static float angle_pyramid = 0.0f;

	private int[] vao_pyramid = new int[1];
	private int[] vbo_position_pyramid = new int[1];
	private int[] vbo_normal_pyramid = new int[1]; 

	float lightPosition_Red[] = {-2.0f,0.0f,0.0f,1.0f};
	float lightAmbient_Red[] = {0.0f,0.0f,0.0f,1.0f};
	float lightDiffuse_Red[] = {1.0f,0.0f,0.0f,1.0f};
	float lightSpecular_Red[] = {1.0f,0.0f,0.0f,1.0f};

	float lightPosition_Blue[] = {2.0f,0.0f,0.0f,1.0f};
	float lightAmbient_Blue[] = {0.0f,0.0f,0.0f,1.0f};
	float lightDiffuse_Blue[] = {0.0f,0.0f,1.0f,1.0f};
	float lightSpecular_Blue[] = {0.0f,0.0f,1.0f,1.0f};

	float MaterialAmbient[] = {0.0f,0.0f,0.0f,0.0f};
	float MaterialDiffuse[] = {1.0f,1.0f,1.0f,1.0f};
	float MaterialSpecular[] = {1.0f,1.0f,1.0f,1.0f};
	float MaterialShininess = 128.0f;

	private int modelMatrixUniform;                             
	private int viewMatrixUniform;
	private int perspectiveProjectionUniform;

	private int LaUniform_Red;
	private int LdUniform_Red;
	private int LsUniform_Red;
	private int lightPositionUniform_Red;

	private int LaUniform_Blue;
	private int LdUniform_Blue;
	private int LsUniform_Blue;
	private int lightPositionUniform_Blue;

	private int KaUniform;
	private int KdUniform;
	private int KsUniform;
	private int KshineUniform;

	private int LKeyPressedUniform;

	public int tap;
	private boolean bLight;

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
		if(tap == 0)
			tap = 1;
		else
			tap = 0;
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
		if(bLight == false)
			bLight = true;	
		else
			bLight = false;
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
            "in vec3 vNormal;"+
            "uniform mat4 u_model_matrix;"+
            "uniform mat4 u_view_matrix;"+
            "uniform mat4 u_perspective_projection_matrix;"+
            "uniform vec3 u_la_red;"+
            "uniform vec3 u_ld_red;"+
            "uniform vec3 u_ls_red;"+
            "uniform vec4 u_light_position_red;"+
            "uniform vec3 u_la_blue;"+
            "uniform vec3 u_ld_blue;"+
            "uniform vec3 u_ls_blue;"+
            "uniform vec4 u_light_position_blue;"+
            "uniform vec3 u_ka;"+
            "uniform vec3 u_kd;"+
            "uniform vec3 u_ks;"+
            "uniform int u_LKeyPressed;"+
            "uniform float u_shininess;"+
            "out vec3 phong_ads_light;"+
            "void main(void)"+
            "{"+
            "if(u_LKeyPressed == 1)"+
            "{"+
            "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;"+
            "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);"+
            "vec3 view_vector = normalize(-eye_coordinates.xyz);"+

            "vec3 light_direction_red = normalize(vec3(u_light_position_red - eye_coordinates));"+
            "vec3 reflection_vector_red = reflect(-light_direction_red,transformed_normal);"+
            "vec3 ambient_red = u_la_red * u_ka;"+
            "vec3 diffuse_red = u_ld_red * u_kd * max(dot(light_direction_red,transformed_normal),0.0f);"+
            "vec3 specular_red = u_ls_red * u_ks * pow(max(dot(reflection_vector_red,view_vector),0.0f),u_shininess);"+

            "vec3 light_direction_blue = normalize(vec3(u_light_position_blue - eye_coordinates));"+
            "vec3 reflection_vector_blue = reflect(-light_direction_blue,transformed_normal);"+
            "vec3 ambient_blue = u_la_blue * u_ka;"+
            "vec3 diffuse_blue = u_ld_blue * u_kd * max(dot(light_direction_blue,transformed_normal),0.0f);"+
            "vec3 specular_blue = u_ls_blue * u_ks * pow(max(dot(reflection_vector_blue,view_vector),0.0f),u_shininess);"+

            "phong_ads_light = ambient_red + ambient_blue + diffuse_red + diffuse_blue + specular_red + specular_blue;"+
            "}"+
            "else"+
            "{"+
            "phong_ads_light = vec3(1.0f,1.0f,1.0f);"+
            "}"+
            "gl_Position = u_perspective_projection_matrix * u_view_matrix * u_model_matrix * vPosition;"+
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
	    "in vec3 phong_ads_light;"+
        "out vec4 FragColor;"+
        "void main(void)"+
        "{"+
        "FragColor = vec4(phong_ads_light,1.0f);"+
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

		//mvpUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_mvp_matrix");
		modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_model_matrix");
    	viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_view_matrix");
    	perspectiveProjectionUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_perspective_projection_matrix");
    	LaUniform_Red = GLES32.glGetUniformLocation(shaderProgramObject,"u_la_red");
    	LdUniform_Red = GLES32.glGetUniformLocation(shaderProgramObject,"u_ld_red");
   		LsUniform_Red = GLES32.glGetUniformLocation(shaderProgramObject,"u_ls_red");
    	lightPositionUniform_Red = GLES32.glGetUniformLocation(shaderProgramObject,"u_light_position_red");
    	LaUniform_Blue = GLES32.glGetUniformLocation(shaderProgramObject,"u_la_blue");
    	LdUniform_Blue = GLES32.glGetUniformLocation(shaderProgramObject,"u_ld_blue");
   		LsUniform_Blue = GLES32.glGetUniformLocation(shaderProgramObject,"u_ls_blue");
    	lightPositionUniform_Blue = GLES32.glGetUniformLocation(shaderProgramObject,"u_light_position_blue");
    	KaUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_ka");
    	KdUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_kd");
    	KsUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_ks");
    	KshineUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_shininess");
    	LKeyPressedUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_LKeyPressed");

     	final float pyramidVertices[] = new float[]
         {
                	//front
		        0.0f,1.0f,0.0f,	//apex
		        -1.0f,-1.0f,1.0f,	//left bottom
		        1.0f,-1.0f,1.0f,	//right bottom

		        //right
		        0.0f,1.0f,0.0f,
		        1.0f,-1.0f,1.0f,
		        1.0f,-1.0f,-1.0f,

		        //back
		        0.0f,1.0f,0.0f,
		        1.0f,-1.0f,-1.0f,
		        -1.0f,-1.0f,-1.0f,

		        //left
		        0.0f,1.0f,0.0f,
		        -1.0f,-1.0f,-1.0f,
		        -1.0f,-1.0f,1.0f
         };

         final float pyramidNormals[] = new float[] 
         {
         	    //FRONT FACE
                0.0f,0.447214f,0.894427f,
                0.0f,0.447214f,0.894427f,
                0.0f,0.447214f,0.894427f,
                0.0f,0.447214f,0.894427f,

                //RIGHT FACE
                0.894427f, 0.447214f, 0.0f,
                0.894427f, 0.447214f, 0.0f,
                0.894427f, 0.447214f, 0.0f,
                0.894427f, 0.447214f, 0.0f,

                //BACK FACE
                0.0f, 0.447214f, -0.894427f,
                0.0f, 0.447214f, -0.894427f,
                0.0f, 0.447214f, -0.894427f,
                0.0f, 0.447214f, -0.894427f,

                //LEFT FACE
                -0.894427f, 0.447214f, 0.0f,
                -0.894427f, 0.447214f, 0.0f,
                -0.894427f, 0.447214f, 0.0f,
                -0.894427f, 0.447214f, 0.0f
         };

		GLES32.glGenVertexArrays(1,vao_pyramid,0);
		GLES32.glBindVertexArray(vao_pyramid[0]);

		GLES32.glGenBuffers(1,vbo_position_pyramid,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_position_pyramid[0]);

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pyramidVertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(pyramidVertices);
		verticesBuffer.position(0);
	
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    pyramidVertices.length*4,	
				    verticesBuffer,
				    GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_POSITION,
					     3,	
					     GLES32.GL_FLOAT,	
					     false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_POSITION);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
//-------------------------------------------------------------------------------------------------------
		GLES32.glGenBuffers(1,vbo_normal_pyramid,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_normal_pyramid[0]);

		byteBuffer = ByteBuffer.allocateDirect(pyramidNormals.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(pyramidNormals);
		verticesBuffer.position(0);
	
		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,	
				    pyramidNormals.length*4,	
				    verticesBuffer,
				    GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_NORMAL,
					     3,	
					     GLES32.GL_FLOAT,	
					     false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_NORMAL);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

		GLES32.glBindVertexArray(0);
//-------------------------------------------------------------------------------------------------------
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
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

	if(bLight == true)
    {
        GLES32.glUniform1i(LKeyPressedUniform,1);
        GLES32.glUniform1f(KshineUniform,MaterialShininess);
        GLES32.glUniform4fv(lightPositionUniform_Red,1,lightPosition_Red,0);
        GLES32.glUniform3fv(LaUniform_Red,1,lightAmbient_Red,0);
        GLES32.glUniform3fv(LdUniform_Red,1,lightDiffuse_Red,0);
        GLES32.glUniform3fv(LsUniform_Red,1,lightSpecular_Red,0);
        GLES32.glUniform4fv(lightPositionUniform_Blue,1,lightPosition_Blue,0);
        GLES32.glUniform3fv(LaUniform_Blue,1,lightAmbient_Blue,0);
        GLES32.glUniform3fv(LdUniform_Blue,1,lightDiffuse_Blue,0);
        GLES32.glUniform3fv(LsUniform_Blue,1,lightSpecular_Blue,0);
        GLES32.glUniform3fv(KaUniform,1,MaterialAmbient,0);
        GLES32.glUniform3fv(KdUniform,1,MaterialDiffuse,0);
        GLES32.glUniform3fv(KsUniform,1,MaterialSpecular,0);
    }
    else
    {
        GLES32.glUniform1i(LKeyPressedUniform,0);
    }
	
		float modelMatrix[] = new float[16];
		float viewMatrix[] = new float[16];
		float projectionMatrix[] = new float[16];
		float translateMatrix[] = new float[16];
		float rotateMatrix[] = new float[16];

		Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);
		Matrix.setIdentityM(projectionMatrix,0);
		Matrix.setIdentityM(translateMatrix,0);
		Matrix.setIdentityM(rotateMatrix,0);

        Matrix.translateM(translateMatrix,0,0.0f,0.0f,-5.5f);
        Matrix.rotateM(rotateMatrix,0,angle_pyramid,0.0f,1.0f,0.0f);

		Matrix.multiplyMM(translateMatrix,0,translateMatrix,0,rotateMatrix,0);

        modelMatrix = translateMatrix;
        projectionMatrix = perspectiveProjectionMatrix;

		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
    	GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
    	GLES32.glUniformMatrix4fv(perspectiveProjectionUniform,1,false,projectionMatrix,0);

		GLES32.glBindVertexArray(vao_pyramid[0]);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLES,0,12);	
		GLES32.glBindVertexArray(0);
//---------------------------------------------------------------------------------------------
		GLES32.glUseProgram(0);	

		if(tap == 1)
		{
			angle_pyramid = angle_pyramid + 1.0f;
		}

		if(angle_pyramid >= 360.0f)
		{
			angle_pyramid = 0.0f;
		}
		
		requestRender();
	}

	void uninitialize()
	{

        // destroy vao
        if(vao_pyramid[0] != 0)
        {
            GLES32.glDeleteVertexArrays(1, vao_pyramid, 0);
            vao_pyramid[0]=0;
        }
        
        // destroy position vbo
        if(vbo_position_pyramid[0] != 0)
        {
            GLES32.glDeleteBuffers(1, vbo_position_pyramid, 0);
            vbo_position_pyramid[0]=0;
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