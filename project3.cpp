//---------------------------------------
// Program: surface3.cpp
// Purpose: Use Phong shading to display
//          quadratic surface model.
// Author:  John Gauch
// Date:    October 2008
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Transformation variables 
#define ROTATE 1
#define TRANSLATE 2
int xangle = 0;
int yangle = 0;
int zangle = 0;
int xpos = 0;
int ypos = 0;
int zpos = 0;
int mode = ROTATE;

// Surface variables
#define SIZE 32
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];
float Nx[SIZE + 1][SIZE + 1];
float Ny[SIZE + 1][SIZE + 1];
float Nz[SIZE + 1][SIZE + 1];
#define STEP 0.1

#include "shading.cpp"

//---------------------------------------
// Initialize surface 
//---------------------------------------
void init_surface(float Xmin, float Xmax, float Ymin, float Ymax,
	float Wxx, float Wxy, float Wyy, float Wx, float Wy, float W1)
{
	// Initialize surface
	int i, j;
	for (i = 0; i <= SIZE; i++)
		for (j = 0; j <= SIZE; j++)
		{
			// Calculate point
			Px[i][j] = Xmin + i * (Xmax - Xmin) / SIZE;
			Py[i][j] = Ymin + j * (Ymax - Ymin) / SIZE;
			Pz[i][j] = Wxx * Px[i][j] * Px[i][j]
				+ Wxy * Px[i][j] * Py[i][j]
				+ Wyy * Py[i][j] * Py[i][j]
				+ Wx * Px[i][j]
				+ Wy * Py[i][j] + W1;

			// Calculate unit length normal
			Nx[i][j] = -(2 * Wxx * Px[i][j] + Wxy * Py[i][j] + Wx);
			Ny[i][j] = -(2 * Wyy * Py[i][j] + Wxy * Px[i][j] + Wy);
			Nz[i][j] = 1;
			float length = sqrt(Nx[i][j] * Nx[i][j]
				+ Ny[i][j] * Ny[i][j]
				+ Nz[i][j] * Nz[i][j]);
			if (length > 0)
			{
				Nx[i][j] /= length;
				Ny[i][j] /= length;
				Nz[i][j] /= length;
			}
		}
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
	// Initialize OpenGL
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float radius = 2;
	glOrtho(-radius, radius, -radius, radius, -radius, radius);
	glEnable(GL_DEPTH_TEST);

	// Initialize smooth shading
	glShadeModel(GL_SMOOTH);
	init_light(GL_LIGHT0, 1, 1, 1, 1, 1, 1);

	// Initialize surface
	init_surface(-1.0, 1.0, -1.0, 1.0, -1, 0, -1, 0, 0, 0);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
	// Incrementally rotate objects
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);
	glRotatef(zangle, 0.0, 0.0, 1.0);

	// Initialize material properties
	init_material(Ka, Kd, Ks, 100 * Kp, 0.6, 0.4, 0.8);

	// Draw the surface
	int i, j;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
		{
			glBegin(GL_POLYGON);
			glNormal3f(Nx[i][j], Ny[i][j], Nz[i][j]);
			glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
			glNormal3f(Nx[i + 1][j], Ny[i + 1][j], Nz[i + 1][j]);
			glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
			glNormal3f(Nx[i + 1][j + 1], Ny[i + 1][j + 1], Nz[i + 1][j + 1]);
			glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
			glNormal3f(Nx[i][j + 1], Ny[i][j + 1], Nz[i][j + 1]);
			glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
			glEnd();
		}
	glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
	// Determine if we are in ROTATE or TRANSLATE mode
	if ((key == 'r') || (key == 'R'))
	{
		printf("Type x y z to decrease or X Y Z to increase ROTATION angles.\n");
		mode = ROTATE;
	}
	else if ((key == 't') || (key == 'T'))
	{
		printf
		("Type x y z to decrease or X Y Z to increase TRANSLATION distance.\n");
		mode = TRANSLATE;
	}

	// Handle ROTATE
	if (mode == ROTATE)
	{
		if (key == 'x')
			xangle -= 5;
		else if (key == 'y')
			yangle -= 5;
		else if (key == 'z')
			zangle -= 5;
		else if (key == 'X')
			xangle += 5;
		else if (key == 'Y')
			yangle += 5;
		else if (key == 'Z')
			zangle += 5;
	}

	// Handle TRANSLATE
	if (mode == TRANSLATE)
	{
		if (key == 'x')
			xpos -= 10;
		else if (key == 'y')
			ypos -= 10;
		else if (key == 'z')
			zpos -= 10;
		else if (key == 'X')
			xpos += 10;
		else if (key == 'Y')
			ypos += 10;
		else if (key == 'Z')
			zpos += 10;
	}

	// Handle material properties
	if (key == 'a')
		Ka -= STEP;
	if (key == 'd')
		Kd -= STEP;
	if (key == 's')
		Ks -= STEP;
	if (key == 'p')
		Kp -= STEP;
	if (key == 'A')
		Ka += STEP;
	if (key == 'D')
		Kd += STEP;
	if (key == 'S')
		Ks += STEP;
	if (key == 'P')
		Kp += STEP;
	if (Ka < 0)
		Ka = 0;
	if (Kd < 0)
		Kd = 0;
	if (Ks < 0)
		Ks = 0;
	if (Kp < STEP)
		Kp = STEP;
	glutPostRedisplay();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
	// Handle mouse down
	static int xdown, ydown;
	if (state == GLUT_DOWN)
	{
		xdown = x;
		ydown = y;
	}

	// Handle ROTATE
	if ((mode == ROTATE) && (state == GLUT_UP))
	{
		xangle += (y - ydown);
		yangle -= (x - xdown);
		zangle = 0;
		glutPostRedisplay();
	}

	// Handle TRANSLATE
	if ((mode == TRANSLATE) && (state == GLUT_UP))
	{
		xpos += (x - xdown);
		ypos -= (y - ydown);
		glutPostRedisplay();
	}
}

// Material properties
//float Ka = 0.2;
//float Kd = 0.4;
//float Ks = 0.4;
//float Kp = 0.5;

//---------------------------------------
// Initialize light source
//---------------------------------------

// Put following inside init() function
// glShadeModel(GL_SMOOTH);
// glEnable(GL_NORMALIZE);
// init_light(GL_LIGHT0, 0, 1, 1, 0.5, 0.5, 0.5);
// init_light(GL_LIGHT1, 0, 0, 1, 0.5, 0.5, 0.5);
// init_light(GL_LIGHT2, 0, 1, 0, 0.5, 0.5, 0.5);

// Put following inside display() function
// init_material(Ka, Kd, Ks, 100 * Kp, 0.8, 0.6, 0.4);


//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
	// Create OpenGL window
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(250, 250);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Surface");
	init();
	printf("Type r to enter ROTATE mode or t to enter TRANSLATE mode.\n");

	// Specify callback function
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
