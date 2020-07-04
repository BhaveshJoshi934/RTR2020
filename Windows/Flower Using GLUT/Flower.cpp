#include<GL\freeglut.h>

bool bFullScreen = false;

int main(int argc, char** argv)
{
	//code
	//Function Declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLUT: Bhavesh Joshi !!");

	initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return(0);
}

void initialize(void)
{
	//code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height)
{
	//code
	if (height <= 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void display(void)
{
	//code

	int ref_x = 0.0f;
	int ref_y = 0.0f;

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_POLYGON);
	//Ulat
	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x + 0.125f, ref_y - 0.4f, 0.0f);                    //1

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(ref_x + 0.0f, ref_y + 0.0f, 0.0f);                     //2

	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x - 0.125f, ref_y - 0.4f, 0.0f);                    //3 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x - 0.0625f, ref_y - 0.60f, 0.0f);                   //4

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x + 0.0625f, ref_y - 0.60f, 0.0f);                  //5

	glEnd();


	glBegin(GL_POLYGON);
	//saral
	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x - 0.125f, ref_y + 0.4f, 0.0f);                    //1

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(ref_x + 0.0f, ref_y + 0.0f, 0.0f);                     //2

	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x + 0.125f, ref_y + 0.4f, 0.0f);                    //3 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x + 0.0625f, ref_y + 0.60f, 0.0f);                   //4

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x - 0.0625f, ref_y + 0.60f, 0.0f);                  //5

	glEnd();

	glBegin(GL_POLYGON);
	//On +ve X a
	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x + 0.4f, ref_y - 0.125f, 0.0f);                    //1

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(ref_x + 0.0f, ref_y + 0.0f, 0.0f);                     //2

	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x + 0.4f, ref_y + 0.125f, 0.0f);                    //3 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x + 0.60f, ref_y + 0.0625f, 0.0f);                   //4

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x + 0.60f, ref_y - 0.0625f, 0.0f);                  //5

	glEnd();



	glBegin(GL_POLYGON);
	//On -ve X a
	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x - 0.4f, ref_y - 0.125f, 0.0f);                    //1

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(ref_x + 0.0f, ref_y + 0.0f, 0.0f);                     //2

	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x - 0.4f, ref_y + 0.125f, 0.0f);                    //3 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x - 0.60f, ref_y + 0.0625f, 0.0f);                   //4

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x - 0.60f, ref_y - 0.0625f, 0.0f);                  //5

	glEnd();

	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	//On +ve X a
	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x + 0.4f, ref_y - 0.125f, 0.0f);                    //1

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(ref_x + 0.0f, ref_y + 0.0f, 0.0f);                     //2

	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x + 0.4f, ref_y + 0.125f, 0.0f);                    //3 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x + 0.60f, ref_y + 0.0625f, 0.0f);                   //4

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x + 0.60f, ref_y - 0.0625f, 0.0f);                  //5

	glEnd();

	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	//On -ve X a
	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x - 0.4f, ref_y - 0.125f, 0.0f);                    //1

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(ref_x + 0.0f, ref_y + 0.0f, 0.0f);                     //2

	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x - 0.4f, ref_y + 0.125f, 0.0f);                    //3 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x - 0.60f, ref_y + 0.0625f, 0.0f);                   //4

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x - 0.60f, ref_y - 0.0625f, 0.0f);                  //5

	glEnd();


	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	//On +ve X a
	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x + 0.4f, ref_y - 0.125f, 0.0f);                    //1

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(ref_x + 0.0f, ref_y + 0.0f, 0.0f);                     //2

	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x + 0.4f, ref_y + 0.125f, 0.0f);                    //3 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x + 0.60f, ref_y + 0.0625f, 0.0f);                   //4

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x + 0.60f, ref_y - 0.0625f, 0.0f);                  //5

	glEnd();


	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	//On -ve X a
	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x - 0.4f, ref_y - 0.125f, 0.0f);                    //1

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(ref_x + 0.0f, ref_y + 0.0f, 0.0f);                     //2

	glColor3f(0.40f, 0.0f, 0.40f);
	glVertex3f(ref_x - 0.4f, ref_y + 0.125f, 0.0f);                    //3 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x - 0.60f, ref_y + 0.0625f, 0.0f);                   //4

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(ref_x - 0.60f, ref_y - 0.0625f, 0.0f);                  //5

	glEnd();


	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	//code
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;

	case 'F':
	case 'f':
		if (bFullScreen == false)
		{
			glutFullScreen();
			bFullScreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bFullScreen = false;
		}
		break;

	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	//code
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		break;

	case GLUT_RIGHT_BUTTON:
		glutLeaveMainLoop();
		break;

	default:
		break;
	}
}

void uninitialize(void)
{
	//code
}
