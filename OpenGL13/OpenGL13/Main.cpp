#define GLEW_STATIC
#define FREEGLUT_STATIC
#pragma comment (lib, "freeglut_static.lib")
#pragma comment (lib, "libglew32.lib")
//#pragma comment (lib, "libpng16.lib")
#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
//#include <png.h>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <math.h>

#include "LoadShaders.h"

using namespace std;

int refreshMS = 15;
GLuint  prog_hdlr, program;
GLint location_attribute_0, location_viewport;
const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
const float light0_position[4] = {1,1,1,0};

static const GLfloat f[] =
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

unsigned int ind;
unsigned indarr[] = {0,1,2};

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glUseProgram(prog_hdlr);

	/*
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, program);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	lDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, program);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glutSwapBuffers();
}
void process_keys(unsigned char key, int x, int y)
{
	if(key == 27)exit(0);
}
void reshape(GLsizei w, GLsizei h)
{
	if(h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0*(GLfloat)w/(GLfloat)h, 0.1f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
}
void timer(int value)
{
	glutPostRedisplay(); //Post re-paint request to activate display()
	glutTimerFunc(refreshMS, timer, 0); //next timer call milliseconds later
}
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("GLSL tutorial");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(process_keys);
	//glutIdleFunc(display);
	
	glClearColor(0.7f, 0.7f, 0.3f, 1.0f);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glewInit();
	if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
	{
		printf("Ready for GLSL - vertex, fragment, and geometry units\n");
		setShaders(prog_hdlr, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");
		location_attribute_0 = glGetAttribLocation(prog_hdlr, "R"); //radius
		location_viewport = glGetUniformLocation(prog_hdlr, "viewport"); //viewport
		//
		glGenBuffers(1, &program);
		glGenBuffers(1, &ind);
		glBindBuffer(GL_ARRAY_BUFFER, program);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);

		glBufferData(GL_ARRAY_BUFFER, sizeof(f), f, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indarr), indarr, GL_STATIC_DRAW);
	}
	else
	{
		printf("No GLSL support\n");
	}
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}