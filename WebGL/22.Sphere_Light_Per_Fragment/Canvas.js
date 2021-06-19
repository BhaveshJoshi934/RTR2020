// JavaScript source code

var canvas = null;
var gl = null;
var canvas_original_width;
var canvas_original_height;
var bFullscreen = false;

const WebGLMacros = 
{
	BDJ_ATTRIBUTE_POSITION:0,
    BDJ_ATTRIBUTE_COLOR:1,
    BDJ_ATTRIBUTE_NORMAL:2,
    BDJ_ATTRIBUTE_TEXTURE0:3,

};

var vertexShaderObject;
var fragmentShaderObject;
var shaderProgramObject;

var lightPosition = [100.0,100.0,100.0,1.0];
var lightAmbient = /*{0.0f,0.0f,0.0f,1.0f};*/  [0.1,0.1,0.1];
var lightDiffuse = /*{1.0f,1.0f,1.0f,1.0f}; */ [0.5,0.2,0.7];
var lightSpecular = /*{1.0f,1.0f,1.0f,1.0f}; */ [0.7,0.7,0.7];
var MaterialAmbient = [0.0,0.0,0.0];
var MaterialDiffuse = [1.0,1.0,1.0];
var MaterialSpecular = [1.0,1.0,1.0];

var MaterialShininess = /*50.0f;*/ 128.0;

var sphere = null;

var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;

var lightPositionUniform;
var LaUniform;
var LdUniform;
var LsUniform;

var KaUniform;
var KdUniform;
var KsUniform;
var KshineUniform;

var LKeyPressedUniform;

var bLight = false;

var perspeciveProjectionMatrix;

var requestAnimationFrame = window.requestAnimationFrame ||
							window.webkitRequestAnimationFrame ||
							window.mozRequestAnimationFrame ||
							window.oRequestAnimationFrame ||
							window.msRequestAnimationFrame;

var cancelAnimationFrame = window.cancelAnimationFrame ||
						   window.webkitCancelRequestAnimationFrame || window.webkitCancelAnimationFrame ||
						   window.mozCancelRequestAnimationFrame || window.mozCancelAnimationFrame ||
						   window.oCancelRequestAnimationFrame || window.oCancelAnimationFrame ||
						   window.msCancelRequestAnimationFrame || window.msCancelAnimationFrame;

function main()
{
	canvas = document.getElementById("BDJ");  //Type Inference : Variable chya ujvya hatala kay yenar ahe he guess karan...Eg. Auto in C
	//document he inbuilt interface ahe...He Document define karato

	if(!canvas)
	{
		console.log("Obataining Canvas Failed/n");
	}
	else
	{
		console.log("Obataining Canvas Succeded/n");
	}

	canvas_original_width = canvas.width;
	canvas_original_height = canvas.height; 

	//"window" is inbuilt variable like document used above...
	//window is DOM object as it is inherited from document...
	window.addEventListener("keydown", keyDown, false);  // 1st param : Inbuilt event ; 2nd param : my function ; 3rd param : following bubble propagation not capture propagation
	window.addEventListener("click", mouseDown, false);  // 1st param : Inbuilt event ; 2nd param : my function ; 3rd param : following bubble propagation not capture propagation 

	window.addEventListener("resize",resize,false);

	init();

	resize(); // Warmup Resize()

	draw(); // Warmup Repaint()

}

function toggleFullScreen()
{
	var fullscreen_element = document.fullscreen_Element ||              
							 document.webkitFullscreenElement ||
							 document.mozFullScreenElement ||
							 document.msFullscreenElement ||
							 null;

	if(fullscreen_element == null)
	{
		if(canvas.requestFullscreen)   // Ithe apan fuction pointer check kela...Ki null nahiye na ? ani mg aat gelo
		{
			canvas.requestFullscreen();
		}
		else if(canvas.webkitRequestFullscreen)
		{
			canvas.webkitRequestFullscreen();
		}
		else if(canvas.mozRequestFullScreen)
		{
			canvas.mozRequestFullScreen();
		}
		else if(canvas.msRequestFullscreen)
		{
			canvas.msRequestFullscreen();
		}
		bFullscreen = true;
	}
	else
	{
		if(document.exitFullscreen)
		{
			document.exitFullscreen();
		}
		else if(document.webkitExitFullscreen)
		{
			document.webkitExitFullscreen();
		}
		else if(document.mozCancelFullScreen)
		{
			document.mozCancelFullScreen();
		}
		else if(document.msExitFullscreen)
		{
			document.msExitFullscreen();
		}
		bFullscreen = false;
	}
							
}

function keyDown(event) // Type Inference : type runtime la tharel
{
	switch(event.keyCode)
	{
		case 27:
			uninitialize();
			window.close();
			break;

		case 70:
			toggleFullScreen(); 
			break;

		case 76:
			if(bLight == false)
				bLight = true;
			else
				bLight = false;
			break;
	}
}

function mouseDown(event)
{
}

function init()
{

	//To maintain our own code consistency

	gl = canvas.getContext("webgl2");

	if(!gl)
	{
		console.log("Webgl2 Context is Fail to received/n");
	}
	else
	{
		console.log("Webgl2 Context is received/n");
	}

	gl.viewportWidth = canvas.width;
	gl.viewportHeight = canvas.height;

	vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

	var vertexShaderSourceCode = 
	"#version 300 es"+
	"\n"+
    "in vec4 vPosition;" +
	"in vec3 vNormal;" +
	"uniform mat4 u_model_matrix;" +
	"uniform mat4 u_view_matrix;" +
	"uniform mat4 u_perspective_projection_matrix;" +
	"uniform vec4 u_light_position;" +
	"uniform int u_LKeyPressed;" +
	"out vec3 transformed_normal;" +
	"out vec3 light_direction;" +
	"out vec3 view_vector;" +
	"void main(void)" +
	"{"+
	"vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" +
	"transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal;" +
	"light_direction = vec3(u_light_position - eye_coordinates);" +
	"view_vector = -eye_coordinates.xyz;" +
	"gl_Position = u_perspective_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" +
	"}";

	gl.shaderSource(vertexShaderObject,vertexShaderSourceCode);
	gl.compileShader(vertexShaderObject);

	if(gl.getShaderParameter(vertexShaderObject,gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(vertexShaderObject);
		if(error.length > 0)
		{
			alert(error);
			uninitialize();
		}
	}

	fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

	var fragmentShaderSourceCode = 
	"#version 300 es"+
	"\n"+
	"precision highp float;"+
	"in vec3 transformed_normal;" +
	"in vec3 light_direction;" +
	"in vec3 view_vector;" +
	"uniform vec3 u_la;" +
	"uniform vec3 u_ld;" +
	"uniform vec3 u_ls;" +
	"uniform vec3 u_ka;" +
	"uniform vec3 u_kd;" +
	"uniform vec3 u_ks;" +
	"uniform float u_shininess;" +
	"uniform int u_LKeyPressed;" +
	"out vec4 FragColor;" +
	"void main(void)" +
	"{" +
	"vec3 phong_ads_light;" +
	"if(u_LKeyPressed == 1)" +
	"{" +
	"vec3 normalized_transformed_normal = normalize(transformed_normal);" +
	"vec3 normalized_light_direction    = normalize(light_direction);" +
	"vec3 normalized_view_vector = normalize(view_vector);" +
	"vec3 ambient = u_la * u_ka;" +
	"vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction,normalized_transformed_normal),0.0f);" +
	"vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normal);" +
	"vec3 specular = u_ls * u_ks * pow(max(dot(reflection_vector,normalized_view_vector),0.0f),u_shininess);" +
	"phong_ads_light = ambient + diffuse + specular;" +
	"}" +
	"else" +
	"{" +
	"phong_ads_light = vec3(1.0f,1.0f,1.0f);" +
	"}" +
	"FragColor = vec4(phong_ads_light,1.0f);" +
	"}";

	gl.shaderSource(fragmentShaderObject,fragmentShaderSourceCode);
	gl.compileShader(fragmentShaderObject);
	if(gl.getShaderParameter(fragmentShaderObject,gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(fragmentShaderObject);
		if(error.length > 0)
		{
			alert(error);
			uninitialize();
		}
	}

	shaderProgramObject = gl.createProgram();

	gl.attachShader(shaderProgramObject,vertexShaderObject);
	gl.attachShader(shaderProgramObject,fragmentShaderObject);

	gl.bindAttribLocation(shaderProgramObject,WebGLMacros.BDJ_ATTRIBUTE_POSITION,"vPosition");
	gl.bindAttribLocation(shaderProgramObject,WebGLMacros.BDJ_ATTRIBUTE_NORMAL,"vNormal");


	gl.linkProgram(shaderProgramObject);

	if(gl.getProgramParameter(shaderProgramObject,gl.LINK_STATUS) == false)
	{
		var error = gl.getProgramInfoLog(shaderProgramObject);
		if(error.length > 0)
		{
			alert(error);
			uninitialize();
		}
	}

    modelMatrixUniform = gl.getUniformLocation(shaderProgramObject,"u_model_matrix");
    viewMatrixUniform = gl.getUniformLocation(shaderProgramObject,"u_view_matrix");
    projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject,"u_perspective_projection_matrix");
    LaUniform = gl.getUniformLocation(shaderProgramObject,"u_la");
    LdUniform = gl.getUniformLocation(shaderProgramObject,"u_ld");
    LsUniform = gl.getUniformLocation(shaderProgramObject,"u_ls");
    lightPositionUniform = gl.getUniformLocation(shaderProgramObject,"u_light_position");
    KaUniform = gl.getUniformLocation(shaderProgramObject,"u_ka");
    KdUniform = gl.getUniformLocation(shaderProgramObject,"u_kd");
    KsUniform = gl.getUniformLocation(shaderProgramObject,"u_ks");
    KshineUniform = gl.getUniformLocation(shaderProgramObject,"u_shininess");
    LKeyPressedUniform = gl.getUniformLocation(shaderProgramObject,"u_LKeyPressed");

	gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

	sphere = new Mesh();
	makeSphere(sphere,2.0,30,30);

	gl.clearColor(0.0,0.0,0.0,1.0);

	perspectiveProjectionMatrix = mat4.create();
}

function resize()
{
	if(bFullscreen == true)
	{
		canvas.width = window.innerWidth; // Fakt client area havay so inner width use kelay...
		canvas.height = window.innerHeight;
	}
	else
	{
		canvas.width = canvas_original_width;
		canvas.height = canvas_original_height;
	}

	gl.viewport(0,0,canvas.width,canvas.height);
	
	mat4.perspective(perspectiveProjectionMatrix,45.0,parseFloat(canvas.width)/parseFloat(canvas.height),0.1,100.0);
}

function draw()
{
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	gl.useProgram(shaderProgramObject);

	if(bLight == true)
    {
        gl.uniform1i(LKeyPressedUniform,1);
        gl.uniform1f(KshineUniform,MaterialShininess);      // Ithe Error Yeu Shakate
        gl.uniform4fv(lightPositionUniform,lightPosition);
        gl.uniform3fv(LaUniform,lightAmbient);
        gl.uniform3fv(LdUniform,lightDiffuse);
        gl.uniform3fv(LsUniform,lightSpecular);
        gl.uniform3fv(KaUniform,MaterialAmbient);
        gl.uniform3fv(KdUniform,MaterialDiffuse);
        gl.uniform3fv(KsUniform,MaterialSpecular);
    }
    else
    {
        gl.uniform1i(LKeyPressedUniform,0);
    }

	var modelViewMatrix = mat4.create();
	var viewMatrix = mat4.create();
	var projectionMatrix = mat4.create();
	var translateMatrix = mat4.create();

	modelMatrix = translateMatrix;
	projectionMatrix = perspectiveProjectionMatrix;

	mat4.translate(translateMatrix,translateMatrix,[0.0,0.0,-10.0]);

	//mat4.multiply(modelViewProjectionMatrix,perspectiveProjectionMatrix,modelViewMatrix);

	gl.uniformMatrix4fv(modelMatrixUniform,false,modelMatrix);
	gl.uniformMatrix4fv(viewMatrixUniform,false,viewMatrix);
	gl.uniformMatrix4fv(projectionMatrixUniform,false,projectionMatrix);

	sphere.draw();

	gl.useProgram(null);

	requestAnimationFrame(draw,canvas);
}

function uninitialize()
{
	if(sphere)
	{
		sphere.deallocate();
	}
	
	if(shaderProgramObject)
	{
		if(fragmentShaderObject)
		{
			gl.detachShader(shaderProgramObject,fragmentShaderObject);
			gl.deleteShader(fragmentShaderObject);
			fragmentShaderObject = null;
		}

		if(vertexShaderObject)
		{
			gl.detachShader(shaderProgramObject,vertexShaderObject);
			gl.deleteShader(vertexShaderObject);
			vertexShaderObject = null;
		}

		gl.deleteProgram(shaderProgramObject);
		shaderProgramObject = null;
	}
	
}