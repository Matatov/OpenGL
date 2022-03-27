//lab05.c
#include<time.h>
#include<stdio.h>
#include<math.h>
#include"GL/glut.h" 
#define Pi 3.141592654


//vertex array
typedef struct POINT {
	GLfloat x;
	GLfloat y;
} Point;

#define POINTS_VERTEX_NUM 12 
#define CIRCLE_VERTEX_NUM 72 

Point points_vertexes[POINTS_VERTEX_NUM];
Point circle_vertexes[CIRCLE_VERTEX_NUM];


//function declerations
void Deg2Vector(float VecDeg, float VecSize, Point* p);
float Deg2Rad(float deg);

void drawingCB(void);
void reshapeCB(int width, int height);
void menuCB(int value);
void keyboardCB(int key, int x, int y);
//globals
int hours, minutes;


int main(int argc, char* argv[])
{
	//initializing points and circle array
	int n;
	for (n = 0; n < POINTS_VERTEX_NUM; n++) {
		Deg2Vector((n + 1) * 360 / POINTS_VERTEX_NUM, 0.7, &points_vertexes[n]);
	}
	for (n = 0; n < CIRCLE_VERTEX_NUM; n++) {
		Deg2Vector((n + 1) * 360 / CIRCLE_VERTEX_NUM, 0.9, &circle_vertexes[n]);
	}

	time_t t;
	struct tm* timeinfo;
	time(&t);
	timeinfo = localtime(&t);
	hours = timeinfo->tm_hour;
	minutes = timeinfo->tm_min;

	//hours = 3;
	//minutes = 25;

	//initizlizing GLUT
	glutInit(&argc, argv);

	//initializing window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutInitDisplayMode(GLUT_RGB);
	glutCreateWindow("Clock");

	//registering callbacks
	glutDisplayFunc(drawingCB);
	glutReshapeFunc(reshapeCB);
	glutSpecialFunc(keyboardCB);
	glutCreateMenu(menuCB);
	glutAddMenuEntry("Reset", 25);
	glutAddMenuEntry("Exit", 30);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//starting main loop
	glutMainLoop();
}

// create vector from angle and size of vector
void Deg2Vector(float VecDeg, float VecSize, Point* p)
{
	p->x = (GLfloat)cos(Deg2Rad(VecDeg)) * VecSize;
	p->y = (GLfloat)sin(Deg2Rad(VecDeg)) * VecSize;
}

// convert degree to radians
float Deg2Rad(float deg)
{
	return deg * Pi / 180.0;
}


void DrawClock(int hours, int minutes)
{
	int n;
	float HourDeg;
	float MinutesDeg;

	HourDeg = hours * 360 / 12;
	MinutesDeg = minutes * 360 / 60;

	//drawing points
	glColor3f(0.1f, 0.5f, 0.1f);
	glPointSize(4);
	glBegin(GL_POINTS);
	for (n = 0; n < POINTS_VERTEX_NUM; n++) {
		glVertex2f(points_vertexes[n].x, points_vertexes[n].y);
	}
	glEnd();

	//drawing circle
	glLineWidth(2);
	glColor3f(0.1f, 0.4f, 0.8f);
	glBegin(GL_LINE_LOOP);
	for (n = 0; n < CIRCLE_VERTEX_NUM; n++) {
		glVertex2f(circle_vertexes[n].x, circle_vertexes[n].y);
	}
	glEnd();

	//drawing middle point
	glColor3f(0.1f, 0.5f, 0.1f);
	glPointSize(6);
	glBegin(GL_POINTS);
	glVertex2f(0, 0);
	glEnd();

	//drawing clock lines
	glColor3f(0.0f, 0.0f, 0.8f);

	glPushMatrix();
	glRotatef(-HourDeg, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, 0.4);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-MinutesDeg, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, 0.6);
	glEnd();
	glPopMatrix();
}

// rendering callback
void drawingCB()
{
	GLenum er;
	GLfloat ModelviewMatrix[16];
	GLfloat ProjectionMatrix[16];

	//clearing the background
	glClearColor(0.2f, 0.8f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//initializing modelview transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//drawing clock 1
	glPushMatrix();
	glTranslated(1.5, 1.5, 0);
	DrawClock(hours, minutes);
	glPopMatrix();

	//drawing clock 2
	glPushMatrix();
	glTranslated(-1.5, 1.5, 0);
	DrawClock(hours + 1, minutes + 10);
	glPopMatrix();

	//drawing clock 3
	glPushMatrix();
	glTranslated(-1.5, -1.5, 0);
	DrawClock(hours + 3, minutes + 30);
	glPopMatrix();

	//drawing clock 4
	glPushMatrix();
	glTranslated(1.5, -1.5, 0);
	DrawClock(hours + 2, minutes + 20);
	glPopMatrix();

	//checking transformation matrixes for debuging
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelviewMatrix);
	glGetFloatv(GL_PROJECTION_MATRIX, ProjectionMatrix);

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
	GLfloat ModelviewMatrix[16];
	GLfloat ProjectionMatrix[16];

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
	AR = (float)width / height;

	//if AR>=1 update left, right
	if (AR > 1)
	{
		left = left * AR;
		right = right * AR;
	}
	//else i.e. AR<1 update the top, bottom
	else
	{
		top = top / AR;
		bottom = bottom / AR;
	}

	//projection or gluOrtho2D
	gluOrtho2D(left, right, bottom, top);


	//checking transformation matrixes for debuging
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelviewMatrix);
	glGetFloatv(GL_PROJECTION_MATRIX, ProjectionMatrix);
}


void menuCB(int value)
{

	switch (value)
	{
	case 25:
		hours = 12;
		minutes = 0;
		glutPostRedisplay();
		break;
	case 30:
		exit(0);
	}
}


void keyboardCB(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		minutes = --minutes % 60;
		break;
	case GLUT_KEY_UP:
		minutes = ++minutes % 60;
		break;
	case GLUT_KEY_LEFT:
		hours = ++hours % 12;
		break;
	case GLUT_KEY_RIGHT:
		hours = --hours % 12;
		break;
	}
	glutPostRedisplay();
}