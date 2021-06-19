// JavaScript source code

var canvas = null;
var gl = null;
var canvas_original_width;
var canvas_original_height;
var bFullscreen = false;

var angle_pyramid = 0.0;
var angle_cube = 0.0;

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

var bLight;

var vao_pyramid;
var vbo_Position_pyramid;
var vbo_Normal_pyramid;

var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;
var LKeyPressedUniform;
var LaUniform_Red;
var LdUniform_Red;
var LsUniform_Red;
var LaUniform_Blue;
var LdUniform_Blue;
var LsUniform_Blue;
var KaUniform;
var KdUnifrom;
var KsUniform;
var KshineUniform;
var lightPositionUniform_Red;
var lightPositionUniform_Blue;

var lightPosition_Red = [-2.0,0.0,0.0,1.0];
var lightAmbient_Red = [0.1,0.1,0.1];
var lightDiffuse_Red = [1.0,0.0,0.0];
var lightSpecular_Red = [1.0,0.0,0.0];

var lightPosition_Blue = [2.0,0.0,0.0,1.0];
var lightAmbient_Blue = [0.1,0.1,0.1];
var lightDiffuse_Blue = [0.0,0.0,1.0];
var lightSpecular_Blue = [0.0,0.0,1.0];

var materialAmbient = [0.0,0.0,0.0];
var materialDiffuse = [1.0,1.0,1.0];
var materialSpecular = [1.0,1.0,1.0];
var materialShininess = 50.0;

var perspectiveProjectionMatrix;

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

		case 76:
		case 108:
			if(bLight == false)
				bLight = true;
			else
				bLight = false;
			break;

		case 70:
			toggleFullScreen(); 
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
    "uniform vec3 u_la_red;" +
    "uniform vec3 u_ld_red;" +
    "uniform vec3 u_ls_red;" +
    "uniform vec4 u_light_position_red;" +
    "uniform vec3 u_la_blue;" +
    "uniform vec3 u_ld_blue;" +
    "uniform vec3 u_ls_blue;" +
    "uniform vec4 u_light_position_blue;" +
    "uniform vec3 u_ka;" +
    "uniform vec3 u_kd;" +
    "uniform vec3 u_ks;" +
    "uniform int u_LKeyPressed;" +  
    "uniform float u_shininess;" +
    "out vec3 phong_ads_light;" +
    "void main(void)" +
    "{" +   
    "if(u_LKeyPressed == 1)" +
    "{" +
    "vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition;" +
    "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" +
    "vec3 view_vector = normalize(-eye_coordinates.xyz);" +

    "vec3 light_direction_red = normalize(vec3(u_light_position_red - eye_coordinates));" +
    "vec3 reflection_vector_red = reflect(-light_direction_red,transformed_normal);" +
    "vec3 ambient_red = u_la_red * u_ka;" +
    "vec3 diffuse_red = u_ld_red * u_kd * max(dot(light_direction_red,transformed_normal),0.0f);" +
    "vec3 specular_red = u_ls_red * u_ks * pow(max(dot(reflection_vector_red,view_vector),0.0f),u_shininess);" +

    "vec3 light_direction_blue = normalize(vec3(u_light_position_blue - eye_coordinates));" +
    "vec3 reflection_vector_blue = reflect(-light_direction_blue,transformed_normal);" +
    "vec3 ambient_blue = u_la_blue * u_ka;" +
    "vec3 diffuse_blue = u_ld_blue * u_kd * max(dot(light_direction_blue,transformed_normal),0.0f);" +
    "vec3 specular_blue = u_ls_blue * u_ks * pow(max(dot(reflection_vector_blue,view_vector),0.0f),u_shininess);" +

    "phong_ads_light = ambient_red + ambient_blue + diffuse_red + diffuse_blue + specular_red + specular_blue;" +
    "}" +
    "else" +
    "{" +
    "phong_ads_light = vec3(1.0f,1.0f,1.0f);" +
    "}" +
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
    "in vec3 phong_ads_light;" +
    "out vec4 FragColor;" +
    "void main(void)" +
    "{" +
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
	LaUniform_Red = gl.getUniformLocation(shaderProgramObject,"u_la_red");
	LdUniform_Red = gl.getUniformLocation(shaderProgramObject,"u_ld_red");
	LsUniform_Red = gl.getUniformLocation(shaderProgramObject,"u_ls_red");
	LaUniform_Blue = gl.getUniformLocation(shaderProgramObject,"u_la_blue");
	LdUniform_Blue = gl.getUniformLocation(shaderProgramObject,"u_ld_blue");
	LsUniform_Blue = gl.getUniformLocation(shaderProgramObject,"u_ls_blue");
	KaUniform = gl.getUniformLocation(shaderProgramObject,"u_ka");
	KdUnifrom = gl.getUniformLocation(shaderProgramObject,"u_kd");
	KsUniform = gl.getUniformLocation(shaderProgramObject,"u_ks");
	KshineUniform = gl.getUniformLocation(shaderProgramObject,"u_shininess");
	lightPositionUniform_Red = gl.getUniformLocation(shaderProgramObject,"u_light_position_red");
	lightPositionUniform_Blue = gl.getUniformLocation(shaderProgramObject,"u_light_position_blue");
	LKeyPressedUniform = gl.getUniformLocation(shaderProgramObject,"u_LKeyPressed");

	var pyramidVertices = new Float32Array([
												0.0,0.5,0.0,
											   -0.5,-0.5,0.5,
								                0.5,-0.5,0.5,

												0.0,0.5,0.0,
												0.5,-0.5,0.5,
												0.5,-0.5,-0.5,

												0.0,0.5,0.0,
												0.5,-0.5,-0.5,
												-0.5,-0.5,-0.5,

											    0.0,0.5,0.0,
												-0.5,-0.5,-0.5,
												-0.5,-0.5,0.5
											]);

	var pyramidNormal = new Float32Array([
												0.0,0.447214,0.894427,
												0.0,0.447214,0.894427,
												0.0,0.447214,0.894427,
												0.0,0.447214,0.894427,
    
												0.894427, 0.447214, 0.0,
												0.894427, 0.447214, 0.0,
												0.894427, 0.447214, 0.0,
												0.894427, 0.447214, 0.0,
    
												0.0, 0.447214, -0.894427,
												0.0, 0.447214, -0.894427,
												0.0, 0.447214, -0.894427,
												0.0, 0.447214, -0.894427,
    
												-0.894427, 0.447214, 0.0,
												-0.894427, 0.447214,0.0,
												-0.894427, 0.447214, 0.0,
												-0.894427, 0.447214, 0.0
										]);
/*
	var pyramidColors = new Float32Array([
												1.0,0.0,0.0,
												0.0,1.0,0.0,
												0.0,0.0,1.0,

												1.0,0.0,0.0,
												0.0,1.0,0.0,
												0.0,0.0,1.0,

												1.0,0.0,0.0,
												0.0,1.0,0.0,
												0.0,0.0,1.0,

												1.0,0.0,0.0,
												0.0,1.0,0.0,
												0.0,0.0,1.0
										  ]);

*/
  
	vao_pyramid = gl.createVertexArray();

	gl.bindVertexArray(vao_pyramid);

	vbo_Position_pyramid = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,vbo_Position_pyramid);
	gl.bufferData(gl.ARRAY_BUFFER,pyramidVertices,gl.STATIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.BDJ_ATTRIBUTE_POSITION,3,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(WebGLMacros.BDJ_ATTRIBUTE_POSITION);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	vbo_Normal_pyramid = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,vbo_Normal_pyramid);
	gl.bufferData(gl.ARRAY_BUFFER,pyramidNormal,gl.STATIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.BDJ_ATTRIBUTE_NORMAL,3,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(WebGLMacros.BDJ_ATTRIBUTE_NORMAL);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	/*
	vbo_Color_pyramid = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,vbo_Color_pyramid);
	gl.bufferData(gl.ARRAY_BUFFER,pyramidColors,gl.STATIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.BDJ_ATTRIBUTE_COLOR,3,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(WebGLMacros.BDJ_ATTRIBUTE_COLOR);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);
	*/
	gl.bindVertexArray(null);

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
	gl.clear(gl.COLOR_BUFFER_BIT);

	gl.useProgram(shaderProgramObject);

	if(bLight == true)
	{
		gl.uniform1i(LKeyPressedUniform,1);
		gl.uniform1f(KshineUniform,materialShininess);
		gl.uniform4fv(lightPositionUniform_Red,lightPosition_Red);
		gl.uniform3fv(LaUniform_Red,lightAmbient_Red);
		gl.uniform3fv(LdUniform_Red,lightDiffuse_Red);
		gl.uniform3fv(LsUniform_Red,lightSpecular_Red);
		gl.uniform4fv(lightPositionUniform_Blue,lightPosition_Blue);
		gl.uniform3fv(LaUniform_Blue,lightAmbient_Blue);
		gl.uniform3fv(LdUniform_Blue,lightDiffuse_Blue);
		gl.uniform3fv(LsUniform_Blue,lightSpecular_Blue);
		gl.uniform3fv(KaUniform,materialAmbient);
		gl.uniform3fv(KdUnifrom,materialDiffuse);
		gl.uniform3fv(KsUniform,materialSpecular);
	}
	else
	{
		gl.uniform1i(LKeyPressedUniform,0);
	}

//----------------------------------------------------------------------------------------------
	var modelMatrix = mat4.create();
	var viewMatrix = mat4.create();
	var projectionMatrix = mat4.create();
	//var translateMatrix = mat4.create();
	//var rotateMatrix = mat4.create();
	
	//translateMatrix = modelMatrix;
	//rotateMatrix = modelMatrix;

	projectionMatrix = perspectiveProjectionMatrix;

	//mat4.translate(translateMatrix,translateMatrix,[0.0,0.0,-2.0]);
	//mat4.rotateY(rotateMatrix,rotateMatrix,degToRad(angle_pyramid));

	mat4.translate(modelMatrix,modelMatrix,[0.0,0.0,-2.0]);
	mat4.rotateY(modelMatrix,modelMatrix,degToRad(angle_pyramid));

	//mat4.multiply(modelViewProjectionMatrix,perspectiveProjectionMatrix,modelViewMatrix);

	gl.uniformMatrix4fv(modelMatrixUniform,false,modelMatrix);
	gl.uniformMatrix4fv(viewMatrixUniform,false,viewMatrix);
	gl.uniformMatrix4fv(projectionMatrixUniform,false,projectionMatrix);

	gl.bindVertexArray(vao_pyramid);

	gl.drawArrays(gl.TRIANGLES,0,12);

	gl.bindVertexArray(null);
//----------------------------------------------------------------------------------------------

	angle_pyramid = angle_pyramid + 1.0;
	if(angle_pyramid >= 360.0)
	{
		angle_pyramid = 0.0;
	}

	requestAnimationFrame(draw,canvas);
}

function degToRad(degree)
{
	return(degree * Math.PI / 180.0);
}

function uninitialize()
{
	if(vao_pyramid)
	{
		gl.deleteVertexArray(vao_pyramid);
		vao_pyramid = null;
	}

	if(vbo_Position_pyramid)
	{
		gl.deleteBuffer(vbo_Position_pyramid);
		vbo_Position_pyramid = null;
	}

	if(vbo_Normal_pyramid)
	{
		gl.deleteBuffer(vbo_Normal_pyramid);
		vbo_Normal_pyramid = null;
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