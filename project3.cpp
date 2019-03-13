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
#include <string>
#include <iostream>
#include <vector>
using namespace std;
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Transformation variables 
#define ROTATE 1
#define TRANSLATE 2
#define COLOR 3
int xangle = 0;
int yangle = 0;
int zangle = 0;
int xpos = 0;
int ypos = 0;
int zpos = 0;

int xposa = 0;
int yposa = 0;
int zposa = 0;
float rvala = 1;
float gvala = 1;
float bvala = 1;

int xposb = 0;
int yposb = 0;
int zposb = 0;
float rvalb = 1;
float gvalb = 1;
float bvalb = 1;

int mode = ROTATE;

// Surface Variables
#define SIZE 31

// Coordinates
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];

// Surface Normals
float Nx[SIZE + 1][SIZE + 1];
float Ny[SIZE + 1][SIZE + 1];
float Nz[SIZE + 1][SIZE + 1];

// Colors
float R[SIZE + 1][SIZE + 1];
float G[SIZE + 1][SIZE + 1];
float B[SIZE + 1][SIZE + 1];
#define STEP 0.1

#include "shading.cpp"

//---------------------------------------
// Calculate random value between [-R..R]
//---------------------------------------
float myrand(float R)
{
	return (2 * R * rand()) / RAND_MAX - R;
}

//---------------------------------------
// Recursive function to split surface
//---------------------------------------
void split(int xlow, int xhigh, int ylow, int yhigh, float radius)
{
	// Check terminating condition
	if ((xhigh > xlow + 1) || (yhigh > ylow + 1))
	{
		// Calculate length of diagonal
		int xmid = (xhigh + xlow) / 2;
		int ymid = (yhigh + ylow) / 2;
		float dx = Px[xhigh][yhigh] - Px[xlow][ylow];
		float dy = Py[xhigh][yhigh] - Py[xlow][ylow];
		float dz = Pz[xhigh][yhigh] - Pz[xlow][ylow];
		float length = sqrt(dx * dx + dy * dy + dz * dz) / radius;


		// Generate five midpoints with random displacements
		Px[xlow][ymid] = (Px[xlow][ylow] + Px[xlow][yhigh]) / 2 + myrand(length);
		Py[xlow][ymid] = (Py[xlow][ylow] + Py[xlow][yhigh]) / 2 + myrand(length);
		Pz[xlow][ymid] = (Pz[xlow][ylow] + Pz[xlow][yhigh]) / 2 + myrand(length);

		Px[xhigh][ymid] = (Px[xhigh][ylow] + Px[xhigh][yhigh]) / 2 + myrand(length);
		Py[xhigh][ymid] = (Py[xhigh][ylow] + Py[xhigh][yhigh]) / 2 + myrand(length);
		Pz[xhigh][ymid] = (Pz[xhigh][ylow] + Pz[xhigh][yhigh]) / 2 + myrand(length);

		Px[xmid][ylow] = (Px[xlow][ylow] + Px[xhigh][ylow]) / 2 + myrand(length);
		Py[xmid][ylow] = (Py[xlow][ylow] + Py[xhigh][ylow]) / 2 + myrand(length);
		Pz[xmid][ylow] = (Pz[xlow][ylow] + Pz[xhigh][ylow]) / 2 + myrand(length);

		Px[xmid][yhigh] = (Px[xlow][yhigh] + Px[xhigh][yhigh]) / 2 + myrand(length);
		Py[xmid][yhigh] = (Py[xlow][yhigh] + Py[xhigh][yhigh]) / 2 + myrand(length);
		Pz[xmid][yhigh] = (Pz[xlow][yhigh] + Pz[xhigh][yhigh]) / 2 + myrand(length);

		Px[xmid][ymid] = (Px[xlow][ylow] + Px[xhigh][yhigh]) / 2 + myrand(length);
		Py[xmid][ymid] = (Py[xlow][ylow] + Py[xhigh][yhigh]) / 2 + myrand(length);
		Pz[xmid][ymid] = (Pz[xlow][ylow] + Pz[xhigh][yhigh]) / 2 + myrand(length);

		// Perform recursive calls
		split(xlow, xmid, ylow, ymid, radius);
		split(xmid, xhigh, ylow, ymid, radius);
		split(xlow, xmid, ymid, yhigh, radius);
		split(xmid, xhigh, ymid, yhigh, radius);
	}
}

//---------------------------------------
// Initialize random surface 
//---------------------------------------
void init_surface()
{
	// Initialize surface points
	Px[0][0] = -0.5;
	Py[0][0] = -0.5;
	Pz[0][0] = 0.0;
	Px[0][SIZE] = -0.5;
	Py[0][SIZE] = 0.5;
	Pz[0][SIZE] = 0.0;
	Px[SIZE][0] = 0.5;
	Py[SIZE][0] = -0.5;
	Pz[SIZE][0] = 0.0;
	Px[SIZE][SIZE] = 0.5;
	Py[SIZE][SIZE] = 0.5;
	Pz[SIZE][SIZE] = 0.0;
	split(0, SIZE, 0, SIZE, 20);

	// Define surface normals
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			// Get two tangent vectors
			float Ix = Px[i + 1][j] - Px[i][j];
			float Iy = Py[i + 1][j] - Py[i][j];
			float Iz = Pz[i + 1][j] - Pz[i][j];
			float Jx = Px[i][j + 1] - Px[i][j];
			float Jy = Py[i][j + 1] - Py[i][j];
			float Jz = Pz[i][j + 1] - Pz[i][j];

			// Do cross product
			Nx[i][j] = Iy * Jz - Iz * Jy;
			Ny[i][j] = Iz * Jx - Ix * Jz;
			Nz[i][j] = Ix * Jy - Iy * Jx;
			float length = sqrt(
				Nx[i][j] * Nx[i][j] +
				Ny[i][j] * Ny[i][j] +
				Nz[i][j] * Nz[i][j]);
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
	float radius = 1;
	glOrtho(-radius, radius, -radius, radius, -radius, radius);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	// Initialize surface
	init_surface();
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

	// You need to take this out - used for phong shading
	// init_light(GL_LIGHT0, xposa, yposa, zposa, rvala, gvala, bvala);
	// init_light(GL_LIGHT1, xposb, yposb, zposb, rvalb, gvalb, bvalb);

	//Calculate the vertex colors
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++) {
			int a = 0;
			int b = 0;
			int c = 1;
			// "find the vector V from the vertex point to the light source"
			vector<float> vA = { xposa - Px[i][j], yposa - Py[i][j], zposa - Pz[i][j]};
			// "calculate the dot product of V with the surface normal N"
			float dotProductA = vA[0] * Nx[i][j] + vA[1] * Ny[i][j] + vA[2] * Nz[i][j];
			// then multiply by 1/(a+bD+cD2) where D is the Euclidean distance from the vertex point abc
			float DA = sqrt(pow(a - vA[0], 2) + pow(b - vA[1], 2) + pow(c - vA[2], 2));
			dotProductA *= 1 / (a + b * DA + pow(c*DA, 2));
			
			// "find the vector V from the vertex point to the light source"
			vector<float> vB = { xposb - Px[i][j], yposb - Py[i][j], zposb - Pz[i][j] };
			// "calculate the dot product of V with the surface normal N"
			float dotProductB = vB[0] * Nx[i][j] + vB[1] * Ny[i][j] + vB[2] * Nz[i][j];
			// then multiply by 1/(a+bD+cD2) where D is the Euclidean distance from the vertex point abc
			float DB = sqrt(pow(a - vB[0], 2) + pow(b - vB[1], 2) + pow(c - vB[2], 2));
			dotProductB *= 1 / (a + b * DB + pow(c*DB, 2));

			R[i][j] = dotProductA * rvala + rvalb * dotProductB;
			G[i][j] = dotProductA * gvala + gvalb * dotProductB;
			B[i][j] = dotProductA * bvala + bvalb * dotProductB;
		}

	// Draw the surface
	int i, j;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
		{
			glBegin(GL_POLYGON);

			// Get the color of the point
			glColor3f(R[i][j], G[i][j], B[i][j]);
			glNormal3f(Nx[i][j], Ny[i][j], Nz[i][j]);
			glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);

			glColor3f(R[i+1][j], G[i+1][j], B[i+1][j]);
			glNormal3f(Nx[i + 1][j], Ny[i + 1][j], Nz[i + 1][j]);
			glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);

			glColor3f(R[i+1][j+1], G[i+1][j+1], B[i + 1][j + 1]);
			glNormal3f(Nx[i + 1][j + 1], Ny[i + 1][j + 1], Nz[i + 1][j + 1]);
			glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);

			glColor3f(R[i][j + 1], G[i][j + 1], B[i][j + 1]);
			glNormal3f(Nx[i][j + 1], Ny[i][j + 1], Nz[i][j + 1]);
			glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
			glEnd();
		}
	glFlush();
}

bool lightModeA = true;
bool lightModeB = true;
int lastMode;
string onOrOff(bool on) {
	return on ? "on" : "off";
}
//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
	// Determine if we are in ROTATE or TRANSLATE mode
	if ((tolower(key) == 'r') && mode != COLOR)
	{
		cout << "Rotate mode" << endl;
		// printf("Type x y z to decrease or X Y Z to increase ROTATION angles.\n");
		mode = ROTATE;
	}
	else if (tolower(key) == 't' && mode != COLOR)
	{
		cout << "Translate mode" << endl;
		// printf("Type x y z to decrease or X Y Z to increase TRANSLATION distance.\n");
		mode = TRANSLATE;
	}
	else if (tolower(key) == 'c') {
		mode = (mode == COLOR) ? ROTATE : COLOR;
		if (mode == ROTATE) {
			cout << "Back to rotate mode" << endl;
		}
		else {
			cout << "Color mode " << onOrOff(mode == COLOR) << endl;
		}
	}

	if (mode == COLOR) {
		// Handle light 1
		if (lightModeA) {
			if (key == 'r')
				rvala -= .1;
			else if (key == 'g')
				gvala -= .1;
			else if (key == 'b')
				bvala -= .1;
			else if (key == 'R')
				rvala += .1;
			else if (key == 'G')
				gvala += .1;
			else if (key == 'B')
				bvala += .1;
			cout << "RGB: " << rvala << " " << gvala << " " << bvala << endl;
			if (key == 'x')
				xposa -= 1;
			else if (key == 'y')
				yposa -= 1;
			else if (key == 'z')
				zposa -= 1;
			else if (key == 'X')
				xposa += 1;
			else if (key == 'Y')
				yposa += 1;
			else if (key == 'Z')
				zposa += 1;
			cout << "camera 1 Position x,y,z: " << xposa << ", " << yposa << ", " << zposa << endl;
		}

		// Handle light 2
		if (lightModeB) {
			if (key == 'r')
				rvalb -= .1;
			else if (key == 'g')
				gvalb -= .1;
			else if (key == 'b')
				bvalb -= .1;
			else if (key == 'R')
				rvalb += .1;
			else if (key == 'G')
				gvalb += .1;
			else if (key == 'B')
				bvalb += .1;
			if (key == 'x')
				xposb -= 1;
			else if (key == 'y')
				yposb -= 1;
			else if (key == 'z')
				zposb -= 1;
			else if (key == 'X')
				xposb += 1;
			else if (key == 'Y')
				yposb += 1;
			else if (key == 'Z')
				zposb += 1;
			cout << "camera 2 Position x,y,z: " << xposb << ", " << yposb << ", " << zposb << endl;
		}
	}
	else {
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
	}
	if (key == 'i')
		init_surface();
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
	if (key == '1') {
		lightModeA = !lightModeA;
		cout << "light mode A " << onOrOff(lightModeA) << endl;
	}
	if (key == '2') {
		lightModeB = !lightModeB;
		cout << "light mode B " << onOrOff(lightModeB) << endl;
	}
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
		xangle += (y - ydown) / 4;
		yangle -= (x - xdown) / 4;
		zangle = 0;
		glutPostRedisplay();
	}

	// Handle TRANSLATE
	if ((mode == TRANSLATE) && (state == GLUT_UP))
	{
		xpos += (x - xdown) / 4;
		ypos -= (y - ydown) / 4;
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