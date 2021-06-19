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

var TexCoord = new Float32Array (8);
var cube_texture;

var vao_cube;
var vbo_Position_cube;
var vbo_texture_cube;

var mvpUniform;
var KeyPressed;
var textureSamplerUniform;

var perspeciveProjectionMatrix;  // ADHI O/p dakhvto....

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

		case 49:
			KeyPressed = 1;
			gl.enable(gl.TEXTURE_2D);
			break;

		case 50:
			KeyPressed = 2;
			gl.enable(gl.TEXTURE_2D);
			break;

		case 51:
			KeyPressed = 3;
			gl.enable(gl.TEXTURE_2D);
			break;

		case 52:
			KeyPressed = 4;
			gl.enable(gl.TEXTURE_2D);
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
	"in vec4 vPosition;"+
	"in vec2 vTexCoord;"+
	"uniform mat4 u_mvp_matrix;"+
	"out vec2 out_TexCoord;"+
	"void main(void)"+
	"{"+
	"gl_Position = u_mvp_matrix * vPosition;"+
	"out_TexCoord = vTexCoord;"+
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
	"in vec2 out_TexCoord;"+
	"uniform sampler2D u_texture_sampler;"+
	"out vec4 FragColor;"+
	"void main(void)"+
	"{"+
	"FragColor = texture(u_texture_sampler,out_TexCoord);"+
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
	gl.bindAttribLocation(shaderProgramObject,WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0,"vTexCoord");

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

	mvpUniform = gl.getUniformLocation(shaderProgramObject,"u_mvp_matrix");
	textureSamplerUniform = gl.getUniformLocation(shaderProgramObject,"u_texture_sampler");

    var cubeVertices = new Float32Array([
                0.5,0.5,0.5,
                -0.5,0.5,0.5,
                -0.5,-0.5,0.5,
                0.5,-0.5,0.5
            ]);
/*
	var cubeTexCoord = new Float32Array 
            ([
                                //RONT ACE
                0.0, 0.0,
                1.0, 0.0,
                1.0,1.0,
                0.0,1.0
            ]);
*/
	vao_cube = gl.createVertexArray();

	gl.bindVertexArray(vao_cube);

	vbo_Position_cube = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,vbo_Position_cube);
	gl.bufferData(gl.ARRAY_BUFFER,cubeVertices,gl.STATIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.BDJ_ATTRIBUTE_POSITION,3,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(WebGLMacros.BDJ_ATTRIBUTE_POSITION);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	vbo_texture_cube = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,vbo_texture_cube);
	gl.bufferData(gl.ARRAY_BUFFER,0,gl.DYNAMIC_DRAW); 
	gl.vertexAttribPointer(WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0,2,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);
	
	gl.bindVertexArray(null);
	
	gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

	cube_texture = gl.createTexture();
	cube_texture.image = new Image();
	cube_texture.image.src = "smiley_512x512.png";
	cube_texture.image.onload = function(){
												gl.bindTexture(gl.TEXTURE_2D,cube_texture);
												gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL,1);
												gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_MAG_FILTER,gl.NEAREST);
												gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_MIN_FILTER,gl.NEAREST);
												gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,cube_texture.image);
												gl.bindTexture(gl.TEXTURE_2D,null);
										  };  


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

	var modelViewMatrix = mat4.create();
	var modelViewProjectionMatrix = mat4.create();
	var translateMatrix = mat4.create();
	
	mat4.translate(modelViewMatrix,modelViewMatrix,[0.0,0.0,-3.0]);

	mat4.multiply(modelViewProjectionMatrix,perspectiveProjectionMatrix,modelViewMatrix);

	gl.uniformMatrix4fv(mvpUniform,false,modelViewProjectionMatrix); 

	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D,cube_texture);
	gl.uniform1i(textureSamplerUniform,0);

	if(KeyPressed == 1)
	{
		TexCoord[0] = 1.0;
        TexCoord[1] = 1.0;
        TexCoord[2] = 0.0;
        TexCoord[3] = 1.0;
        TexCoord[4] = 0.0;
        TexCoord[5] = 0.0;
        TexCoord[6] = 1.0;
        TexCoord[7] = 0.0;
	}

	else if(KeyPressed == 2)
	{
		TexCoord[0] = 0.5;
        TexCoord[1] = 0.5;
        TexCoord[2] = 0.0;
        TexCoord[3] = 0.5;
        TexCoord[4] = 0.0;
        TexCoord[5] = 0.0;
        TexCoord[6] = 0.5;
        TexCoord[7] = 0.0;
	}

	else if(KeyPressed == 3)
	{
		TexCoord[0] = 2.0;
        TexCoord[1] = 2.0; 
        TexCoord[2] = 0.0;
        TexCoord[3] = 2.0;
        TexCoord[4] = 0.0;
        TexCoord[5] = 0.0;
        TexCoord[6] = 2.0;
        TexCoord[7] = 0.0;
	}

	else if(KeyPressed == 4)
	{
		TexCoord[0] = 0.5;
        TexCoord[1] = 0.5;
        TexCoord[2] = 0.5;
        TexCoord[3] = 0.5;
        TexCoord[4] = 0.5;
        TexCoord[5] = 0.5;
        TexCoord[6] = 0.5;
        TexCoord[7] = 0.5;
	}

	gl.bindBuffer(gl.ARRAY_BUFFER,vbo_texture_cube);
	gl.bufferData(gl.ARRAY_BUFFER,TexCoord,gl.DYNAMIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0,2,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	gl.bindVertexArray(vao_cube);

	gl.drawArrays(gl.TRIANGLE_FAN,0,4);  

	gl.bindVertexArray(null);

	gl.useProgram(null);

	requestAnimationFrame(draw,canvas);
}

function degToRad(degree)
{
	return(degree * Math.PI / 180.0);
}

function uninitialize()
{
	if(vao_cube)
	{
		gl.deleteVertexArray(vao_cube);
		vao_cube= null;
	}

	if(vbo_Position_cube)
	{
		gl.deleteBuffer(vbo_Position_cube);
		vbo_Position_cube = null;
	}

	if(vbo_texture_cube)
	{
		gl.deleteBuffer(vbo_texture_cube);
		vbo_texture_cube = null;
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