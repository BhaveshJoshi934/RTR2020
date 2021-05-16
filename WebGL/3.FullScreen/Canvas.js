// JavaScript source code

var canvas = null;
var context = null;

function main()
{
	//1. Get Canvas From DOM(Document Object Model) => html Document

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

	//2. Retrive Width and Height of Canvas

	console.log("canvaswidth = "+canvas.width +" canvasHeight = "+canvas.height +"/n");

	//3. Get DrawingContext 

	context = canvas.getContext("2d");

	if(!canvas)
	{
		console.log("Obataining Context Failed/n");
	}
	else
	{
		console.log("Obataining Context Succeded/n");
	}

	context.fillStyle="black";  //Setter Function(= chya ujvya bajula) OR Mutator Function  and   Getter Function(= chya davya bajula) OR 
	context.fillRect(0,0,canvas.width,canvas.height);

	drawText("HELLO WORLD!!!");

	//"window" is inbuilt variable like document used above...
	//window is DOM object as it is inherited from document...

	window.addEventListener("keydown", keyDown, false);  // 1st param : Inbuilt event ; 2nd param : my function ; 3rd param : following bubble propagation not capture propagation
	window.addEventListener("click", mouseDown, false);  // 1st param : Inbuilt event ; 2nd param : my function ; 3rd param : following bubble propagation not capture propagation 

}

function drawText(text)
{
	context.textAlign="center"; // horizontal center
	context.textBaseline="middle"; // vertical center
	context.font="48px sans-serif";
	context.fillStyle="white";
	context.fillText(text,canvas.width / 2, canvas.height / 2 );
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
	}
							
}

function keyDown(event) // Type Inference : type runtime la tharel
{
	switch(event.keyCode)
	{
		case 70:
			toggleFullScreen();
			drawText("HELLO WORLD!!!");  // As there is No repaint event in javascript 
			break;
	}
}

function mouseDown(event)
{
}