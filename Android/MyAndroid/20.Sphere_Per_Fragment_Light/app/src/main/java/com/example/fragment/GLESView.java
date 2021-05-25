package com.example.fragment;

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
import java.nio.ShortBuffer;

import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer,OnGestureListener,OnDoubleTapListener
{
	private final Context context;
		
	private GestureDetector gestureDetector;

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

    private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
   	private int[] vbo_sphere_element = new int[1];
	int numVertices;
    int numElements;

	private int modelMatrixUniform;                             
	private int viewMatrixUniform;
	private int perspectiveProjectionUniform;

	private int LaUniform;
	private int LdUniform;
	private int LsUniform;
	private int lightPositionUniform;

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
		System.out.println("BDJ : Double Tap!!!");
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
            "uniform vec4 u_light_position;"+
            "uniform int u_LKeyPressed;"+
            "out vec3 transformed_normal;"+
            "out vec3 light_direction;"+
            "out vec3 view_vector;"+
            "void main(void)"+
            "{"+
            "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;"+
            "transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal;"+
            "light_direction = vec3(u_light_position - eye_coordinates);"+
            "view_vector = -eye_coordinates.xyz;"+
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
        "in vec3 transformed_normal;"+
        "in vec3 light_direction;"+
        "in vec3 view_vector;"+
        "uniform vec3 u_la;"+
        "uniform vec3 u_ld;"+
        "uniform vec3 u_ls;"+
        "uniform vec3 u_ka;"+
        "uniform vec3 u_kd;"+
        "uniform vec3 u_ks;"+
        "uniform highp float u_shininess;"+
        "uniform int u_LKeyPressed;"+
        "out vec4 FragColor;"+
        "void main(void)"+
        "{"+
        "vec3 phong_ads_light;"+
        "if(u_LKeyPressed == 1)"+
        "{"+
        "vec3 normalized_transformed_normal = normalize(transformed_normal);"+
        "vec3 normalized_light_direction    = normalize(light_direction);"+
        "vec3 normalized_view_vector        = normalize(view_vector);"+
        "vec3 ambient = u_la * u_ka;"+
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction,normalized_transformed_normal),0.0f);"+
        "vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normal);"+
        "vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector,normalized_view_vector),0.0f),u_shininess);"+
        "phong_ads_light = ambient + diffuse + specular;"+
        "}"+
        "else"+
        "{"+
        "phong_ads_light = vec3(1.0f,1.0f,1.0f);"+
        "}"+
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
    	LaUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_la");
    	LdUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_ld");
   		LsUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_ls");
    	lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_light_position");
    	KaUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_ka");
    	KdUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_kd");
    	KsUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_ks");
    	KshineUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_shininess");
    	LKeyPressedUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_LKeyPressed");

            Sphere sphere = new Sphere();
        	float sphere_vertices[]=new float[1146];
        	float sphere_normals[]=new float[1146];
        	float sphere_textures[]=new float[764];
        	short sphere_elements[]=new short[2280];
        	sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
        	numVertices = sphere.getNumberOfSphereVertices();
        	numElements = sphere.getNumberOfSphereElements();

        // vao
        GLES32.glGenVertexArrays(1,vao_sphere,0);
        GLES32.glBindVertexArray(vao_sphere[0]);
        
        // position vbo
        GLES32.glGenBuffers(1,vbo_sphere_position,0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_sphere_position[0]);
        
        ByteBuffer byteBuffer=ByteBuffer.allocateDirect(sphere_vertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer=byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_vertices);
        verticesBuffer.position(0);
        
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                            sphere_vertices.length * 4,
                            verticesBuffer,
                            GLES32.GL_STATIC_DRAW);
        
        GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_POSITION,
                                     3,
                                     GLES32.GL_FLOAT,
                                     false,0,0);
        
        GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_POSITION);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
        
        // normal vbo
        GLES32.glGenBuffers(1,vbo_sphere_normal,0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_sphere_normal[0]);
        
        byteBuffer=ByteBuffer.allocateDirect(sphere_normals.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer=byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_normals);
        verticesBuffer.position(0);
        
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                            sphere_normals.length * 4,
                            verticesBuffer,
                            GLES32.GL_STATIC_DRAW);
        
        GLES32.glVertexAttribPointer(GLESMacros.BDJ_ATTRIBUTE_NORMAL,
                                     3,
                                     GLES32.GL_FLOAT,
                                     false,0,0);
        
        GLES32.glEnableVertexAttribArray(GLESMacros.BDJ_ATTRIBUTE_NORMAL);
        
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
        
        // element vbo
        GLES32.glGenBuffers(1,vbo_sphere_element,0);
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER,vbo_sphere_element[0]);
        
        byteBuffer=ByteBuffer.allocateDirect(sphere_elements.length * 2);
        byteBuffer.order(ByteOrder.nativeOrder());
        ShortBuffer elementsBuffer=byteBuffer.asShortBuffer();
        elementsBuffer.put(sphere_elements);
        elementsBuffer.position(0);
        
        GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER,
                            sphere_elements.length * 2,
                            elementsBuffer,
                            GLES32.GL_STATIC_DRAW);
        
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER,0);

        GLES32.glBindVertexArray(0);

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
        float lightPosition[] = {100.0f,100.0f,100.0f,1.0f};
        float lightAmbient[] = {0.0f,0.0f,0.0f,1.0f};
        float lightDiffuse[] = {1.0f,1.0f,1.0f,1.0f}; 
        float lightSpecular[] = {1.0f,1.0f,1.0f,1.0f}; 
        float MaterialAmbient[] = {0.0f,0.0f,0.0f,0.0f};
        float MaterialDiffuse[] = {1.0f,1.0f,1.0f,1.0f};
        float MaterialSpecular[] = {1.0f,1.0f,1.0f,1.0f};

        float MaterialShininess = 50.0f;

        GLES32.glUniform1i(LKeyPressedUniform,1);
        GLES32.glUniform1f(KshineUniform,MaterialShininess);      // Ithe Error Yeu Shakate
        GLES32.glUniform4fv(lightPositionUniform,1,lightPosition,0);
        GLES32.glUniform3fv(LaUniform,1,lightAmbient,0);
        GLES32.glUniform3fv(LdUniform,1,lightDiffuse,0);
        GLES32.glUniform3fv(LsUniform,1,lightSpecular,0);
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

		Matrix.setIdentityM(modelMatrix,0);
		Matrix.setIdentityM(viewMatrix,0);
		Matrix.setIdentityM(projectionMatrix,0);
		Matrix.setIdentityM(translateMatrix,0);

        Matrix.translateM(translateMatrix,0,0.0f,0.0f,-2.5f);

        modelMatrix = translateMatrix;
        projectionMatrix = perspectiveProjectionMatrix;

		//Matrix.multiplyMM(modelMatrix,0,viewMatrix,0,projectionMatrix,0);

		GLES32.glUniformMatrix4fv(modelMatrixUniform,1,false,modelMatrix,0);
    	GLES32.glUniformMatrix4fv(viewMatrixUniform,1,false,viewMatrix,0);
    	GLES32.glUniformMatrix4fv(perspectiveProjectionUniform,1,false,projectionMatrix,0);

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements, GLES32.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
        GLES32.glBindVertexArray(0);
//---------------------------------------------------------------------------------------------
		GLES32.glUseProgram(0);	
		
		requestRender();
	}

	void uninitialize()
	{

        // destroy vao
        if(vao_sphere[0] != 0)
        {
            GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
            vao_sphere[0]=0;
        }
        
        // destroy position vbo
        if(vbo_sphere_position[0] != 0)
        {
            GLES32.glDeleteBuffers(1, vbo_sphere_position, 0);
            vbo_sphere_position[0]=0;
        }
        
        // destroy normal vbo
        if(vbo_sphere_normal[0] != 0)
        {
            GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
            vbo_sphere_normal[0]=0;
        }
        
        // destroy element vbo
        if(vbo_sphere_element[0] != 0)
        {
            GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
            vbo_sphere_element[0]=0;
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