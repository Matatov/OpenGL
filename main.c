//lab06.c
#include<stdio.h>
#include<math.h>
#include"GL/glut.h"

//menu constants
#define MENU_1  10
#define MENU_2  20

//function declerations
void drawingCB(void);
void reshapeCB(int width, int height);
void menuCB(int value);
void keyboardCB(unsigned char key, int x, int y);



int main(int argc, char* argv[])
{
	//initizlizing GLUT
	glutInit(&argc, argv);

	//initializing window
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGB);
	glutCreateWindow("3D world");

	//registering callbacks
	glutDisplayFunc(drawingCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);

	//registering and creating menu
	glutCreateMenu(menuCB);
	glutAddMenuEntry("menu 1", MENU_1);
	glutAddMenuEntry("menu 2", MENU_2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//starting main loop
	glutMainLoop();
}


void drawingCB(void)
{
	GLenum er;

	//clearing the background
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//initializing modelview transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

	glBegin(GL_LINE_STRIP);
	glVertex2f(-1, 0);
	glVertex2f(0, 1);
	glVertex2f(1, 0);
	glEnd();

	glColor3f(1, 1, 1);

	glutWireTeapot(2);
	//forces execution of OpenGL functions in finite time
	glFlush();

	//check for errors
	er = glGetError();  //get errors. 0 for no error, find the error codes in: https://www.opengl.org/wiki/OpenGL_Error
	if (er) printf("error: %d\n", er);
}



void reshapeCB(int width, int height)
{
	float left, right, bottom, top;
	float AR;

	//define our ortho
	left = -3;
	right = 3;
	bottom = -3;
	top = 3;

	//update viewport
	glViewport(0, 0, width, height);

	//clear the transformation matrices (load identity)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	
		//compute the aspect ratio
		AR = (float)width/height;

		//if AR>=1 update left, right
		if(AR>1)
		{
			left=left*AR;
			right=right*AR;
		}
		//else i.e. AR<1 update the top, bottom
		else
		{
			top=top/AR;
			bottom=bottom/AR;
		}
	

	//projection or gluOrtho2D
	glOrtho(left, right, bottom, top, 1, 10);
	
}






void keyboardCB(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'a':
		break;
	}
}



void menuCB(int value)
{
	switch (value) {
	case MENU_1:
		break;
	case MENU_2:
		break;
	}
}