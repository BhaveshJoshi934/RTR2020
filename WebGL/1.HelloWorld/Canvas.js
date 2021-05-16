// JavaScript source code
function main()
{
	//1. Get Canvas From DOM(Document Object Model) => html Document

	var canvas = document.getElementById("BDJ");  //Type Inference : Variable chya ujvya hatala kay yenar ahe he guess karan...Eg. Auto in C
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

	var context = canvas.getContext("2d");

	if(!canvas)
	{
		console.log("Obataining Context Failed/n");
	}
	else
	{
		console.log("Obataining Context Succeded/n");
	}

	//4. Paint Background by black color

	context.fillStyle="black";  //Setter Function(= chya ujvya bajula) OR Mutator Function  and   Getter Function(= chya davya bajula) OR 
	context.fillRect(0,0,canvas.width,canvas.height);

	//5. Center the Future coming text

	context.textAlign="center"; // horizontal center

	context.textBaseline="middle"; // vertical center

	//6. 

	context.font="48px sans-serif";

	//7. 

	var str = "Hello World !!!";

	//8.

	context.fillStyle="white";
	
	//9. Display text
	context.fillText( str,canvas.width / 2, canvas.height / 2 );

}