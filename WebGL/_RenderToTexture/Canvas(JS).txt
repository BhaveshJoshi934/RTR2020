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
var fragmentShaderObject_1;
var fragmentShaderObject_2;
var shaderProgramObject_1;
var shaderProgramObject_2;

var Width;
var Height;

var vao;
var position_buffer;
var index_buffer;
var vbo;
var fbo;
var color_texture;
var depth_texture;
var mvUniform1;
var projUniform1;
var mvUniform2;
var projUniform2;

var Cube_angle = 0.0;

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
	"in vec2 vTexcoord;" +
	"out vec4 out_color;" +
	"out vec2 out_texcoord;" +
	"uniform mat4 mv_matrix;" +
	"uniform mat4 proj_matrix;" +
	"void main(void)" +
	"{" +
	"gl_Position = proj_matrix * mv_matrix * vPosition;" +
	"out_color = vPosition * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);" +
	"out_texcoord = vTexcoord;" +
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
//----------------------------------------------------------------------
	fragmentShaderObject_1 = gl.createShader(gl.FRAGMENT_SHADER);

	var fragmentShaderSourceCode_1 = 
	"#version 300 es"+
	"\n"+
	"precision highp float;"+
	"in vec4 out_color;"  +
	"in vec2 out_texcoord;" +
	"out vec4 FragColor;" +
	"void main(void)" +
	"{" +
	"FragColor = sin(out_color * vec4(40.0, 20.0, 30.0, 1.0)) * 0.5 + vec4(0.5);" +
	"}";

	gl.shaderSource(fragmentShaderObject_1,fragmentShaderSourceCode_1);
	gl.compileShader(fragmentShaderObject_1);
	if(gl.getShaderParameter(fragmentShaderObject_1,gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(fragmentShaderObject_1);
		if(error.length > 0)
		{
			alert(error);
			uninitialize();
		}
	}
//-------------------------------------------------------------------------------
	fragmentShaderObject_2 = gl.createShader(gl.FRAGMENT_SHADER);

	var fragmentShaderSourceCode_2 = 
	"#version 300 es"+
	"\n"+
	"precision highp float;"+
	"uniform sampler2D tex;" +
    "out vec4 FragColor;" +
    "in vec4 out_color;" +
    "in vec2 out_texcoord;" +
    "void main(void)" +
    "{" +
    "FragColor = mix(out_color, texture(tex,out_texcoord), 0.7);" +
    "}";

	gl.shaderSource(fragmentShaderObject_2,fragmentShaderSourceCode_2);
	gl.compileShader(fragmentShaderObject_2);
	if(gl.getShaderParameter(fragmentShaderObject_2,gl.COMPILE_STATUS) == false)
	{
		var error = gl.getShaderInfoLog(fragmentShaderObject_2);
		if(error.length > 0)
		{
			alert(error);
			uninitialize();
		}
	}
//-------------------------------------------------------------------------------
	shaderProgramObject_1 = gl.createProgram();

	gl.attachShader(shaderProgramObject_1,vertexShaderObject);
	gl.attachShader(shaderProgramObject_1,fragmentShaderObject_1);

	gl.bindAttribLocation(shaderProgramObject_1,WebGLMacros.BDJ_ATTRIBUTE_POSITION,"vPosition");
	gl.bindAttribLocation(shaderProgramObject_1,WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0,"vTexcoord");

	gl.linkProgram(shaderProgramObject_1);

	if(gl.getProgramParameter(shaderProgramObject_1,gl.LINK_STATUS) == false)
	{
		var error = gl.getProgramInfoLog(shaderProgramObject_1);
		if(error.length > 0)
		{
			alert(error);
			uninitialize();
		}
	}

	mvUniform1 = gl.getUniformLocation(shaderProgramObject_1,"mv_matrix");
	projUniform1 = gl.getUniformLocation(shaderProgramObject_1,"proj_matrix");

//-------------------------------------------------------------------------------
	shaderProgramObject_2 = gl.createProgram();

	gl.attachShader(shaderProgramObject_2,vertexShaderObject);
	gl.attachShader(shaderProgramObject_2,fragmentShaderObject_2);

	gl.bindAttribLocation(shaderProgramObject_2,WebGLMacros.BDJ_ATTRIBUTE_POSITION,"vPosition");
	gl.bindAttribLocation(shaderProgramObject_2,WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0,"vTexcoord");

	gl.linkProgram(shaderProgramObject_2);

	if(gl.getProgramParameter(shaderProgramObject_2,gl.LINK_STATUS) == false)
	{
		var error = gl.getProgramInfoLog(shaderProgramObject_2);
		if(error.length > 0)
		{
			alert(error);
			uninitialize();
		}
	}

	mvUniform2 = gl.getUniformLocation(shaderProgramObject_2,"mv_matrix");
	projUniform2 = gl.getUniformLocation(shaderProgramObject_2,"proj_matrix");

//-------------------------------------------------------------------------------
	vao = gl.createVertexArray();
	gl.bindVertexArray(vao);

	 var vertex_indices = new Float32Array
    ([
            0, 1, 2,
            2, 1, 3,
            2, 3, 4,
            4, 3, 5,
            4, 5, 6,
            6, 5, 7,
            6, 7, 0,
            0, 7, 1,
            6, 0, 2,
            2, 4, 6,
            7, 5, 3,
            7, 3, 1
    ]);

    var vertex_cube = new Float32Array
    ([
              // Position              
            -0.5, -0.5,  0.5,      
            -0.5, -0.5, -0.5,      
             0.5, -0.5, -0.5,      

             0.5, -0.5, -0.5,    
             0.5, -0.5,  0.5,      
            -0.5, -0.5,  0.5,      

             0.5, -0.5, -0.5,      
             0.5,  0.5, -0.5,      
             0.5, -0.5,  0.5,      

             0.5,  0.5, -0.5,      
             0.5,  0.5,  0.5,      
             0.5, -0.5,  0.5,      

             0.5,  0.5, -0.5,      
            -0.5,  0.5, -0.5,      
             0.5,  0.5,  0.5,      

            -0.5,  0.5, -0.5,      
            -0.5,  0.5,  0.5,      
             0.5,  0.5,  0.5,      

            -0.5,  0.5, -0.5,     
            -0.5, -0.5, -0.5,      
            -0.5,  0.5,  0.5,      

            -0.5, -0.5, -0.5,      
            -0.5, -0.5,  0.5,      
            -0.5,  0.5,  0.5,     

            -0.5,  0.5, -0.5,      
             0.5,  0.5, -0.5,      
             0.5, -0.5, -0.5,      

             0.5, -0.5, -0.5,     
            -0.5, -0.5, -0.5,      
            -0.5,  0.5, -0.5,      

            -0.5, -0.5,  0.5,      
             0.5, -0.5,  0.5,      
             0.5,  0.5,  0.5,     

             0.5,  0.5,  0.5,     
            -0.5,  0.5,  0.5,    
            -0.5, -0.5,  0.5,     

    ]);

	var vertex_texcoord = new Float32Array
    ([
			0.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,

			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,

			0.0, 0.0,
			1.0, 0.0,
			0.0, 1.0,

			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,

			1.0, 0.0,
			0.0, 0.0,
			1.0, 1.0,

			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,

			1.0, 0.0,
            0.0, 0.0,
            1.0, 1.0,

            0.0, 0.0,
            0.0, 1.0,
            1.0, 1.0,

            0.0, 1.0,
            1.0, 1.0,
            1.0, 0.0,

            1.0, 0.0,
            0.0, 0.0,
            0.0, 1.0,

            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,

            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
	]);

	position_buffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,position_buffer);
	gl.bufferData(gl.ARRAY_BUFFER,vertex_cube,gl.STATIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.BDJ_ATTRIBUTE_POSITION,3,gl.FLOAT,false,0,0); 
	gl.enableVertexAttribArray(WebGLMacros.BDJ_ATTRIBUTE_POSITION);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	texture_buffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,texture_buffer);
	gl.bufferData(gl.ARRAY_BUFFER,vertex_texcoord,gl.STATIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0,2,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(WebGLMacros.BDJ_ATTRIBUTE_TEXTURE0);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	index_buffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,index_buffer);
	gl.bufferData(gl.ARRAY_BUFFER,vertex_indices,gl.STATIC_DRAW);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	gl.bindVertexArray(null);

	fbo = gl.createBuffer();

	fbo = gl.createFramebuffer();
	gl.bindFramebuffer(gl.FRAMEBUFFER,fbo);

	color_texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D,color_texture);
	gl.texStorage2D(gl.TEXTURE_2D,9,gl.RGBA8,512,512);

	gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_MIN_FILTER,gl.LINEAR);
	gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_MAG_FILTER,gl.LINEAR);

	depth_texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D,depth_texture);
	gl.texStorage2D(gl.TEXTURE_2D,9,gl.DEPTH_COMPONENT32F,512,512);

	gl.framebufferTexture2D(gl.FRAMEBUFFER,gl.COLOR_ATTACHMENT0,gl.TEXTURE_2D,color_texture,0);
	gl.framebufferTexture2D(gl.FRAMEBUFFER,gl.DEPTH_ATTACHMENT,gl.TEXTURE_2D,depth_texture,0);

	//gl.enable(gl.TEXTURE_2D);

	var draw_buffers = [gl.COLOR_ATTACHMENT0];

	gl.drawBuffers(draw_buffers);

	gl.bindFramebuffer(gl.FRAMEBUFFER,null);

	gl.clearDepth(1.0);
	gl.enable(gl.DEPTH_TEST);
	gl.depthFunc(gl.LEQUAL);

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

	var green = new Float32Array([0.0, 0.3, 0.0, 1.0]);
    var blue  = new Float32Array([0.0, 0.0, 0.3, 1.0]);
    var one   = new Float32Array([1.0]);

	var proj_matrix = mat4.create();
	var mv_matrix   = mat4.create();
	
	proj_matrix = perspectiveProjectionMatrix;

	mat4.translate(mv_matrix,mv_matrix,[0.0,0.0,-3.0]);  
	mat4.rotateX(mv_matrix,mv_matrix,degToRad(Cube_angle));
	mat4.rotateY(mv_matrix,mv_matrix,degToRad(Cube_angle));
	mat4.rotateZ(mv_matrix,mv_matrix,degToRad(Cube_angle));

	gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);

    gl.viewport(0, 0, 512,512);
    gl.clearBufferfv(gl.COLOR, 0, green);
    gl.clearBufferfi(gl.DEPTH_STENCIL, 0, 1.0, 0);

    gl.useProgram(shaderProgramObject_1);

    gl.uniformMatrix4fv(mvUniform1,false, mv_matrix);
    gl.uniformMatrix4fv(projUniform1,false,proj_matrix);

    gl.bindVertexArray(vao);

    gl.drawArrays(gl.TRIANGLES, 0, 36);

    gl.bindVertexArray(null);

    gl.bindFramebuffer(gl.FRAMEBUFFER, null);

    gl.useProgram(null);

    gl.viewport(0, 0,canvas.width, canvas.height);

    gl.clearBufferfv(gl.COLOR, 0, blue);
    gl.clearBufferfv(gl.DEPTH, 0, one);

    gl.bindTexture(gl.TEXTURE_2D, color_texture);

    gl.useProgram(shaderProgramObject_2);

    gl.uniformMatrix4fv(mvUniform2,false, mv_matrix);
    gl.uniformMatrix4fv(projUniform2,false, proj_matrix);

    gl.bindVertexArray(vao);

    gl.drawArrays(gl.TRIANGLES, 0, 36);

    gl.bindVertexArray(null);

    gl.bindTexture(gl.TEXTURE_2D, null);

    gl.useProgram(null);

    Cube_angle = Cube_angle + 1.0;
    if(Cube_angle >= 360.0)
    {
         Cube_angle = 0.0;
    }


	requestAnimationFrame(draw,canvas);
}

function degToRad(angle)
{
	return(angle * Math.PI / 180.0);
}

function uninitialize()
{
	if(vao)
	{
		gl.deleteVertexArray(vao);
		vao = null;
	}

	if(position_buffer)
	{
		gl.deleteBuffer(position_buffer);
		position_buffer = null;
	}

	if(index_buffer)
	{
		gl.deleteBuffer(index_buffer);
		index_buffer = null;
	}

	if(fbo)
	{
		gl.deleteFramebuffer(fbo);
		fbo = null;
	}
	
	if(shaderProgramObject_1)
	{
		if(fragmentShaderObject_1)
		{
			gl.detachShader(shaderProgramObject_1,fragmentShaderObject_1);
			gl.deleteShader(fragmentShaderObject_1);
			fragmentShaderObject_1 = null;
		}

		if(vertexShaderObject)
		{
			gl.detachShader(shaderProgramObject_1,vertexShaderObject);
			gl.deleteShader(vertexShaderObject);
			vertexShaderObject = null;
		}

		gl.deleteProgram(shaderProgramObject_1);
		shaderProgramObject_1 = null;
	}

	if(shaderProgramObject_2)
	{
		if(fragmentShaderObject_2)
		{
			gl.detachShader(shaderProgramObject_2,fragmentShaderObject_2);
			gl.deleteShader(fragmentShaderObject_2);
			fragmentShaderObject_2 = null;
		}

		if(vertexShaderObject)
		{
			gl.detachShader(shaderProgramObject_2,vertexShaderObject);
			gl.deleteShader(vertexShaderObject);
			vertexShaderObject = null;
		}

		gl.deleteProgram(shaderProgramObject_2);
		shaderProgramObject_2 = null;
	}
	
}