#ifdef _WIN32
	#define NOMINMAX	
	#include"GL/freeglut.h"	// Windows machines: freeGLUT 3 is provided here, make sure you have x64 set as architecture
	#pragma comment(lib,"lib/glew32.lib")
	#pragma comment(lib,"lib/freeglut.lib")
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#else
	#include<GLUT/glut.h> // MacOS: comes with GLUT pre-installed
#endif
// Selman Tabet (@selmantabet - https://selman.io/) - Implementation of the Marching Cubes Algorithm (HBKU DataVis Course Assignment)
// MyVolume variable in the init function can take a .dat file (volume dataset). Modify as deemed necessary.
// Press the plus (+) key to increase the isovalue and the minus (-) key to decrease it.
#include<iostream>
#include"timer.h"
#include<vector>
#include"ext_math.h"
#include<cassert>


// ASSIGNMENT 2: MARCHING CUBES
// First, we sample a test function to a regular grid. 
// The test function has concentric sphere as iso-surfaces.
// To do so, we're including a volume class (see file volume.h)
#include"volume.h"

volume generate_radial_volume(int dims = 128) {
	volume vol(dims, dims, dims);
	for (int k = 0; k < vol.dimension(2); k++) {
		float z = 2.0f * float(k) / (vol.dimension(2) - 1) - 1.0f;
		for (int j = 0; j < vol.dimension(1); j++) {
			float y = 2.0f * float(j) / (vol.dimension(1) - 1) - 1.0f;
			for (int i = 0; i < vol.dimension(0); i++) {
				float x = 2.0f * float(i) / (vol.dimension(0) - 1) - 1.0f;
				// normalize to 0,1, denser material inside...
				vol(i, j, k) = 1.0f-sqrt(x * x + y * y + z * z)/sqrt(3.0f);
			}
		}
	}
	return vol;
}

volume MyVolume;

// TASK:: [TODO] This is where your MarchingCubes code fragments will go.
// Look at MC.h and MC.cpp.
#include"MC.h"
float isovalue = 0.2f;	// default isovalue

// TASK:: [TODO] Some of the volumes you will be working with
// are fairly large. In order to get a quick preview, implement
// the subsampling routing in volume.h as well.


// This is a shallow wrapper for a triangle mesh
#include"mesh.h"
mesh MyMesh; // This will be our triangle model

int winwidth = 512;
int winheight = 512;

timer myTimer;

void display(void) {
	// This function will be called to generate a new frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear both color buffer and depth buffer

	glLoadIdentity();	
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef(60.0f * (float)myTimer.query(), 0.0f, 1.0f, 0.0f);
	
	// Beware, deprecated, slow OpenGL
	glBegin(GL_TRIANGLES);
	for (int n = 0; n < MyMesh.nTriangles(); n++) {
		auto tri = MyMesh.triangle(n);
		for (int k = 0; k < 3; k++) {
			glColor3fv(MyMesh.color(tri[k]));
			glNormal3fv(MyMesh.normal(tri[k]));
			glVertex3fv(MyMesh.position(tri[k]));
		}
	}
	glEnd();

	glutSwapBuffers();											// Show rendered image
}

void keyboard(unsigned char key, int x, int y) {
	// key will contain the key that was pressed
	// x,y are the mouse coordinates when the key was pressed.	
	switch (key) {
	case '-':
	{
		isovalue = std::max(isovalue - 0.05f, 0.0f);
		MarchingCubes MC(MyVolume);
		MyMesh = MC.compute(isovalue);
		break;
	}
	case '+':
	{
		isovalue = std::min(isovalue + 0.05f, 1.0f);
		MarchingCubes MC(MyVolume);
		MyMesh = MC.compute(isovalue);
		break;
	}
	}
	if (key == 27) exit(0);
}

void special_key(int key, int x, int y) {
	// key will contain a code (e.g., GLUT_KEY_F1, GLUT_KEY_LEFT, ...)
	// See https://www.opengl.org/resources/libraries/glut/spec3/node54.html
	// x,y are the mouse coordinates when the key was pressed.

	if (key == GLUT_KEY_F1) {
		std::cout << std::endl;
		std::cout << "F1 : This help message" << std::endl;
		std::cout << "ESC: close window" << std::endl;
	}
}

void mouse_button(int button, int state, int x, int y) {
	// button can be GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, GLUT_MIDDLE_BUTTON
	// state can be GLUT_UP or GLUT_DOWN
	// x,y is the mouse coordinate
}

void mouse_motion(int x, int y) {
	// This function is called when the mouse is moved and a button is pressed
	// x,y are the mouse coordinates
}

void mouse_passive_motion(int x, int y) {
	// This function is called when the mouse is moved and no button is pressed
	// x,y are the mouse coordinates
}

void reshape(int width, int height) {
	// This function is called when the window is resized
	glViewport(0, 0, width, height);
	double aspect = double(width) / double(height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, aspect, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	winwidth = width;
	winheight = height;
}

void init(void) {
	// Here, we set up our data structures and OpenGL state
	// As an example, we can set the background color:
	glClearColor(0.3f, 0.3f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	myTimer.reset();
	MyVolume = generate_radial_volume(64);
	//MyVolume.import_dat("stagbeetle832x832x494.dat");
	MyVolume.subsample();
	MarchingCubes MC(MyVolume);
	MyMesh = MC.compute(isovalue);
	MyMesh.export_obj("latest.obj");
	
	// NOW, set up fixed function lighting... meh.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT0, GL_POSITION, vec4f(0.0f, 3.0f, 0.0f, 1.0f));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, vec4f(0.8f, 0.8f, 0.8f, 1.0f));
	glLightfv(GL_LIGHT0, GL_SPECULAR, vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, vec4f(0.1f, 0.1f, 0.1f, 1.0f));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, vec4f(64.0f,64.0f,64.0f,0.0f));
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
}

void cleanup(void) {
	// Here, you can clean up data structures
}



int main(int argc, char** argv) {
	atexit(cleanup);											// This makes sure that you call cleanup() before exiting
	std::cout << "CPEG 418 Assignment 2" << std::endl;
	glutInit(&argc, argv);										// Initialize GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// Specify display parameters: double RGBA-color buffer plus depth buffer
	glutInitWindowSize(winwidth, winheight);					// Set up the initial window size
	glutCreateWindow("Assignment 2");							// Create a window with title "Assignment 1"
	glutDisplayFunc(display);									// The display() function is called for each new frame
	glutIdleFunc(glutPostRedisplay);							// Keep on rendering frames
	glutKeyboardFunc(keyboard);									// The keyboard() function is called when a key is pressed
	glutSpecialFunc(special_key);								// Called when a special key (F1, LEFT, RIGHT, etc.) is pushed
	glutMouseFunc(mouse_button);								// The mouse_button() function is called when the user clicks or releases a button
	glutMotionFunc(mouse_motion);								// The mouse_motion() function is called when the mouse moves while a button is pressed
	glutPassiveMotionFunc(mouse_passive_motion);				// The mouse_passive_motion() function is called then the mouse moves without a button begin pressed.
	glutReshapeFunc(reshape);									// The reshape() function is called when the window is resized
	init();														// This function initializes a few OpenGL things
	glutShowWindow();											// Show the window
	glutMainLoop();												// enter the main loop
	return 0;
}